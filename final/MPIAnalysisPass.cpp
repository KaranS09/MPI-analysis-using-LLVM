#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
#include <set>
#include <vector>

using namespace llvm;

namespace
{
    struct MPICommunication
    {
        std::string type; // "Send" or "Recv"
        std::string comm;
        int tag;
        int rank;
    };

    struct MPIAnalysisPass : public PassInfoMixin<MPIAnalysisPass>
    {
        std::vector<MPICommunication> mpiCalls;

        PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM)
        {
            errs() << "MPIAnalysisPass running on function: " << F.getName() << "\n";

            for (auto &BB : F)
            {
                for (auto &I : BB)
                {
                    if (auto *call = dyn_cast<CallInst>(&I))
                    {
                        if (Function *calledFunc = call->getCalledFunction())
                        {
                            StringRef funcName = calledFunc->getName();
                            if (funcName.equals("MPI_Send") || funcName.equals("MPI_Recv"))
                            {
                                analyzeMPICall(call, funcName);
                            }
                        }
                    }
                }
            }

            analyzeUniformParticipation();
            return PreservedAnalyses::all();
        }

        void analyzeMPICall(CallInst *call, StringRef funcName)
        {
            MPICommunication mpiComm;
            mpiComm.type = funcName.str();

            // Assuming MPI_COMM_WORLD for simplicity
            mpiComm.comm = "MPI_COMM_WORLD";

            // Extract tag (5th argument for both Send and Recv)
            if (auto *tagArg = dyn_cast<ConstantInt>(call->getArgOperand(4)))
            {
                mpiComm.tag = tagArg->getSExtValue();
            }

            // Extract rank (3rd argument for Send, 3rd for Recv)
            int rankArgIndex = (funcName.equals("MPI_Send")) ? 3 : 3; // Both should use index 3
            if (auto *rankArg = dyn_cast<ConstantInt>(call->getArgOperand(rankArgIndex)))
            {
                mpiComm.rank = rankArg->getSExtValue();
            }

            mpiCalls.push_back(mpiComm);

            errs() << "[INFO] Detected MPI " << mpiComm.type << ": comm=" << mpiComm.comm
                   << ", tag=" << mpiComm.tag << ", rank=" << mpiComm.rank << "\n";
        }

        void analyzeUniformParticipation()
        {
            errs() << "[INFO] Analyzing Uniform Participation Patterns...\n";

            std::map<std::pair<std::string, int>, std::set<int>> participationMap;

            for (const auto &call : mpiCalls)
            {
                participationMap[{call.comm, call.tag}].insert(call.rank);
            }

            for (const auto &entry : participationMap)
            {
                if (entry.second.size() > 1)
                {
                    errs() << "[INFO] Uniform Participation Detected in Comm " << entry.first.first
                           << " with Tag " << entry.first.second << " involving Ranks: ";
                    for (int rank : entry.second)
                    {
                        errs() << rank << " ";
                    }
                    errs() << "\n";

                    errs() << "Uniform Participation Report:\n";
                    errs() << "------------------------------------\n";
                    errs() << "- Communicator: " << entry.first.first << "\n";
                    errs() << "- Tag: " << entry.first.second << "\n";
                    errs() << "- Participating Ranks: {";
                    for (auto it = entry.second.begin(); it != entry.second.end(); ++it)
                    {
                        if (it != entry.second.begin())
                            errs() << ", ";
                        errs() << *it;
                    }
                    errs() << "}\n";
                    errs() << "This indicates that both MPI_Send and MPI_Recv operations with tag "
                           << entry.first.second << " in communicator\n"
                           << entry.first.first
                           << " involve these ranks.\n\n";
                }
            }
        }

        static bool isRequired() { return true; }
    };
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo()
{
    return {
        LLVM_PLUGIN_API_VERSION, "MPIAnalysisPass", LLVM_VERSION_STRING,
        [](PassBuilder &PB)
        {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>)
                {
                    if (Name == "mpi-analysis")
                    {
                        FPM.addPass(MPIAnalysisPass());
                        return true;
                    }
                    return false;
                });
        }};
}

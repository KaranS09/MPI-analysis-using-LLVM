#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct MPIAnalysisPass : public PassInfoMixin<MPIAnalysisPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
    errs() << "MPIAnalysisPass running on function: " << F.getName() << "\n";
    bool modified = false;
    for (auto &BB : F) {
      for (auto &I : BB) {
        if (auto *call = dyn_cast<CallInst>(&I)) {
          if (Function *calledFunc = call->getCalledFunction()) {
            StringRef funcName = calledFunc->getName();
            errs() << "Found direct function call: " << funcName << "\n";
            if (funcName.equals("MPI_Send") || funcName.equals("MPI_Recv")) {
              errs() << "Found MPI communication call: " << funcName << " in function " << F.getName() << "\n";
              modified = true;
            }
          } else {
            errs() << "Found indirect call\n";
          }
        }
      }
    }
    return PreservedAnalyses::all();
  }
  // Add this function to make the pass run on optnone functions
  static bool isRequired() { return true; }
};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  errs() << "MPIAnalysisPass plugin loaded\n";
  return {
    LLVM_PLUGIN_API_VERSION, "MPIAnalysisPass", LLVM_VERSION_STRING,
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
           ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "mpi-analysis") {
            errs() << "MPIAnalysisPass added to pipeline\n";
            FPM.addPass(MPIAnalysisPass());
            return true;
          }
          return false;
        });
    }
  };
}

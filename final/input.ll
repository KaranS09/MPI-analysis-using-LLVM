; ModuleID = 'mpi_example.c'
source_filename = "mpi_example.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct._comm = type opaque
%struct._dtype = type opaque
%struct._status = type { i32, i32, i32, i32 }

@lam_mpi_comm_world = external global %struct._comm, align 1
@lam_mpi_int = external global %struct._dtype, align 1
@.str = private unnamed_addr constant [29 x i8] c"Process 1 received data: %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [29 x i8] c"Process 3 received data: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 noundef %0, i8** noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca %struct._comm*, align 8
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  %12 = call i32 @MPI_Init(i32* noundef null, i8*** noundef null)
  %13 = call i32 @MPI_Comm_rank(%struct._comm* noundef @lam_mpi_comm_world, i32* noundef %6)
  %14 = load i32, i32* %6, align 4
  %15 = icmp eq i32 %14, 0
  br i1 %15, label %16, label %19

16:                                               ; preds = %2
  store i32 100, i32* %7, align 4
  %17 = bitcast i32* %7 to i8*
  %18 = call i32 @MPI_Send(i8* noundef %17, i32 noundef 1, %struct._dtype* noundef @lam_mpi_int, i32 noundef 1, i32 noundef 0, %struct._comm* noundef @lam_mpi_comm_world)
  br label %28

19:                                               ; preds = %2
  %20 = load i32, i32* %6, align 4
  %21 = icmp eq i32 %20, 1
  br i1 %21, label %22, label %27

22:                                               ; preds = %19
  %23 = bitcast i32* %8 to i8*
  %24 = call i32 @MPI_Recv(i8* noundef %23, i32 noundef 1, %struct._dtype* noundef @lam_mpi_int, i32 noundef 0, i32 noundef 0, %struct._comm* noundef @lam_mpi_comm_world, %struct._status* noundef null)
  %25 = load i32, i32* %8, align 4
  %26 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([29 x i8], [29 x i8]* @.str, i64 0, i64 0), i32 noundef %25)
  br label %27

27:                                               ; preds = %22, %19
  br label %28

28:                                               ; preds = %27, %16
  %29 = load i32, i32* %6, align 4
  %30 = srem i32 %29, 2
  %31 = load i32, i32* %6, align 4
  %32 = call i32 @MPI_Comm_split(%struct._comm* noundef @lam_mpi_comm_world, i32 noundef %30, i32 noundef %31, %struct._comm** noundef %9)
  %33 = load i32, i32* %6, align 4
  %34 = icmp eq i32 %33, 2
  br i1 %34, label %35, label %39

35:                                               ; preds = %28
  store i32 200, i32* %10, align 4
  %36 = bitcast i32* %10 to i8*
  %37 = load %struct._comm*, %struct._comm** %9, align 8
  %38 = call i32 @MPI_Send(i8* noundef %36, i32 noundef 1, %struct._dtype* noundef @lam_mpi_int, i32 noundef 3, i32 noundef 1, %struct._comm* noundef %37)
  br label %49

39:                                               ; preds = %28
  %40 = load i32, i32* %6, align 4
  %41 = icmp eq i32 %40, 3
  br i1 %41, label %42, label %48

42:                                               ; preds = %39
  %43 = bitcast i32* %11 to i8*
  %44 = load %struct._comm*, %struct._comm** %9, align 8
  %45 = call i32 @MPI_Recv(i8* noundef %43, i32 noundef 1, %struct._dtype* noundef @lam_mpi_int, i32 noundef 2, i32 noundef 1, %struct._comm* noundef %44, %struct._status* noundef null)
  %46 = load i32, i32* %11, align 4
  %47 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([29 x i8], [29 x i8]* @.str.1, i64 0, i64 0), i32 noundef %46)
  br label %48

48:                                               ; preds = %42, %39
  br label %49

49:                                               ; preds = %48, %35
  %50 = call i32 @MPI_Comm_free(%struct._comm** noundef %9)
  %51 = call i32 @MPI_Finalize()
  ret i32 0
}

declare i32 @MPI_Init(i32* noundef, i8*** noundef) #1

declare i32 @MPI_Comm_rank(%struct._comm* noundef, i32* noundef) #1

declare i32 @MPI_Send(i8* noundef, i32 noundef, %struct._dtype* noundef, i32 noundef, i32 noundef, %struct._comm* noundef) #1

declare i32 @MPI_Recv(i8* noundef, i32 noundef, %struct._dtype* noundef, i32 noundef, i32 noundef, %struct._comm* noundef, %struct._status* noundef) #1

declare i32 @printf(i8* noundef, ...) #1

declare i32 @MPI_Comm_split(%struct._comm* noundef, i32 noundef, i32 noundef, %struct._comm** noundef) #1

declare i32 @MPI_Comm_free(%struct._comm** noundef) #1

declare i32 @MPI_Finalize() #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}

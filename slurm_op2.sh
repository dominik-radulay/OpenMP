#!/bin/bash

# load module for intel compiler - icc used for compilation
module load compilers/intel/2019u5 

usage() {
    echo $0 numberOfThreads numberOfRepeats
}

if [[ $# -ne 2 ]]; then
    usage
    exit -1
fi

ulimit -s unlimited #OS limits for the program.
  
THREADS=$1
REPEATS=$2

export OMP_PROC_BIND=TRUE
export OMP_DYNAMIC=FALSE
export OMP_STACKSIZE=1g # tells the OpenMP implementation about how much stack to actually allocate for each of the stacks. 

echo "Node list                    : $SLURM_JOB_NODELIST"
echo "Number of nodes allocated    : $SLURM_JOB_NUM_NODES or $SLURM_NNODES"
echo "Number of threads or processes          : $SLURM_NTASKS"
echo "Number of processes per node : $SLURM_TASKS_PER_NODE"
echo "Requested tasks per node     : $SLURM_NTASKS_PER_NODE"
echo "Requested CPUs per task      : $SLURM_CPUS_PER_TASK"
echo "Scheduling priority          : $SLURM_PRIO_PROCESS"

echo  OMP_PROC_BIND is $OMP_PROC_BIND, OMP_DYNAMIC is $OMP_DYNAMIC

    export OMP_NUM_THREADS=$THREADS
    echo running on $OMP_NUM_THREADS threads;echo '---'; echo
    for k in `seq 1 $REPEATS`; do
    ./op2_omp.o files/a.dat files/b.dat files/c.dat;echo '---'; echo
    done
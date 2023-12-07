# LINFO1252 – Systèmes informatiques

## Project Structure

- **posix:** Implementations using Posix threads and basic synchronization methods.
- **ts:** Implementations using our version of Test-and-Set lock.
- **tts:** Implementations using our version of Test-and-Test-and-Set lock combined with our version of Semaphores.
- **locker.h, locker.c:** Common implementation of lockers used in the `ts` folder.
- **locker_tts.h, locker_tts.c:** Implementation of Test-and-Test-and-Set locker used in the `tts` folder.
- **sem.h, sem.c:** Implementation of Semaphores used in the `tts` folder.
- **Makefile:** Build and run automation script.
- **experiments.sh:** Performance testing script.
- **plot.py:** Python script for plotting performance results.

## Building and Running

### Prerequisites

- GCC (GNU Compiler Collection)
- Python3 (for plotting)

### Build

For all the following commands, run them in the project root directory where the 'Makefile' file is.

To build, run and plot the results, run:

```bash
make
```

To build the project and get the compile files, run:

```bash
make build
```

To only run the experiments script, the compile files must exists:

```bash
make experiments
```

To only plot the results, the csv files must exists:

```bash
make plot
```

To clean up all generated file, run:

```bash
make clean
```

You can also compile a specific file by specifying its name, or delete all compiled file:

```bash
make <FILENAME>
make clean_comp
```
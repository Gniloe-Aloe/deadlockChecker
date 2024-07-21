## Contents

The repository contains two main files:

- deadlockCreator.cpp - Compiles and optionally creates a process that may experience a deadlock after being run.
- deadlock_checker.sh - Takes the PID of a process and attempts to detect a deadlock by analyzing logs from strace.

## Usage

Compile deadlockCreator.cpp:
```sh
g++ deadlockCreator.cpp
```
Run the process in the background:
```sh
./a.out 0 & # No deadlock created
# Or
./a.out 1 & # Deadlock created
```
Obtain the PID of your process:
```sh
ps | grep a.out
```
Run the deadlock checker script:
```sh
bash deadlock_checker.sh <PID>
```
After a few seconds, you will receive logs analyzing the process. The message [FAILED] will be displayed in purple if a potential deadlock is detected. If no deadlock is found, the message [PASSED] will be displayed.















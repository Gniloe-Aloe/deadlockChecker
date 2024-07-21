#!/bin/bash

# Check if PID argument is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <PID>"
    exit 1
fi

PID=$1
LOGFILE="trace.log"

# Check if the process with the given PID exists
if ! ps -p "$PID" > /dev/null 2>&1; then
    echo "Process with PID $PID does not exist."
    exit 1
fi

# Start strace on the given PID and log to file
echo "Starting strace on process with PID $PID..."
strace -p "$PID" -o "$LOGFILE" -f -tt &

STRACE_PID=$!

# Wait a few seconds to accumulate strace data
sleep 5

# Stop strace
kill -INT "$STRACE_PID"

# List of system calls that may indicate a deadlock
SYSCALLS=("FUTEX_WAIT_PRIVATE" "FUTEX_WAIT" "waitpid" "wait" "recvfrom" "recv" "sendto" "send" "epoll_wait" "epoll_pwait" "semop" "read" "write" "SIGTSTP")

# Threshold for the number of repetitions to determine deadlock
THRESHOLD=2

# ANSI color codes for purple text
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

# Function to check for deadlock
check_deadlock() {
    for SYSCALL in "${SYSCALLS[@]}"; do
        # Check the number of repeated system calls
        COUNT=$(grep "$SYSCALL" "$LOGFILE" | wc -l)
        
        if [ "$COUNT" -ge "$THRESHOLD" ]; then
            echo -e "${PURPLE}[FAILED]Possible deadlock detected: '$SYSCALL' repeats $COUNT times.${NC}"
            return 0
        fi
    done
    echo -e "${PURPLE}[PASSED]No deadlock detected.${NC}"
    return 1
}

# Run the deadlock check function
check_deadlock

# Output the log for review
cat "$LOGFILE"

# Clean up the temporary file
rm -f "$LOGFILE"

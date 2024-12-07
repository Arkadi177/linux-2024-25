#!/bin/bash

# Script3: Run and Validate
set -e

# Step 1: Run Script1 to set up the infrastructure
echo "Running directory_creater.sh to set up infrastructure..."
bash directory_creater.sh

# Step 2: Run the watcher in the background and redirect its output to results.txt
echo "Starting watcher_demo to watch the directory..."
./watcher_demo watcher_directory > results.txt 2>&1 &
WATCHER_PID=$!

# Step 3: Allow the watcher to initialize
echo "Watcher process started with PID: $WATCHER_PID"
sleep 2

# Step 4: Check if watcher_demo is still running and show its output
if ps -p $WATCHER_PID > /dev/null; then
    echo "Watcher process is still running."
else
    echo "Watcher process with PID $WATCHER_PID has already stopped. Here is the output from watcher_demo:"
    cat results.txt
    exit 1
fi

# Step 5: Run Script2 to modify the infrastructure
echo "Running modify.sh to make changes to the infrastructure..."
bash modify.sh

# Step 6: Allow changes to be logged by the watcher
echo "Allowing changes to be logged..."
sleep 2

# Step 7: Check if the watcher process is still running before killing it
if ps -p $WATCHER_PID > /dev/null; then
    echo "Stopping watcher_demo..."
    kill $WATCHER_PID
else
    echo "Error: watcher_demo process with PID $WATCHER_PID is not running."
    exit 1
fi

# Step 8: Define the expected results
cat <<EOF > expected_results.txt
File Modified: file1.txt
File Created: new_file.txt
File Deleted: file2.txt
EOF

# Step 9: Compare the actual results with the expected ones
echo "Comparing results with expected output..."
if diff -q results.txt expected_results.txt; then
    echo "Test passed: Results match expected output."
else
    echo "Test failed: Results do not match expected output."
    diff results.txt expected_results.txt
fi

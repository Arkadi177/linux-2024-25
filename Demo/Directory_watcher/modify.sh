#!/bin/bash

# Script2: Modify Infrastructure
set -e

# Modify an existing file
echo "Appending text to file1" >> watcher_directory/file1.txt

# Create a new file
touch watcher_directory/new_file.txt

# Delete an existing file
rm -f watcher_directory/file2.txt

echo "Infrastructure modifications completed."

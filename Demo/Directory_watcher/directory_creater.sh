#!/bin/bash

# Script1: Setup Infrastructure
set -e

# Create a directory to watch
mkdir -p watcher_directory

# Create some files in the directory
touch watcher_directory/file1.txt
touch watcher_directory/file2.txt

echo "Infrastructure setup completed."

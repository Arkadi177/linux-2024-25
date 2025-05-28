#!/bin/bash

SERVER_BIN=../../cmake-build-debug/Demo/FTP/server
CLIENT_BIN=../../cmake-build-debug/Demo/FTP/client
OUT_DIR=../../cmake-build-debug/Demo/FTP
SERVER_LOG=$OUT_DIR/server_log.txt

# Start server in background
$SERVER_BIN  > "$SERVER_LOG" 2>&1 &
SERVER_PID=$!
echo "Started FTP server with PID $SERVER_PID"
sleep 1

# Start client interactively
$CLIENT_BIN 127.0.0.1

# After client exits, stop the server
kill $SERVER_PID
echo "Server stopped."

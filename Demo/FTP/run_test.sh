#!/bin/bash

SERVER_BIN=../../cmake-build-debug/Demo/FTP/server
CLIENT_BIN=../../cmake-build-debug/Demo/FTP/client
OUT_DIR=../../cmake-build-debug/Demo/FTP
SERVER_PORT=2121
SERVER_LOG=$OUT_DIR/server_log.txt

# Start server
$SERVER_BIN $SERVER_PORT > "$SERVER_LOG" 2>&1 &
SERVER_PID=$!
sleep 1

echo "Started FTP server with PID $SERVER_PID"

# Define test for each client
run_client() {
  local CLIENT_ID=$1
  local BASE_PORT=$((50000 + CLIENT_ID * 10))
  local OUT_FILE="$OUT_DIR/ftp_test_output_client_$CLIENT_ID.txt"

  $CLIENT_BIN 127.0.0.1 $SERVER_PORT <<EOF | tee "$OUT_FILE"
USER admin
PASS secret123
PORT 127,0,0,1,$((BASE_PORT / 256)),$((BASE_PORT % 256))
PWD
MKD testdir$CLIENT_ID
CWD testdir$CLIENT_ID
PWD
PORT 127,0,0,1,$(((BASE_PORT+1) / 256)),$(((BASE_PORT+1) % 256))
LIST
PORT 127,0,0,1,$(((BASE_PORT+2) / 256)),$(((BASE_PORT+2) % 256))
RETR foo.txt
QUIT
EOF
}

# Launch multiple clients in parallel
NUM_CLIENTS=4
for ((i=0; i<NUM_CLIENTS; i++)); do
  run_client $i &
  sleep 3
done

# Wait for all clients
wait

# Basic verification
for ((i=0; i<NUM_CLIENTS; i++)); do
  OUT_FILE="$OUT_DIR/ftp_test_output_client_$i.txt"
  echo "=== Verifying client $i ==="
  grep -q "230 Login successful" "$OUT_FILE" && echo "Client $i LOGIN OK"
  grep -q "257" "$OUT_FILE" && echo "Client $i PWD OK"
  sleep 2
done

# Cleanup
echo "Server stopped."

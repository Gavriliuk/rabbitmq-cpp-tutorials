#!/bin/sh

set -x
../build/src/receive_logs_topic "*.error" &
sleep 1
../build/src/receive_logs_topic "database.*" "network.*" &
sleep 1
../build/src/emit_log_topic "login.error" "Incorrect username or password"
../build/src/emit_log_topic "network.error" "Connection is broken, reconnect"
../build/src/emit_log_topic "database.warning" "Data storage is out of space"

set +x
sleep 1
kill $(ps -u | grep "../build/src/receive_logs_topic" | grep -v "grep" | grep -o "^[^ ]*[ ]*[^ ]*" | grep -o "[^ ]*$")

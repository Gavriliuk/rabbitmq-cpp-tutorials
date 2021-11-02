#!/bin/sh

set -x
../build/src/receive_logs_direct "error" &
sleep 1
../build/src/receive_logs_direct "info" "warning" "error" &
sleep 1
../build/src/emit_log_direct "info"    "This is the INFO log message 1"
../build/src/emit_log_direct "warning" "This is the WARNING log message 2"
../build/src/emit_log_direct "error"   "This is the ERROR log message 3"

set +x
sleep 1
kill $(ps -u | grep "../build/src/receive_logs_direct" | grep -v "grep" | grep -o "^[^ ]*[ ]*[^ ]*" | grep -o "[^ ]*$")

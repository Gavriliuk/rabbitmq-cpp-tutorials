#!/bin/sh

set -x
../build/src/receive_logs &
sleep 1
../build/src/emit_log "info: This is the log message 1"
../build/src/emit_log "info: This is the log message 2"
../build/src/emit_log "info: This is the log message 3"

set +x
sleep 1
kill $(ps -u | grep "../build/src/receive_logs" | grep -v "grep" | grep -o "^[^ ]*[ ]*[^ ]*" | grep -o "[^ ]*$")

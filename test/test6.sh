#!/bin/sh

set -x
../build/src/rpc_client &
sleep 1

../build/src/rpc_server &
sleep 1

../build/src/rpc_client &

set +x
sleep 1
kill $(ps -u | grep -e "../build/src/rpc_client" -e "../build/src/rpc_server" | grep -v "grep" | grep -o "^[^ ]*[ ]*[^ ]*" | grep -o "[^ ]*$")

#!/bin/sh

set -x
../build/src/send
../build/src/send

../build/src/receive &
sleep 1

../build/src/send
../build/src/send

set +x
sleep 1
kill $(ps -u | grep "../build/src/receive" | grep -v "grep" | grep -o "^[^ ]*[ ]*[^ ]*" | grep -o "[^ ]*$")

#!/bin/sh

set -x

../build/src/worker &
sleep 1

../build/src/worker &
sleep 1

../build/src/new_task "A very hard task 1 which takes two seconds.."
../build/src/new_task "A very hard task 2 which takes two seconds.."
../build/src/new_task "A very hard task 3 which takes two seconds.."

set +x
sleep 5
kill $(ps -u | grep "../build/src/worker" | grep -v "grep" | grep -o "^[^ ]*[ ]*[^ ]*" | grep -o "[^ ]*$")

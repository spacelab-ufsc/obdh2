#!/bin/bash

cppcheck --force --error-exitcode=-1 --inline-suppr --addon=misra.json ../system
cppcheck --force --error-exitcode=-1 --addon=misra.json ../devices
cppcheck --std=c99 --force --error-exitcode=-1 --inline-suppr --addon=misra.json ../drivers
cppcheck --std=c99 --force --error-exitcode=-1 --inline-suppr --addon=misra.json ../app/tasks
cppcheck --std=c99 --force --error-exitcode=-1 --addon=misra.json ../app/utils
cppcheck --std=c99 --force --error-exitcode=-1 --addon=misra.json ../app/structs

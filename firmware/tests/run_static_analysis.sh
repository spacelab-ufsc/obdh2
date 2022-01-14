#!/bin/bash

cppcheck --error-exitcode=-1 --inline-suppr --addon=misra.json ../system
cppcheck --error-exitcode=-1 --addon=misra.json ../devices
cppcheck --std=c99 --error-exitcode=-1 --inline-suppr --addon=misra.json ../drivers
cppcheck --std=c99 --force --error-exitcode=-1 --addon=misra.json ../app/tasks

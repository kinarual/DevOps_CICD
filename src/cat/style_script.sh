#!/bin/bash

if /usr/bin/grep -q 'warning: code should be clang-formatted' style_output.txt; 
then
exit 1
fi

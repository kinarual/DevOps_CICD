#!/bin/bash

if /usr/bin/grep -q 'warning: code should be clang-formatted' style_output.txt > /dev/null; 
then
    exit 1
else
    cat style_output.txt
    echo "No error"
fi
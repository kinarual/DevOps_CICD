#!/bin/bash

cd cat || exit 1
touch cat_style_test.log
make style_check 2>&1 | tee cat_style_test.log
if grep -q 'warning:.*clang-formatted' cat_style_test.log; then
    echo "Style formatting does not match in cat directory."
    exit 1
fi

cd ../grep || exit 1
touch grep_style_test.log
make style_check 2>&1 | tee grep_style_test.log
if grep -q 'warning:.*clang-formatted' grep_style_test.log; then
    echo "Style formatting does not match in grep directory."
    exit 1
fi

echo "No style warnings found in any files."
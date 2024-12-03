#!/bin/bash

cd cat || exit 1
touch cat_unit_test.log
make test | tee cat_unit_test.log
if grep -q "differ" cat_unit_test.log; then
    echo "Интеграционные тесты cat не прошли!"
    exit 1
else
    echo "Интеграционные тесты cat прошли успешно!"
fi

cd ../grep || exit 1
touch grep_unit_test.log
make test | tee grep_unit_test.log
if grep -q "differ" grep_unit_test.log; then
    echo "Интеграционные тесты grep не прошли!"
    exit 1
else
    echo "Интеграционные тесты grep прошли успешно!"
fi
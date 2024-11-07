#!/bin/bash

cd components

for dd in abase amath main; do
    cd ${dd}
    make
    cd ..
done

cd ..

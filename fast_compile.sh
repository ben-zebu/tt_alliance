#!/bin/bash

cd components

for dd in abase main; do
    cd ${dd}
    make
    cd ..
done

cd ..

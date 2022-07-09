#!/bin/bash
for target_dir in $@
do
    make TARGET_DIR="${target_dir}"
    mv ${target_dir}/*.o ./object
    mv ${target_dir}/*.d ./object
done

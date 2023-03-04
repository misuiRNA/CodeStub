WORK_DIR=`pwd`

rm ./build/code_test

cmake -S . -B build

cmake --build build

./build/code_test


#/bin/bash

ROOT_PATH=$(pwd)
LIB=$ROOT_PATH/lib/lib

export LD_LIBRARY_PATH=$LIB; ./server 8080

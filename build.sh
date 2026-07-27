#!/bin/bash

code="$PWD"
opts=-g
     cd build > /dev/null
     g++ $opts $code/src/code/program.cpp -o tb
     cd $code > /dev/null

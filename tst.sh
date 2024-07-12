#!/bin/sh

gcc hashmap.c tst.c; if [ $? -ne 0 ]; then exit 1; fi

./a.out

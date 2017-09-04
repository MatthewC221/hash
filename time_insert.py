#!/usr/bin/python

# Test speed against time_insert.c

import sys
import random as rand
import time

H = {}

if (len(sys.argv) == 4):

    size = int(sys.argv[1])
    check_flag = int(sys.argv[2])
    delete_flag = int(sys.argv[3])
    
    if (size < 1):
        print "Use size > 1"
        sys.exit(1)
        
    H = {}
        
    for i in range(0, size):
        H[i] = 1
    
    if (check_flag):
        for i in range(0, size):
            assert H[i] == 1
    
    if (delete_flag):
        for i in range(0, size):
            H.pop(i, None)

else:
    print "Usage ./resize_test <size> <check_flag> <delete_flag> "



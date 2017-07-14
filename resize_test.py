#!/usr/bin/python

# Test speed against time_insert.c

import sys
import random as rand
import time

H = {}

if (len(sys.argv) == 4):
    size = int(sys.argv[1])
    
    if (size < 1):
        print "Use size > 1"
        sys.exit(1)
        
    keys = [None] * size
    H = {}
    rand_range = int(sys.argv[2])
    
    if (rand_range < 1):
        print "Use range > 1"
        sys.exit(1)
    
    rand.seed(time.time())
    for i in range(0, size):
        num = rand.randint(0, rand_range)
        H[num] = 1
        keys[i] = num
    
    check_flag = int(sys.argv[3])
    if (check_flag):
        for i in range(0, size):
            assert H[keys[i]] == 1
    
    print ("Total items = " + str(len(H)))

else:
    print "Usage ./resize_test <size> <random_range> <check_flag>"



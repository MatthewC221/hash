#!/usr/bin/python

# Test speed against time_insert.c

import sys
import random as rand

H = {}
keys = [None] * 100000

# Don't actually check it, but matches the C program

for i in range(0, 100000):
    num = rand.randint(0, 100000)
    H[num] = 1
    keys[i] = num




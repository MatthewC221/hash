#!/usr/bin/python

import sys
import matplotlib.pyplot as plt
import time
import os
import numpy as np

sizes = [1000, 5000, 10000, 50000, 100000, 500000,
750000, 1000000, 2000000, 3000000, 4000000, 5000000, 
6000000]

tot = len(sizes)
mhash_times = np.zeros(tot)
glib_times = np.zeros(tot)
dict_times = np.zeros(tot)
uthash_times = np.zeros(tot)

for i in xrange(len(sizes)):
	run_str = "./time_test " + str(sizes[i]) + " 8 2"
	cur = time.time()
	os.system(run_str)
	mhash_times[i] = time.time() - cur 

for i in xrange(len(sizes)):
	run_str = "./time_test_glib " + str(sizes[i])
	cur = time.time()
	os.system(run_str)
	glib_times[i] = time.time() - cur 

for i in xrange(len(sizes)):
	run_str = "./time_insert.py " + str(sizes[i]) + " 0 0"
	cur = time.time()
	os.system(run_str)
	dict_times[i] = time.time() - cur 

for i in xrange(len(sizes)):
	run_str = "./time_test_uthash " + str(sizes[i]) 
	cur = time.time()
	os.system(run_str)
	uthash_times[i] = time.time() - cur 

plt.plot(mhash_times, label="Mhash (C)")
plt.plot(glib_times, label="Glib (C)")
plt.plot(dict_times, label="Dict (Py)")
plt.plot(uthash_times, label="Uthash (C)")
plt.legend(loc='upper left')
plt.xlabel("Refer to plot_put.py")
plt.ylabel("Time (s)")
plt.title("Non-collision inserts")
fig = plt.gcf()
fig.canvas.set_window_title("Output of plot_put.py")
plt.show()
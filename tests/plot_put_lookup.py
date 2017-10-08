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
uthash_times = np.zeros(tot)

for i in xrange(len(sizes)):
	run_str = "./collision_speed_lookup 8 " + str(sizes[i]) + " 2"
	cur = time.time()
	os.system(run_str)
	mhash_times[i] = time.time() - cur 

for i in xrange(len(sizes)):
	run_str = "./glib_collision_speed_lookup " + str(sizes[i])
	cur = time.time()
	os.system(run_str)
	glib_times[i] = time.time() - cur 

for i in xrange(len(sizes)):
	run_str = "./uthash_collision_lookup " + str(sizes[i]) 
	cur = time.time()
	os.system(run_str)
	uthash_times[i] = time.time() - cur 

plt.plot(mhash_times, label="Mhash (C)")
plt.plot(glib_times, label="Glib (C)")
plt.plot(uthash_times, label="Uthash (C)")
plt.legend(loc='upper left')
plt.xlabel("Refer to plot_put.py")
plt.ylabel("Time (s)")
plt.title("Looking up on collisions")
fig = plt.gcf()
fig.canvas.set_window_title("Output of plot_put_col.py")
plt.show()
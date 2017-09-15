### My hash library for C
##### Winter Break 2017

Faster than Py dict and C glib's hash library
Compatible for beginners to advanced programmers to use

#### Main features
	- Variety of key val combinations (int/string key, string/int val)
	- Runs very fast for put, get and del
	- Good diagnostic tools

#### How to install
	- (TODO) script that will save into your libs: typically ~/usr/include/*

#### Quick tutorial
	- Hash * H = createHash();	// Defaults to int keys and int values and size 8
	- int key = 5;
	- int val = 4;
	- put(H, &key, &val);
	- assert(4 == get(H, &key));

#### DOCS
	- Will be created and uploaded soon

#### Main files
	hash.h, hash.c
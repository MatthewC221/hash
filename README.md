### My hash library for C
##### Winter Break 2017

Faster than Py dict and C glib's hash library
Compatible for beginners to advanced programmers to use

#### Main features
	Variety of key val combinations (int/string key, string/int val)
	Runs very fast for put, get and del
	Good diagnostic tools

#### How to install
	git clone git://github.com/MatthewC221/hash.git
	chmod 755 config.sh
	./config.sh 
	*Enter password*

This will put hash.h, hash.c in the typical libraries folder which is 
/usr/include/X

#### Compiling
	gcc -o X X.c -I/usr/include hash.c

#### Quick tutorial
	
	#include <hash.h>
	
	int main() {
		Hash * H = createHash();	// Defaults to int keys and int values and size 8
		int key = 5;
		int val = 4;
		put(H, &key, &val);
		assert(4 == get_INT_k_INT_v(H, &key));

		return 0;
	}

#### DOCS
	Will be created and uploaded soon

#### Main files
	hash.h, hash.c

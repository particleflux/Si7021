
example: examples/usage/usage.ino
	particle flash test examples/usage

doc: src/Si7021.h
	doxygen
	moxygen doc/xml/ -o doc/doc.md


clean: *.bin
	rm -rf *.bin

.PHONY: doc



all: src doc

clean:
	cd src; make clean
	cd doc; make clean

src:
	cd src; make

doc:
	git submodule init
	git submodule update
	cd doc; make

.PHONY: src doc clean
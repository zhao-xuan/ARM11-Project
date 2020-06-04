
all: src doc

clean:
	cd src; make clean
	cd doc; make clean

src:
	cd src; make

doc:
	git submodule update --init
	cd doc; make

.PHONY: src doc clean

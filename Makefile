
all: src doc

clean:
	cd src; make clean
	cd doc; make clean
	cd lib; make clean


src:
	cd src; make

doc:
	cd doc; make

lib:
	cd $(LIB_DIR); make

.PHONY: src doc clean lib

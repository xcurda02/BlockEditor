
all: blockeditor


blockeditor:
	cd src/BlockEditor && qmake BlockEditor.pro && make && make clean && mv BlockEditor ../../blockeditor

doxygen:
	doxygen doxygen-config/doxy_conf.txt

clean:
	rm -f blockeditor
	rm -rf doc/*
pack:
	zip -r xcurda02-xbulic02.zip doxygen-config examples src/BlockEditor Makefile README.md

run:	all
	./blockeditor

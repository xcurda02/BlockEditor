
all: mypc

doxygen:
	doxygen doxygen-config/doxy_conf.txt

merlin:
	cd src/BlockEditor && export LD_LIBRARY_PATH=/usr/local/share/Qt-5.5.1/5.5/gcc_64/lib:${LD_LIBRARY_PATH} && export QT_PLUGIN_PATH=/usr/local/share/Qt-5.5.1/5.5/gcc_64/plugins:${QT_PLUGIN_PATH} && export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/local/share/Qt-5.5.1/5.5/gcc_64/plugins && /usr/local/share/Qt-5.5.1/5.5/gcc_64/bin/qmake -makefile BlockEditor.pro && make && make clean && mv blockeditor ../../blockeditor

mypc:
	cd src/BlockEditor && qmake BlockEditor.pro && make && make clean && mv BlockEditor ../../blockeditor

clean:
	rm -f blockeditor
	rm -rf doc/*
pack:
	zip xcurda02-xbulic02.zip doxygen-config/* examples src/BlockEditor/* Makefile README.md

run:	all
	./blockeditor

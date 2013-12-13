all:	openWindow.icn openWindow.so
	icont openWindow.icn

openWindow.so:	openWindow.cpp
	g++ openWindow.cpp -fPIC -shared -lglfw -lGLEW -lGL -o openWindow.so

clean:
	rm openWindow.so openWindow

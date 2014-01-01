all:	iconGL.icn iconGL.so
	icont iconGL.icn

iconGL.so:	iconGL.cpp
	g++ iconGL.cpp common/loadshader.cpp common/loadbmp.cpp \
	common/object.cpp common/material.cpp \
	-fPIC -shared -lglfw -lGLEW -lGL -o iconGL.so

clean:
	rm iconGL.so iconGL

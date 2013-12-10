all:	hello.icn hello.so
	icont hello.icn

hello.so:	hello.cpp
	g++ -fPIC -shared hello.cpp -o hello.so

clean:
	rm hello.so hello

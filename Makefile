all:
	mkdir -p out
	gcc -Wall -fPIC -shared -Iincl -ldl -o out/shim.so src/lib.cpp
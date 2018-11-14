out/shim.so: src/lib/*
	mkdir -p out
	gcc -Wall -fPIC -shared -Iincl -ldl -o out/shim.so src/lib/*.cpp

out/test: lib/libsteam_api.so out/shim.so src/test/main.cpp
	g++ -Wall -Iincl -Llib -lsteam_api -pthread -o out/test src/test/*.cpp
	#LD_LIBRARY_PATH=lib LD_PRELOAD=out/shim.so out/test
main: src/main.cpp
	g++ -O3 -Ofast -march=native -mtune=native -flto -funroll-loops -fomit-frame-pointer -ffast-math -fno-rtti -std=c++20 src/main.cpp -o waldgeist
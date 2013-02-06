iss: display.cpp display.hpp genetic.cpp genetic.hpp issstate.cpp issstate.hpp main.cpp renderer.cpp renderer.hpp
	clang++ *.cpp -o iss -lGL -lGLU `sdl-config --cflags --libs` -std=c++0x

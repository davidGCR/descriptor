all:
	g++ -ggdb descriptor/main.cpp descriptor/sample_cube.h descriptor/utils.h descriptor/angles_magnitude.h descriptor/opticalFlowSet.h -o main -std=c++11 `pkg-config --cflags --libs opencv`
exec:
	g++ -ggdb practica.cpp descriptor/haralick.h -o main -std=c++11 `pkg-config --cflags --libs opencv`
	

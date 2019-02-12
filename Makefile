all:
	g++ -ggdb descriptor/main.cpp descriptor/sample_cube.h descriptor/utils.h -o main -std=c++11 `pkg-config --cflags --libs opencv`
exec:
	g++ -ggdb camera_calibration.cpp -o chess -std=c++11 `pkg-config --cflags --libs opencv`
	

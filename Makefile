CC = g++

all:
	g++ main.cpp -o rt

clean:
	rm rt
	rm result.*
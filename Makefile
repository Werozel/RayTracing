CC = g++

all:
	g++ main.cpp -o rt

check:
	test -r result.ppm && echo "Result generated" || echo "Result not generated!"

clean:
	rm rt
	rm result.*
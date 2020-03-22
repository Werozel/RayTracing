CC = g++

all:
	g++ main.cpp -o rt -O2 

check:
	test -r result.ppm && echo "Result generated" || (echo "Result not generated!" & exit 1)

clean:
	rm rt
	rm result.*
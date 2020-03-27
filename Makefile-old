CC = g++

all:
	g++ main.cpp vectors.cpp objects.cpp -o rt -O2

debug:
	g++ main.cpp vectors.cpp objects.cpp -o rt -g

run: all
	./rt

check:
	test -r result.ppm && echo "Result generated" || (echo "Result not generated!" & exit 1)

clean:
	rm rt -f
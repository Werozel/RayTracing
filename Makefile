CC = g++

all:
	g++ main.cpp vectors.cpp objects.cpp -o rt -g

check:
	test -r result.ppm && echo "Result generated" || (echo "Result not generated!" & exit 1)

clean:
	rm result.* rt -f
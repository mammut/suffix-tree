all:
	g++ -o suffixtree src/*.cpp -std=c++11

clean:
	rm -f suffixtree

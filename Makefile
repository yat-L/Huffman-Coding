all:
	clang++ -Wall -g static.cpp node.cpp -o A1Q1-Lui-H
	clang++ -Wall -g adaptive.cpp dnode.cpp -o A1Q2-Lui-H



clean:
	rm A1Q1-Lui-H

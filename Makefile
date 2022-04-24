all:
	clang++ -Wall static.cpp node.cpp -o huff-static
	clang++ -Wall adaptive.cpp dnode.cpp -o huff-adap



clean:
	rm huff-static
	rm huff-adap

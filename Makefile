all:
	clang++ -Wall static.cpp node.cpp -o run.exe && ./run.exe


clean:
	rm *.exe

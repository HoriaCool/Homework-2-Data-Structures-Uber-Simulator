
build:
	g++ --std=c++11 -Wall -Wextra main.cpp solver.cpp hash_functions.cpp -o tema2

.PHONY: clean

run:
	./main

clean:
	rm -f out/*/*
	rm -f tema2
	rm -f time.out

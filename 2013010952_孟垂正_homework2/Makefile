./bin/main:./bin/solver.o ./bin/main.o ./src/playarea.h
	g++ -O2 ./bin/solver.o ./bin/main.o -o $@
./bin/solver.o:./src/solver.cpp ./src/solver.h ./src/playarea.h
	g++ -c -O2 ./src/solver.cpp -o $@
./bin/main.o:./src/main.cpp ./src/solver.h ./src/playarea.h
	g++ -c -O2 ./src/main.cpp -o $@
clean:
	rm ./bin/solver.o ./bin/main.o

CC = g++
CPPFLAGS = -g -Wall -O2
LDFLAGS =

OBJ = TSPSolver.o main.o

%.o: %.cpp
		$(CC) $(CPPFLAGS) -c $^ -o $@

main: $(OBJ)
		$(CC) $(CPPFLAGS) $(OBJ) -o main 
		
clean:
		rm -rf $(OBJ) main

.PHONY: clean

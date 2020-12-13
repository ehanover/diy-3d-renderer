CXX = g++
CXXFLAGS = -Wall -g
INC = -I/usr/include/SFML
LIB = -L/usr/lib/x86_64-linux-gnu/ -lsfml-graphics -lsfml-window -lsfml-system
OBJECTS = main.o mymatrix.o myvector.o

a.out: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(INC) $(LIB)

# g++ -c main.cpp matrix.cpp -I/usr/include/SFML
# g++ -o main.o matrix.o -L/usr/lib/x86_64-linux-gnu/ -lsfml-graphics -lsfml-window -lsfml-system

clean:
	-rm -f *.o

main.o: main.cpp mymatrix.h myvector.h
mymatrix.o: mymatrix.cpp mymatrix.h
myvector.o: myvector.cpp myvector.h

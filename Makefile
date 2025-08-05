.PHONY: all clean

all: comp
#debug
CXX = g++ -fsanitize=undefined -g 

#CXX = g++ -Ofast

objects = const_global.o engine.o game.o utils.o main.o
headers = const_global.h engine.h game.h utils.h

comp: $(objects)
	$(CXX) -o comp $(objects) 

const_global.o: const_global.cpp const_global.h 
	$(CXX) -c const_global.cpp

engine.o: engine.cpp $(headers)
	$(CXX) -c engine.cpp

game.o: game.cpp const_global.h
	$(CXX) -c game.cpp

utils.o: utils.cpp const_global.h engine.h utils.h
	$(CXX) -c utils.cpp

main.o: main.cpp $(headers)
	$(CXX) -c main.cpp

clean:
	rm *.o comp
.PHONY: all clean

all: comp
#debug
CXX = g++ -fsanitize=undefined -g 

#CXX = g++ -Ofast

objects = src/const_global.o src/engine.o src/game.o src/utils.o src/main.o
headers = src/const_global.h src/engine.h src/game.h src/utils.h

comp: $(objects)
	$(CXX) -o comp $(objects) 

const_global.o: src/const_global.cpp src/const_global.h 
	$(CXX) -c src/const_global.cpp

engine.o: src/engine.cpp $(headers)
	$(CXX) -c src/engine.cpp

game.o: src/game.cpp src/const_global.h
	$(CXX) -c src/game.cpp

utils.o: src/utils.cpp src/const_global.h src/engine.h src/utils.h
	$(CXX) -c src/utils.cpp

main.o: src/main.cpp $(headers)
	$(CXX) -c src/main.cpp

clean:
	rm *.o comp
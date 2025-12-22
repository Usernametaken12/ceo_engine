.PHONY: all clean

all: comp
#debug
CXX = g++ -fsanitize=undefined -g 

#CXX = g++ -Ofast

objects = src/engine/const_global.o src/engine/engine.o src/engine/game.o src/engine/utils.o src/engine/main.o
headers = src/engine/const_global.h src/engine/engine.h src/engine/game.h src/engine/utils.h

comp: $(objects)
	$(CXX) -o comp $(objects) 

const_global.o: src/engine/const_global.cpp src/engine/const_global.h 
	$(CXX) -c src/engine/const_global.cpp

engine.o: src/engine/engine.cpp $(headers)
	$(CXX) -c src/engine/engine.cpp

game.o: src/engine/game.cpp src/engine/const_global.h
	$(CXX) -c src/engine/game.cpp

utils.o: src/engine/utils.cpp src/engine/const_global.h src/engine/engine.h src/engine/utils.h
	$(CXX) -c src/engine/utils.cpp

main.o: src/engine/main.cpp $(headers)
	$(CXX) -c src/engine/main.cpp

clean:
	rm *.o comp
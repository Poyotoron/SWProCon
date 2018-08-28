test: test.o SWProCon.o
	g++ -Wall -o test test.o SWProCon.o -std=c++11 -pthread
SWProCon.o: SWProCon.cpp
	g++ -Wall -c SWProCon.cpp -std=c++11 -pthread
test.o: test.cpp
	g++ -Wall -c test.cpp -std=c++11 -pthread
clean:
	rm -f *.o test

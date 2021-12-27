all: ps7

ps7: main.o
	g++ main.o -o ps7 -l boost_regex

main.o: main.cpp
	g++ -c main.cpp -Wall -Werror -pedantic

clean:
	rm *.o ps7

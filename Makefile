
all: osx-dvd-speed

clean:
	rm osx-dvd-speed

osx-dvd-speed: osx-dvd-speed.cpp
	g++ -Wall -o osx-dvd-speed osx-dvd-speed.cpp


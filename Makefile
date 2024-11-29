

Wifimodeling.o: Wifimodeling.cpp 
	g++ -c Wifimodeling.cpp
	g++ -g -o debug Wifimodeling.o
	g++ -O3 -o optimize Wifimodeling.o
# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Source files
SOURCES = Wifimodeling.cpp wifi4.cpp wifi5.cpp wifi6.cpp
HEADERS = wifi4.h wifi5.h wifi6.h wifi.h
OBJECTS_WIFI4 = wifi4.o
OBJECTS_WIFI5 = wifi5.o
OBJECTS_WIFI6 = wifi6.o

# Target executables and libraries
EXECUTABLE = wifi_modeling
LIB_WIFI4 = libwifi4.a
LIB_WIFI5 = libwifi5.a
LIB_WIFI6 = libwifi6.a

# Default target
all: $(EXECUTABLE)

# Build wifi_modeling executable
$(EXECUTABLE): Wifimodeling.o $(LIB_WIFI4) $(LIB_WIFI5) $(LIB_WIFI6)
	$(CXX) $(CXXFLAGS) Wifimodeling.o $(LIB_WIFI4) $(LIB_WIFI5) $(LIB_WIFI6) -o $@

# Build libwifi4.a
$(LIB_WIFI4): $(OBJECTS_WIFI4)
	ar rcs $@ $^

# Build libwifi5.a
$(LIB_WIFI5): $(OBJECTS_WIFI5)
	ar rcs $@ $^

# Build libwifi6.a
$(LIB_WIFI6): $(OBJECTS_WIFI6)
	ar rcs $@ $^

# Compile object files for wifi4
wifi4.o: wifi4.cpp wifi4.h wifi.h
	$(CXX) $(CXXFLAGS) -c wifi4.cpp

# Compile object files for wifi5
wifi5.o: wifi5.cpp wifi5.h wifi.h
	$(CXX) $(CXXFLAGS) -c wifi5.cpp

# Compile object files for wifi6
wifi6.o: wifi6.cpp wifi6.h wifi.h
	$(CXX) $(CXXFLAGS) -c wifi6.cpp

# Compile Wifimodeling.cpp
Wifimodeling.o: Wifimodeling.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c Wifimodeling.cpp



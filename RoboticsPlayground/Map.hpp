// header guards to avoid multiple inclusions of this file
#ifndef _MAP_H
#define _MAP_H


#include <iostream>
#include <fstream>
#include <filesystem>
#include <cmath>
#include <vector>
#include "stb_image.h"

// Class Map
// class containing all data and methods for reading/writing data to the map

class Map {
private:
	unsigned char* data;
	int width, height, channels;

public:
	// Constructor
	Map();					// construct the class
	void LoadMap(std::string);
	unsigned char* GetData();
	int GetHeight();
	int GetWidth();
	int GetChannels();
};

#endif

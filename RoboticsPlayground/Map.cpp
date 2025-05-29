#include "Map.hpp"


Map::Map() {
	
	

}

void Map::LoadMap(std::string file_path) {
	data = stbi_load(file_path.c_str(), &width, &height, &channels, 4);
	if (!data) {
		std::cerr << "Failed to load image!" << std::endl;
	}
	std::cout << "height = " << height << std::endl;
}

unsigned char* Map::GetData() {
	return data;
}

int Map::GetHeight() {
	return height;
}

int Map::GetWidth() {
	return width;
}

int Map::GetChannels() {
	return channels;
}
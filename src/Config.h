#pragma once
#include <string>
#include <cstdint>

struct ClientConfig {
	uint16_t	port;
	std::string ip;
};

extern ClientConfig g_Config;

void parseServerConfig(std::string path = "./properties.json");
void exportServerConfig(std::string path = "./properties.json");
#pragma once
#include <string>
#include <cstdint>

struct ClientConfig {
	uint16_t	port;
	std::string ip;
	std::string username;
};

extern ClientConfig g_Config;

void parseClientConfig(std::string path = "./clientproperties.json");
void exportClientConfig(std::string path = "./clientproperties.json");
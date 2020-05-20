#include "Config.h"
#include <Utilities/JSON.h>

using namespace Stardust;

void parseServerConfig(std::string path)
{
	Json::Value v = Utilities::JSON::openJSON(path);
	g_Config.port = v["port"].asInt();
	g_Config.ip = v["ip"].asString();
	g_Config.username = v["username"].asString();
}

void exportServerConfig(std::string path)
{
	Json::Value v;

	v["port"] = g_Config.port;
	v["ip"] = g_Config.ip;
	v["username"] = g_Config.username;

	std::ofstream f(path);
	f << v;
	f.close();
}

ClientConfig g_Config = { 25565, "127.0.0.1" };
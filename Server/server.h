#pragma once

#include <iostream>
#include <enet/enet.h>
#include <map>

class clientData {
private:
	int m_id;
	std::string m_username;

public:
	clientData(int id) : m_id(id) {}

	void SetUsername(std::string username) { m_username = username; }

	int GetID() { return m_id; }
	std::string GetUsername() { return m_username; }
};

class server {
public:
	static int start();
	static void MsgLoop(ENetHost* client);
	static void parseData(ENetHost* server, int id, char* data);

private:
	static inline std::map<int, clientData*> clientMap;
};
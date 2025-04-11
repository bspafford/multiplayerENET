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

class Client {
public:
	Client();
	void MsgLoop(ENetHost* client);
	void sendPacket(ENetPeer* peer, const char* data);
	void sendGameInfo();
	void disconnect();

	void parseData(char* data);

	int getClientId();

	void changeBlockAtIndex(long long index, int blockId);

private:
	int CLIENT_ID;
	std::map<int, clientData*> clientMap;

	ENetHost* client;
	ENetAddress address;
	ENetEvent eEvent;
	ENetPeer* peer;
};
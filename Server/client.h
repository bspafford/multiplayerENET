#pragma once

#include <iostream>
#include <enet/enet.h>

class client {
public:
	static void sendPacket(ENetPeer* peer, const char* data);
	static int start();
};
#define _CRT_SECURE_NO_WARNINGS
#include "server.h"

#include <string>

// 1 | is game data: location (for now)
// 2 | join data: 
// 3 | disconnect data: 

void server::MsgLoop(ENetHost* client) {
	ENetEvent event;
	while (enet_host_service(client, &event, 0) > 0) {
		switch (event.type) {
		case ENET_EVENT_TYPE_RECEIVE:
			/*
			printf("A packet of length %lu containing %s was received from %x:%u on channel %u.\n",
				event.packet->dataLength,
				event.packet->data,
				event.peer->address.host,
				event.peer->address.port,
				event.channelID);
			enet_packet_destroy(event.packet);
			*/
			//printf("Client Message: %s\n", event.packet->data);
			break;
		}
	}
}

void broadcastPacket(ENetHost* server, const char* data) {
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server, 0, packet);
}

void sendPacket(ENetPeer* peer, const char* data) {
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	// who, channel, packet
	enet_peer_send(peer, 0, packet);
}

int server::start() {
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occured while initializing ENet!\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	ENetAddress address;
	ENetHost* server;
	ENetEvent event;

	address.host = ENET_HOST_ANY;
	address.port = 7777;
								// max players, channels num, in/out going bandwidth
	//server = enet_host_create(&address, 8, 1, 0, 0);
	server = enet_host_create(&address, 8, 2, 0, 0);

	if (!server) {
		fprintf(stderr, "An error occured while trying to create an ENet server host!\n");
		return EXIT_FAILURE;
	}

	char ip[22];
	enet_address_get_host_ip(&server->address, ip, sizeof(ip));
	printf("server running: %s:%i\n", ip, address.port);

	int newPlayerId = 0;
	// GAME LOOP START
	while (true) {
		while (enet_host_service(server, &event, 1000) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:

				//printf("hello client has connected!\n");
				//sendPacket(event.peer, "Hello, user!");

				newPlayerId++;
				clientMap[newPlayerId] = new clientData(newPlayerId);
				event.peer->data = clientMap[newPlayerId];

				sendPacket(event.peer, ("4|" + std::to_string(newPlayerId)).c_str());

				//printf("A new client connected from %x:%u, ID: %u.\n", event.peer->address.host, event.peer->address.port, newPlayerId);
				broadcastPacket(server, ("2|" + std::to_string(newPlayerId)).c_str());
				//MsgLoop(server);

				char ip[22];
				enet_address_get_host_ip(&event.peer->address, ip, sizeof(ip));
				printf("Player Joined: %s\n", ip);


				break;
			case ENET_EVENT_TYPE_RECEIVE:
				/*
				printf("A packet of length %lu containing %s was received from %x:%u on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->address.host,
					event.peer->address.port,
					event.channelID);
				break;
				*/
				
				parseData(server, static_cast<clientData*>(event.peer->data)->GetID(), (char*)event.packet->data);

				//broadcastPacket(server, "server recieved stuff");

				// printf("Client Message: %s\n", event.packet->data);
			case ENET_EVENT_TYPE_DISCONNECT:

				//clientMap.erase(id);
				broadcastPacket(server, ("3|" + std::to_string(newPlayerId)).c_str());

				//printf("%x:%u disconnected.\n", event.peer->address.host, event.peer->address.port);
				// event.peer->data = NULL;
				break;
			}
		}
	}

	// GAME LOOP END

	enet_host_destroy(server);

	return EXIT_SUCCESS;
}

void server::parseData(ENetHost* server, int id, char* data) {

	int data_type;
	sscanf(data, "%d|", &data_type);

	switch (data_type) {
	case 1:
	{
		char msg[80];
		sscanf(data, "%*d|%[^\n]", &msg);

		// std::cout << "msg: " << msg << std::endl;

		char send_data[1024] = { '\0' };
		sprintf(send_data, "1|%d|%s", id, msg);
		broadcastPacket(server, send_data);
		break;
	}
	case 2:
	{
		int blockId;
		long long blockIndex;

		std::cout << "ogData: " << data << std::endl;

		sscanf(data, "2|%d|%lld", &blockId, &blockIndex);

		std::cout << "blockId: " << blockId << ", index: " << blockIndex << std::endl;


		char send_data[1024] = { '\0' };
		sprintf(send_data, "5|%d|%d|%lld", id, blockId, blockIndex);

		broadcastPacket(server, send_data);

		break;
	}
	}
}
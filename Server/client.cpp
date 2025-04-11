#include "client.h"

#include <string>

void MsgLoop(ENetHost* client) {
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
			printf("Client Message: %s\n", event.packet->data);

			break;
		}
	}
}

void client::sendPacket(ENetPeer* peer, const char* data) {
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
				// who, channel, packet
	enet_peer_send(peer, 0, packet);
}

int client::start() {
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occured while initializing ENet!\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	ENetHost* client;
	client = enet_host_create(NULL, 1, 1, 0, 0);

	if (!client) {
		fprintf(stderr, "An error occured while trying to create an ENet Client\n");
		return EXIT_FAILURE;
	}

	ENetAddress address;
	ENetEvent event;
	ENetPeer* peer;

	enet_address_set_host(&address, "127.0.0.1");
	address.port = 7777;

										// channel num, info sent on start
	peer = enet_host_connect(client, &address, 1, 0);
	if (!peer) {
		fprintf(stderr, "No available peers for initiating an ENet connection\n");
		return EXIT_FAILURE;
	}

								// timeout time
	if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
		puts("Connection to 127.0.0.1:7777 succeeded!\n");
	} else {
		enet_peer_reset(peer);
		puts("Connection to 127.0.0.1:7777 failed.\n");
		return EXIT_SUCCESS;
	}


	sendPacket(peer, "this_is_some_test_data");


	// GAME LOOP						// will sleep program until passes
	bool running = true;
	while (running) {
		std::string text;
		std::cout << "Message: ";
		std::getline(std::cin, text);
		sendPacket(peer, text.c_str());
		MsgLoop(client);

	}


	// GAME LOOP END
	enet_peer_disconnect(peer, 0);
	while (enet_host_service(client, &event, 3000) > 0) {
		switch (event.type) {
		case ENET_EVENT_TYPE_RECEIVE:
			enet_packet_destroy(event.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			puts("Disconnection succeeded");
			break;
		}
	}

	return EXIT_SUCCESS;
}
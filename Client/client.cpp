#include "client.h"

#include "main.h"
#include "character.h"

#include <string>

Client::Client() {
	if (enet_initialize() != 0) {
		fprintf(stderr, "An error occured while initializing ENet!\n");
		return;
	}
	atexit(enet_deinitialize);

	client = enet_host_create(NULL, 1, 1, 0, 0);

	if (!client) {
		fprintf(stderr, "An error occured while trying to create an ENet Client\n");
		return;
	}


	std::string ip = "0.0.0.0";// "127.0.0.1";
	std::string port = "7777";
	std::cout << "Enter IP: ";
	std::getline(std::cin, ip);
	std::cout << "Enter port: ";
	std::getline(std::cin, port);
	enet_address_set_host(&address, ip.c_str());
	address.port = std::stoi(port);

	// channel num, info sent on start
	peer = enet_host_connect(client, &address, 1, 0);
	if (!peer) {
		fprintf(stderr, "No available peers for initiating an ENet connection\n");
		return;
	}

	printf("Trying to connect to serveo.net:%d...\n", address.port);

	// timeout time
	if (enet_host_service(client, &eEvent, 5000) > 0 && eEvent.type == ENET_EVENT_TYPE_CONNECT) {
		puts(("Connection to " + ip + ":" + std::to_string(address.port) + " succeeded!\n").c_str());
	} else {
		enet_peer_reset(peer);
		puts(("Connection to " + ip + ":" + std::to_string(address.port) + " failed.\n").c_str());
		return;
	}
}

void Client::sendPacket(ENetPeer* peer, const char* data) {
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	// who, channel, packet
	enet_peer_send(peer, 0, packet);
}

void Client::sendGameInfo() {
	// send location data
	sendPacket(peer, ("1|" + std::to_string(Main::character->getLoc().x) + "," + std::to_string(Main::character->getLoc().y)).c_str());
	MsgLoop(client);
}

void Client::MsgLoop(ENetHost* client) {
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

			parseData((char*)event.packet->data);
			enet_packet_destroy(event.packet);
			break;
		}
	}
}

void Client::disconnect() {
	enet_peer_disconnect(peer, 0);
	while (enet_host_service(client, &eEvent, 3000) > 0) {
		switch (eEvent.type) {
		case ENET_EVENT_TYPE_RECEIVE:
			enet_packet_destroy(eEvent.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			puts("Disconnection succeeded");
			break;
		}
	}
}

void Client::parseData(char* data) {
	// Will store the data type (e.g. 1, 2, etc)
	int data_type;

	// Will store the id of the client that is sending the data
	int id;

	// Get first two numbers from the data (data_type and id) and but them in their respective variables
	sscanf(data, "%d|%d", &data_type, &id);

	// Switch between the different data_types
	switch (data_type) {
	case 1: // data is game data, position of char
		if (id != CLIENT_ID) { // if the player is you
			// Get message and Post it using the ClientData at id's username and the parsed msg.
			char msg[80];
			sscanf(data, "%*d|%*d|%[^|]", &msg);
			//chatScreen.PostMessage(client_map[id]->GetUsername().c_str(), msg);

			// std::cout << "message: " << msg << std::endl;

			std::stringstream ss(msg);
			std::vector<std::string> result;

			while (ss.good()) {
				std::string substr;
				getline(ss, substr, ',');
				result.push_back(substr);
			}

			if (Main::otherCharacters[id]) {
				Main::otherCharacters[id]->setLoc({ std::stof(result[0]), std::stof(result[1]) });
			} else { // loads in character if null, might happen if character wasn't first to join
				Main::otherCharacters[id] = new Acharacter(15.f);
				Main::otherCharacters[id]->setLoc({ std::stof(result[0]), std::stof(result[1]) });
			}

		}
		break;
	case 2: // data is if a player joined
		if (id != CLIENT_ID) { // if the player is you
			// Create a new ClientData with username and add it to map at id.
			char username[80];
			sscanf(data, "%*d|%*d|%[^|]", &username);

			clientMap[id] = new clientData(id);
			clientMap[id]->SetUsername(username);

			std::cout << "player has joined with ID of " << id << std::endl;

			Main::playerJoined(id);
		}
		break;
	case 3: // data is disconnect data
		clientMap.erase(id);
		Main::playerDisconnected(id);

		break;
	case 4: // data is our id
		CLIENT_ID = id; // Set our id to the received id.
		// printf("setting id: %u\n", CLIENT_ID);

		//std::cout << "how many others: " << Main::otherCharacters.size() << std::endl;

		//Main::addPreviousPlayers(clientMap);

		break;
	case 5: // break / place block
		if (id != CLIENT_ID) {
			int blockId;
			long long index;

			std::cout << "data: " << data << std::endl;

			sscanf(data, "5|%*d|%d|%lld", &blockId, &index);

			//std::cout << "id: " << id << ", blockId: " << blockId << ", index: " << index << std::endl;
			Main::blocks[index].textureIndex = blockId;
			Main::updateChunk();
		}
		break;
	}
}

int Client::getClientId() {
	return CLIENT_ID;
}

void Client::changeBlockAtIndex(long long index, int blockId) {
	std::cout << "changeBlockAtIndex: blockId: " << std::to_string(blockId) << ", index: " << std::to_string(index) << std::endl;
	sendPacket(peer, ("2|" + std::to_string(blockId) + "|" + std::to_string(index)).c_str());
}
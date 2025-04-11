// multiplayerENET.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "client.h"
#include "server.h"

int main(int argc, char* argv[]) {

	server::start();

	return 0;
	if (strcmp(argv[1], "--server") == 0)
		return server::start();
	else if (strcmp(argv[1], "--client") == 0)
		return client::start();
}
#include <iostream>
#include <ws2tcpip.h>

#pragma comment (lib, "ws2_32.lib") 
#pragma warning(disable:4996) 

int main() {
	// Initialization
	WSADATA wsData;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &wsData);

	if (wsOk != 0) {
		std::cerr << "ERROR!" << std::endl;
	}

	// Creating
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);

	if (sListen == INVALID_SOCKET) {
		std::cerr << "ERROR!" << std::endl;
	}

	// Binging
	sockaddr_in address;
	address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	address.sin_family = AF_INET;
	address.sin_port = htons(8888);

	bind(sListen, (sockaddr*)&address, sizeof(address));

	// Listening
	listen(sListen, SOMAXCONN);

	// Waiting
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET newConnection;
	newConnection = accept(sListen, (sockaddr*)&address, &clientSize);

	if (newConnection == INVALID_SOCKET) {
		std::cerr << "ERROR!" << std::endl;
	}
	else {
		std::cerr << "Cliend connected!" << std::endl;
	}

	system("pause");

	return 0;
}
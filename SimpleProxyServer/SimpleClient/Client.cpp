#include <iostream>
#include <ws2tcpip.h>
#include <string>

#define SERVER_IP "127.0.0.1"
#define ERROR_S "ERROR!"
#define DEFAULT_PORT 1606

#pragma comment (lib, "ws2_32.lib") 
#pragma warning(disable:4996) 

SOCKET connection;

void clientHandler() {
	char msg[256];
	while (true) {
		std::cin.getline(msg, sizeof(msg));
		send(connection, msg, sizeof(msg), 0);
	}
}

int main() {
	// Initialization
	WSADATA wsData;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &wsData);

	if (wsOk != 0) {
		std::cerr << ERROR_S << std::endl;
		return 1;
	}

	// Binging
	sockaddr_in address;
	address.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	address.sin_family = AF_INET;
	address.sin_port = htons(DEFAULT_PORT);

	// Connecting
	connection = socket(AF_INET, SOCK_STREAM, 0);

	if (connect(connection, (sockaddr*)&address, sizeof(address)) == INVALID_SOCKET) {
		std::cerr << ERROR_S << std::endl;
		closesocket(connection);
		WSACleanup();
		return 1;
	} 
	else {
		std::cout << "Client: Connected!" << std::endl;
	}

	while (true) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)clientHandler, 0, 0, 0);
	}

	closesocket(connection);
	WSACleanup();
	system("pause");

	return 0;
}
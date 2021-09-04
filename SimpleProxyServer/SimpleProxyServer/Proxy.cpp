#include <iostream>
#include <ws2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib") 
#pragma warning(disable:4996) 

int counter = 0;
SOCKET connections[1000];
SOCKET newConnection;
SOCKET proxyConnection;

void ClientHandler(int index) {
	int msg_size;
	char msg[256];
	while (true) {
		//recv(connections[index], (char*)&msg_size, sizeof(int), 0);
		//char* msg = new char[msg_size + 1];
		//msg[msg_size] = '/0';
		//recv(connections[index], msg, msg_size, 0);
		recv(connections[index], msg, sizeof(msg), 0);
		std::cout << "Client: " << msg << std::endl;

		//delete[] msg;
	}
}

void ProxyHandler() {
	//std::string msg;
	char msg[256];
	while (true) {
		//std::getline(std::cin, msg);
		//int msg_size = msg.size();
		//send(connection, (char*)&msg_size, sizeof(int), 0);
		std::cin.getline(msg, sizeof(msg));
		send(proxyConnection, msg, sizeof(msg), 0);
	}
}

int main() {
	// Initialization
	WSADATA wsData;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &wsData);

	if (wsOk != 0) {
		std::cerr << "ERROR!" << std::endl;
		return 1;
	}

	// Creating
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);

	if (sListen == INVALID_SOCKET) {
		std::cerr << "ERROR!" << std::endl;
		closesocket(sListen);
		WSACleanup();
		return 1;
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

	proxyConnection = socket(AF_INET, SOCK_STREAM, 0);

	if (connect(proxyConnection, (sockaddr*)&address, sizeof(address)) == INVALID_SOCKET) {
		std::cerr << "Error!" << std::endl;
		closesocket(proxyConnection);
		WSACleanup();
		return 1;
	}
	else {
		std::cout << "Proxy: Connected!" << std::endl;
		/*while (true) {
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ProxyHandler, 0, 0, 0);
		}*/
	}

	for (int i = 0; i < 1000; ++i) {
		newConnection = accept(sListen, (sockaddr*)&address, &clientSize);

		if (newConnection == INVALID_SOCKET) {
			std::cerr << "ERROR!" << std::endl;
			closesocket(newConnection);
			WSACleanup();
			return 1;
		}
		else {
			std::cout << "Proxy: Client connected!" << std::endl;
			connections[i] = newConnection;
			counter++;
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), 0, 0);
		}
	}

	closesocket(newConnection);
	WSACleanup();
	system("pause");

	return 0;
}
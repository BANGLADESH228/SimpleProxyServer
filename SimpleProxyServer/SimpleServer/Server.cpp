#include <iostream>
#include <ws2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib") 
#pragma warning(disable:4996) 

SOCKET newConnection;

void ProxyHandler() {
	char msg[256];
	while (true) {
		recv(newConnection, msg, sizeof(msg), 0);
		std::cout << msg << std::endl;
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
	SOCKET sListenProxy = socket(AF_INET, SOCK_STREAM, 0);

	if (sListenProxy == INVALID_SOCKET) {
		std::cerr << "ERROR!" << std::endl;
		closesocket(sListenProxy);
		WSACleanup();
		return 1;
	}

	// Binging
	sockaddr_in address;
	address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	address.sin_family = AF_INET;
	address.sin_port = htons(8888);

	bind(sListenProxy, (sockaddr*)&address, sizeof(address));

	// Listening
	listen(sListenProxy, SOMAXCONN);

	// Waiting
	sockaddr_in proxy;
	int ProxySize = sizeof(proxy);

	newConnection = accept(sListenProxy, (sockaddr*)&address, &ProxySize);

	if (newConnection == INVALID_SOCKET) {
		std::cerr << "ERROR!" << std::endl;
		/*closesocket(newConnection);
		WSACleanup();
		return 1;*/
	}
	else {
		std::cout << "Server: Proxy connected!" << std::endl;
		ProxyHandler();
	}

	closesocket(newConnection);
	WSACleanup();
	system("pause");

	return 0;
}
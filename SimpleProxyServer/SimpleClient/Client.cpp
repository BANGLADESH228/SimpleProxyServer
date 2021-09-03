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
		std::cerr << "Error!" << std::endl;
	}

	// Binging
	sockaddr_in address;
	address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//InetPton(AF_INET, _T("192.168.1.1"), &address.sin_addr.s_addr);

	address.sin_family = AF_INET;
	address.sin_port = htons(8888);

	// Connecting
	SOCKET connection;
	connection = socket(AF_INET, SOCK_STREAM, 0);

	if (connect(connection, (sockaddr*)&address, sizeof(address)) == INVALID_SOCKET) {
		std::cerr << "Error!" << std::endl;
		return 1;
	}
	else {
		std::cerr << "Connected!" << std::endl;
	}

	system("pause");

	return 0;
}
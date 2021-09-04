#include <iostream>
#include <ws2tcpip.h>
#include <string>
#include <fstream>

#define SERVER_IP "127.0.0.1"
#define ERROR_S "ERROR!"
#define DEFAULT_PORT 1606

#pragma comment (lib, "ws2_32.lib") 
#pragma warning(disable:4996) 

int counter = 0;
SOCKET connections[1000];
SOCKET newConnection;
SOCKET proxyConnection;

void logger(char msg[256]) {
	std::string path = "logger.txt";
	std::fstream fs;
	fs.open(path, std::fstream::in | std::fstream::out | std::fstream::app);
	fs << msg << "\n";
	fs.close();
}

void clientHandler(int index) {
	int msg_size;
	char msg[256];
	while (true) {
		recv(connections[index], msg, sizeof(msg), 0);
		std::cout << "Client: " << msg << std::endl;
		logger(msg);
	}
	send(proxyConnection, msg, sizeof(msg), 0);
}

/*
Написать функцию, которая будет отправлять POST запрос на сайте (curl)
https ://ru.rakko.tools/tools/36/. Где будет происходить проверка сообщения 
на синтаксис SQL-запроса. Полученный ответ будет сравниваться в программе.
Если результат true, то SQL-запрос будет записан в logger файл. В ином
варианте будет проигнорирован. Результат также отправлять на конечный сервер.
*/ 

void checkSQL(){}

int main() {
	// Initialization
	WSADATA wsData;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &wsData);

	if (wsOk != 0) {
		std::cerr << ERROR_S << std::endl;
		return 1;
	}

	// Creating
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);

	if (sListen == INVALID_SOCKET) {
		std::cerr << ERROR_S << std::endl;
		closesocket(sListen);
		WSACleanup();
		return 1;
	}

	// Binging
	sockaddr_in address;
	address.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	address.sin_family = AF_INET;
	address.sin_port = htons(DEFAULT_PORT);

	bind(sListen, (sockaddr*)&address, sizeof(address));

	// Listening
	listen(sListen, SOMAXCONN);

	// Waiting
	sockaddr_in client;
	int clientSize = sizeof(client);

	proxyConnection = socket(AF_INET, SOCK_STREAM, 0);

	if (connect(proxyConnection, (sockaddr*)&address, sizeof(address)) == INVALID_SOCKET) {
		std::cerr << ERROR_S << std::endl;
		closesocket(proxyConnection);
		WSACleanup();
		return 1;
	}
	else {
		std::cout << "Proxy: Connected!" << std::endl;
	}

	for (int i = 0; i < 1000; ++i) {
		newConnection = accept(sListen, (sockaddr*)&address, &clientSize);

		if (newConnection == INVALID_SOCKET) {
			std::cerr << ERROR_S << std::endl;
			closesocket(newConnection);
			WSACleanup();
			return 1;
		}
		else {
			std::cout << "Proxy: Client connected!" << std::endl;
			connections[i] = newConnection;
			counter++;
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)clientHandler, (LPVOID)(i), 0, 0);
		}
	}

	closesocket(newConnection);
	WSACleanup();
	system("pause");

	return 0;
}
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

const int PORT = 12345;
const int BUFFER_SIZE = 1024;

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "UDP server is listening on port " << PORT << std::endl;

    char buffer[BUFFER_SIZE];
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    while (true) {
        int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "recvfrom failed." << std::endl;
            continue;
        }

        // Отправляем данные обратно клиенту
        sendto(serverSocket, buffer, bytesReceived, 0, reinterpret_cast<sockaddr*>(&clientAddr), clientAddrSize);
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

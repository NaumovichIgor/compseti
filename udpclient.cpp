#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

const int PORT = 12345;
const char* SERVER_IP = "127.0.0.1"; // Замените на IP сервера

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    std::string message;
    std::cout << "Enter message: ";
    std::getline(std::cin, message);

    sendto(clientSocket, message.c_str(), message.size() + 1, 0, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));

    char buffer[1024];
    int serverAddrSize = sizeof(serverAddr);
    int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&serverAddr), &serverAddrSize);
    if (bytesReceived > 0) {
        std::cout << "Server response: " << buffer << std::endl;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}

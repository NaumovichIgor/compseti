#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

const int PORT = 12345;
const int BUFFER_SIZE = 1024;

struct StudentData {
    char name[100];
    double height;
    double weight;
};

void handleClient(SOCKET clientSocket) {
    StudentData data;
    char buffer[BUFFER_SIZE];

    while (true) {
        // Получаем данные от клиента
        int bytesReceived = recv(clientSocket, reinterpret_cast<char*>(&data), sizeof(data), 0);
        if (bytesReceived <= 0) {
            std::cerr << "Client disconnected or error occurred." << std::endl;
            break;
        }

        // Обрабатываем данные
        std::string result;
        double bmi = data.weight / ((data.height / 100) * (data.height / 100));
        if (bmi < 18.5) result = "Нехватка веса";
        else if (bmi >= 18.5 && bmi <= 24.9) result = "Нормально";
        else result = "Превышение веса";

        // Отправляем результат клиенту
        send(clientSocket, result.c_str(), result.size() + 1, 0);
    }

    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
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

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed." << std::endl;
            continue;
        }

        std::thread(handleClient, clientSocket).detach();
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

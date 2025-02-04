#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

const std::string con_file = "con";

struct ClientRequest {
    std::string name;
    double height;
    double weight;
};

int main() {
    std::string client_name;
    std::cout << "Enter your name: ";
    std::cin >> client_name;

    std::string client_file = client_name + ".txt";
    std::ofstream(client_file).close(); // Создаем файл для клиента

    while (true) {
        ClientRequest request;
        request.name = client_name;
        std::cout << "Enter height (in cm): ";
        std::cin >> request.height;
        std::cout << "Enter weight (in kg): ";
        std::cin >> request.weight;

        // Отправляем запрос серверу
        std::ofstream out(con_file, std::ios::app);
        if (out.is_open()) {
            out << request.name << " " << request.height << " " << request.weight << std::endl;
        }

        // Ждем ответа от сервера
        std::string response;
        while (true) {
            std::ifstream in(client_file);
            if (in.is_open()) {
                std::string line;
                while (std::getline(in, line)) {
                    response = line;
                }
                if (!response.empty()) {
                    std::cout << "Response from server: " << response << std::endl;
                    // Очищаем файл после получения ответа
                    std::ofstream(client_file, std::ios::trunc).close();
                    break;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    return 0;
}

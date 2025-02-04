#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

const std::string f1 = "f1.txt";
const std::string f2 = "f2.txt";

std::string processRequest(const std::string& request) {
    // Пример обработки запроса
    return "Processed: " + request;
}

int main() {
    size_t last_size = 0;

    while (true) {
        std::ifstream in(f1, std::ios::ate);
        size_t current_size = in.tellg();
        if (current_size > last_size) {
            last_size = current_size;
            in.seekg(0, std::ios::beg);
            std::string request;
            while (std::getline(in, request)) {
                // Пропускаем уже обработанные запросы
                if (in.tellg() == -1) {
                    std::string response = processRequest(request);
                    std::ofstream out(f2, std::ios::app);
                    if (out.is_open()) {
                        out << response << std::endl;
                    }
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}

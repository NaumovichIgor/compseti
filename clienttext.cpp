#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

const std::string f1 = "f1.txt";
const std::string f2 = "f2.txt";

void sendRequest(const std::string& request) {
    std::ofstream out(f1, std::ios::app);
    if (out.is_open()) {
        out << request << std::endl;
    }
}

std::string receiveResponse() {
    std::ifstream in(f2);
    std::string response;
    std::string line;
    while (std::getline(in, line)) {
        response = line;
    }
    return response;
}

int main() {
    std::string request;
    std::string response;
    size_t last_size = 0;

    while (true) {
        std::cout << "Enter your request: ";
        std::getline(std::cin, request);
        sendRequest(request);

        while (true) {
            std::ifstream in(f2, std::ios::ate);
            size_t current_size = in.tellg();
            if (current_size > last_size) {
                last_size = current_size;
                response = receiveResponse();
                std::cout << "Response from server: " << response << std::endl;
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    return 0;
}

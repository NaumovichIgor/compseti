#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

struct Request {
    int id;
    char data[256];
};

struct Response {
    int id;
    char data[256];
};

const std::string f1 = "f1.bin";
const std::string f2 = "f2.bin";

void sendRequest(const Request& request) {
    std::ofstream out(f1, std::ios::app | std::ios::binary);
    if (out.is_open()) {
        out.write(reinterpret_cast<const char*>(&request), sizeof(Request));
    }
}

Response receiveResponse() {
    std::ifstream in(f2, std::ios::binary);
    Response response;
    in.seekg(-sizeof(Response), std::ios::end);
    in.read(reinterpret_cast<char*>(&response), sizeof(Response));
    return response;
}

int main() {
    Request request;
    Response response;
    size_t last_size = 0;

    while (true) {
        std::cout << "Enter request ID: ";
        std::cin >> request.id;
        std::cin.ignore(); // Игнорируем оставшийся символ новой строки
        std::cout << "Enter request data: ";
        std::cin.getline(request.data, 256);

        sendRequest(request);

        while (true) {
            std::ifstream in(f2, std::ios::ate | std::ios::binary);
            size_t current_size = in.tellg();
            if (current_size > last_size) {
                last_size = current_size;
                response = receiveResponse();
                std::cout << "Response from server: ID=" << response.id << ", Data=" << response.data << std::endl;
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    return 0;
}

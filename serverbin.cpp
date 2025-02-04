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

Response processRequest(const Request& request) {
    Response response;
    response.id = request.id;
    std::string processed_data = "Processed: ";
    processed_data += request.data;
    std::strcpy(response.data, processed_data.c_str());
    return response;
}

int main() {
    size_t last_size = 0;

    while (true) {
        std::ifstream in(f1, std::ios::ate | std::ios::binary);
        size_t current_size = in.tellg();
        if (current_size > last_size) {
            last_size = current_size;
            in.seekg(0, std::ios::beg);
            Request request;
            while (in.read(reinterpret_cast<char*>(&request), sizeof(Request))) {
                // Пропускаем уже обработанные запросы
                if (in.tellg() == -1) {
                    Response response = processRequest(request);
                    std::ofstream out(f2, std::ios::app | std::ios::binary);
                    if (out.is_open()) {
                        out.write(reinterpret_cast<const char*>(&response), sizeof(Response));
                    }
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}

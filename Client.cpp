#include "wSocket.h"
#include <iostream>
#include <string>

class Client : public wSocket {
    std::string serverIp;
    int port;
public:
    Client(const std::string& ip, int p) : serverIp(ip), port(p) {}

    void connectToServer() {
        create();

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(serverIp.c_str());

        if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
            throw std::runtime_error("Не удалось подключиться к серверу");
        }
    }

    void sendMessage(const std::string& msg) {
        if (send(sock, msg.c_str(), msg.size(), 0) == SOCKET_ERROR) {
            throw std::runtime_error("Ошибка отправки сообщения");
        }
    }

    std::string receiveResponse() {
        char buffer[1024];
        int bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes <= 0) {
            throw std::runtime_error("Ошибка получения ответа");
        }
        return std::string(buffer, bytes);
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Использование: " << argv[0] << " IP_сервера порт" << std::endl;
        return 1;
    }

    try {
        Client client(argv[1], std::stoi(argv[2]));
        client.connectToServer();

        std::string input;
        while (true) {
            std::cout << "Введите сообщение (или 'выход'): ";
            std::getline(std::cin, input);

            if (input == "выход") break;

            client.sendMessage(input);
            std::string response = client.receiveResponse();
            std::cout << "Ответ сервера: " << response << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

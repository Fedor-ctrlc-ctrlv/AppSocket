#include "wSocket.h"
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>

class ProcessingServer : public wSocket {
    int clientPort;
    std::string displayIp;
    int displayPort;
public:
    ProcessingServer(int cp, const std::string& dip, int dp)
        : clientPort(cp), displayIp(dip), displayPort(dp) {}

    void start() {
        // Подключаемся к серверу отображения
        SOCKET displaySock = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in displayAddr;
        displayAddr.sin_family = AF_INET;
        displayAddr.sin_port = htons(displayPort);
        displayAddr.sin_addr.s_addr = inet_addr(displayIp.c_str());

        if (connect(displaySock, (sockaddr*)&displayAddr, sizeof(displayAddr)) == SOCKET_ERROR) {
            throw std::runtime_error("Не удалось подключиться к серверу отображения");
        }

        // Настраиваем сервер для клиентов
        create();

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(clientPort);
        addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
            throw std::runtime_error("Ошибка привязки сокета");
        }

        if (listen(sock, 1) == SOCKET_ERROR) {
            throw std::runtime_error("Ошибка прослушивания");
        }

        std::cout << "Сервер обработки запущен на порту " << clientPort << std::endl;

        while (true) {
            sockaddr_in clientAddr;
            int clientAddrSize = sizeof(clientAddr);
            SOCKET clientSock = accept(sock, (sockaddr*)&clientAddr, &clientAddrSize);

            if (clientSock == INVALID_SOCKET) {
                std::cerr << "Ошибка принятия соединения" << std::endl;
                continue;
            }

            try {
                char buffer[1024];
                int bytes = recv(clientSock, buffer, sizeof(buffer), 0);
                if (bytes <= 0) throw std::runtime_error("Ошибка получения данных");

                std::string data(buffer, bytes);
                std::cout << "Получены данные: " << data << std::endl;

                // Отправляем подтверждение
                send(clientSock, "OK", 2, 0);

                // Обрабатываем данные
                std::string result = processData(data);
                std::cout << "Результат обработки: " << result << std::endl;

                // Отправляем на сервер отображения
                send(displaySock, result.c_str(), result.size(), 0);
            }
            catch (const std::exception& e) {
                std::cerr << "Ошибка: " << e.what() << std::endl;
            }

            closesocket(clientSock);
        }
    }

private:
    std::string processData(const std::string& data) {
        std::istringstream iss(data);
        std::unordered_set<std::string> words;
        std::string word, result;

        while (iss >> word) {
            if (words.find(word) == words.end()) {
                words.insert(word);
                if (!result.empty()) result += " ";
                result += word;
            }
        }

        return result;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Использование: " << argv[0] << " порт_клиентов IP_дисплея порт_дисплея" << std::endl;
        return 1;
    }

    try {
        ProcessingServer server(std::stoi(argv[1]), argv[2], std::stoi(argv[3]));
        server.start();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

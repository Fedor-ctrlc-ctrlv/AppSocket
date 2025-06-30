#include "wSocket.h"
#include <iostream>

class DisplayServer : public wSocket {
    int port;
public:
    DisplayServer(int p) : port(p) {}

    void start() {
        create();

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
            throw std::runtime_error("������ �������� ������");
        }

        if (listen(sock, 1) == SOCKET_ERROR) {
            throw std::runtime_error("������ �������������");
        }

        std::cout << "������ ����������� ������� �� ����� " << port << std::endl;

        while (true) {
            sockaddr_in clientAddr;
            int clientAddrSize = sizeof(clientAddr);
            SOCKET clientSock = accept(sock, (sockaddr*)&clientAddr, &clientAddrSize);

            if (clientSock == INVALID_SOCKET) {
                std::cerr << "������ �������� ����������" << std::endl;
                continue;
            }

            try {
                char buffer[1024];
                while (true) {
                    int bytes = recv(clientSock, buffer, sizeof(buffer), 0);
                    if (bytes <= 0) break;

                    std::string data(buffer, bytes);
                    std::cout << "������ ��� �����������: " << data << std::endl;
                }
            }
            catch (...) {
                std::cerr << "������ ��������� ������" << std::endl;
            }

            closesocket(clientSock);
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "�������������: " << argv[0] << " ����" << std::endl;
        return 1;
    }

    try {
        DisplayServer server(std::stoi(argv[1]));
        server.start();
    }
    catch (const std::exception& e) {
        std::cerr << "������: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

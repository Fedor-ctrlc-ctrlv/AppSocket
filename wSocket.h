#pragma once
#include <winsock2.h>
#include <iostream>
#include <string>

class wSocket {
protected:
    SOCKET sock;
public:
    wSocket() : sock(INVALID_SOCKET) {
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            throw std::runtime_error("Ошибка инициализации сокетов");
        }
    }

    ~wSocket() {
        if (sock != INVALID_SOCKET) {
            closesocket(sock);
        }
        WSACleanup();
    }

    void create() {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) {
            throw std::runtime_error("Не удалось создать сокет");
        }
    }

    void close() {
        if (sock != INVALID_SOCKET) {
            closesocket(sock);
            sock = INVALID_SOCKET;
        }
    }
};

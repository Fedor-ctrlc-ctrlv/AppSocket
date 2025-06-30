cmake_minimum_required(VERSION 3.10)
project(AppSocket)

# Настройка для Windows (Winsock)
if(WIN32)
    add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lws2_32")
endif()

# Клиент
add_executable(Client Client.cpp wSocket.h)

# Сервер обработки
add_executable(ProcessingServer ProcessingServer.cpp wSocket.h)

# Сервер отображения
add_executable(DisplayServer DisplayServer.cpp wSocket.h)

# Линковка Winsock на Windows
if(WIN32)
    target_link_libraries(Client ws2_32)
    target_link_libraries(ProcessingServer ws2_32)
    target_link_libraries(DisplayServer ws2_32)
endif()
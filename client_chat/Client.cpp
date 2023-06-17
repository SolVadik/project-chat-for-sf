// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "Client.h"

int create_socket() {
#ifdef _WIN64
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return -1;
    }
#endif

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    return client_socket;
}

void connect_to_server(int client_socket, const std::string& server_ip, int server_port) {
    sockaddr_in server_address;
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr);

    if (connect(client_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Failed to connect to server" << std::endl;
        return;
    }
}

void send_data(int client_socket, const std::unique_ptr<char[]>& buffer, int buffer_size) {
    send(client_socket, buffer.get(), buffer_size, 0);
}

void send_string(int client_socket, const std::string& message) {
    auto buffer = std::make_unique<char[]>(message.size());
    memcpy(buffer.get(), message.c_str(), message.size());
    send_data(client_socket, std::move(buffer), message.size());
}

int get_data_size(int socket) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int result = recv(socket, buffer, sizeof(buffer), MSG_PEEK);
    if (result > 0) {
        return result;
    }
    else if (result == 0) {
        std::cout << "Connection closed" << std::endl;
        return -1;
    }
    else {
        perror("recv");
        exit(EXIT_FAILURE);
    }
}

std::unique_ptr<char[]> receive_data(int client_socket, int buffer_size) {
    auto data = std::make_unique<char[]>(buffer_size);
    int result = recv(client_socket, data.get(), buffer_size, 0);
    if (result > 0) {
        return data;
    }
    else if (result == 0) {
        std::cout << "Connection closed" << std::endl;
        return nullptr;
    }
    else {
        perror("recv");
        exit(EXIT_FAILURE);
    }
}

void close_socket(int client_socket) {
#ifdef _WIN64
    closesocket(client_socket);
#else
    close(client_socket);
#endif

#ifdef _WIN64
    WSACleanup();
#endif
}

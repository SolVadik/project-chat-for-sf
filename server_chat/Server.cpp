// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "Server.h"



int create_socket() {
#ifdef _WIN64
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: \n" << iResult << std::endl;
        return -1;
    }
#endif

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create socket\n";
        return -1;
    }

    sockaddr_in server_address;
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(7777);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Failed to bind socket to address\n";
        return -1;
    }

    if (listen(server_socket, 1) == -1) {
        std::cerr << "Error while trying to listen to socket\n";
        return -1;
    }

    return server_socket;
}

int accept_client(int server_socket) {
    sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);
    int client_socket = accept(server_socket, (sockaddr*)&client_address, &client_length);
    if (client_socket == -1) {
        std::cerr << "Error while trying to accept client\n";
        return -1;
    }
    return client_socket;
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



void send_data(int client_socket, const std::unique_ptr<char[]>& buffer, int buffer_size) {
    send(client_socket, buffer.get(), buffer_size, 0);
}

void send_string(int client_socket, const std::string& message) {
    auto buffer = std::make_unique<char[]>(message.size());
    memcpy(buffer.get(), message.c_str(), message.size());
    send_data(client_socket, std::move(buffer), message.size());
}

void send_package(int client_socket, const Package& package) {
    std::string message = package.message;
    char flag = package.flag;
    auto buffer = std::make_unique<char[]>(message.size() + sizeof(flag));
    memcpy(buffer.get(), message.c_str(), message.size());
    memcpy(buffer.get() + message.size(), &flag, sizeof(flag));
    send_data(client_socket, std::move(buffer), message.size() + sizeof(flag));
}

//std::unique_ptr<Package> receive_package(int client_socket, int buffer_size) {
//    auto data = std::make_unique<char[]>(buffer_size);
//    int result = recv(client_socket, data.get(), buffer_size, 0);
//    if (result > 0) {
//        auto package = std::make_unique<Package>();
//        package->message = std::string(data.get(), result - sizeof(package->flag));
//        memcpy(&package->flag, data.get() + result - sizeof(package->flag), sizeof(package->flag));
//        return package;
//    }
//    else if (result == 0) {
//        std::cout << "Connection closed" << std::endl;
//        return nullptr;
//    }
//    else {
//        perror("recv");
//        exit(EXIT_FAILURE);
//    }
//}

std::unique_ptr<Package> receive_package(int client_socket, int buffer_size) {
    auto data = std::make_unique<char[]>(buffer_size);
    int total_bytes_received = 0;
    while (total_bytes_received < buffer_size) {
        int result = recv(client_socket, data.get() + total_bytes_received, buffer_size - total_bytes_received, 0);
        if (result > 0) {
            total_bytes_received += result;
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
    auto package = std::make_unique<Package>();
    package->message = std::string(data.get(), buffer_size - sizeof(package->flag));
    memcpy(&package->flag, data.get() + buffer_size - sizeof(package->flag), sizeof(package->flag));
    return package;
}



void close_socket(int server_socket, int client_socket) {
#ifdef _WIN64
    closesocket(client_socket);
#else
    close(client_socket);
#endif

#ifdef _WIN64
    closesocket(server_socket);
#else
    close(server_socket);
#endif

#ifdef _WIN64
    WSACleanup();
#endif
}

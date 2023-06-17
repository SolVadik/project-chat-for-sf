#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <memory>

#ifdef _WIN64
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#elif __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

int create_socket();
void connect_to_server(int client_socket, const std::string& server_ip, int server_port);
int get_data_size(int socket);
std::unique_ptr<char[]> receive_data(int client_socket, int buffer_size);
void send_data(int client_socket, const std::unique_ptr<char[]>& buffer, int buffer_size);
void send_string(int client_socket, const std::string& message);
void close_socket(int client_socket);

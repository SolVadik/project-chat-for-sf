// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <unordered_set>
#include "Client.h"
#ifdef _WIN64
#include <conio.h>
#elif __linux__
#endif


int main() {
    int client_socket = create_socket();
    if (client_socket == -1) {
        return -1;
    }

    connect_to_server(client_socket, "127.0.0.1", 7777);

    //// container for storing messages that do not require a response 
    //std::unordered_set<std::string> messages_not_requiring_response = { "enter 1 - 2 or q\n", "enter 1 - 5 or q\n", "error: user login is busy\n", "error: user name is busy\n" };

    //while (true) {
    //    int data_size = get_data_size(client_socket);
    //    if (data_size > 0) {
    //        auto data = receive_data(client_socket, data_size);
    //        std::string chek(data.get(), data_size);
    //        std::cout << chek << std::endl;
    //        if (chek == "/End Work")
    //            break;
    //        if (chek == "Enter your message") {
    //            char ch;
    //            std::string message;
    //            while (std::cin.get(ch) && ch != '\n');
    //            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore garbage remaining in the buffer
    //            std::getline(std::cin, message);
    //            send_string(client_socket, message);
    //            continue;
    //        }

    //        // Check if the received message is in the container
    //        if (messages_not_requiring_response.find(chek) == messages_not_requiring_response.end() || chek[0] != '\n') {
    //            std::string message;
    //            std::cin >> message;
    //            send_string(client_socket, message);
    //        }
    //    }
    //}
    bool connect = true;
    while (connect) {
        int data_size = get_data_size(client_socket);
        if (data_size > 0) {
            auto data = receive_package(client_socket, data_size);
            switch (data->flag) {
            case '1': {
                std::cout << data->message << std::endl;
                Package message{ "0", '0' };
                std::cin >> message.flag;
                send_package(client_socket, message);
                break;
            }
            case '2': {
                std::cout << data->message << std::endl;
                break;
            }
            case '3': {
                std::cout << data->message << std::endl;
                Package message{ "0", '0' };
                std::cin >> message.message;
                send_package(client_socket, message);
                break;
            }
            case '4': {
                std::cout << data->message << std::endl;

                char ch;
                Package message{ "0", '0' };
                while (std::cin.get(ch) && ch != '\n');
                std::getline(std::cin, message.message);
                send_package(client_socket, message);
                continue;

            }
            case '5': {
                std::cout << data->message << std::endl;
                connect = false;
                break;
            }
            default: {
                std::cout << "error flag" << std::endl;
            }
            }
        }
    }




    close_socket(client_socket);

#ifdef _WIN64
    _getch();
#elif __linux__
#endif

    return 0;
}

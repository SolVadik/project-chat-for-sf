#include <unordered_set>
#include "Client.h"
//#include <conio.h>

int main() {
    int client_socket = create_socket();
    if (client_socket == -1) {
        return -1;
    }

    connect_to_server(client_socket, "127.0.0.1", 7777);

    // container for storing messages that do not require a response 
    std::unordered_set<std::string> messages_not_requiring_response = { "enter 1 - 2 or q\n", "enter 1 - 5 or q\n", "error: user login is busy\n", "error: user name is busy\n" };

    while (true) {
        int data_size = get_data_size(client_socket);
        if (data_size > 0) {
            auto data = receive_data(client_socket, data_size);
            std::string chek(data.get(), data_size);
            std::cout << chek << std::endl;
            if (chek == "/End Work")
                break;
            if (chek == "Enter your message") {
                char ch;
                std::string message;
                while (std::cin.get(ch) && ch != '\n');
                //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore garbage remaining in the buffer
                std::getline(std::cin, message);
                send_string(client_socket, message);
                continue;
            }

            // Check if the received message is in the container
            if (messages_not_requiring_response.find(chek) == messages_not_requiring_response.end()) {
                std::string message;
                std::cin >> message;
                send_string(client_socket, message);
            }
        }
    }

    close_socket(client_socket);

    //_getch();

    return 0;
}

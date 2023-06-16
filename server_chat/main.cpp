#include "Chat.h"

int  main(int argc, char* argv[]) 
{
    int server_socket = create_socket();
    if (server_socket == -1) {
        return -1;
    }

    int client_socket = accept_client(server_socket);
    if (client_socket == -1) {
        return -1;
    }


	Chat chat; // is_chat_work = false

	chat.start(); // is_chat_work = true

	while (chat.is_chat_work()){
		chat.show_login_menu(client_socket); // making user login
		while (chat.get_current_user()) {
			chat.show_user_menu(client_socket);
		}
	}

    close_socket(server_socket, client_socket);

	return 0;
}

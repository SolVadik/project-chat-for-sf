//This is a final project for Module 1 at SkillFactory course C-PLUS.
//Here we have to demonstrate our knowledges of C++ basics and make a simple console chat for 1 device.
#include "Chat.h"

int main()
{
	Chat chat; // is_chat_work = false

	chat.start(); // is_chat_work = true

	while (chat.is_chat_work()){
		chat.show_login_menu(); // making user login
		while (chat.get_current_user()) {
			chat.show_user_menu();
		}
	}
	return 0;
}
#include "Chat.h"

void Chat::start()
{
	// system info
#ifdef _WIN64

	std::time_t t = std::time(nullptr);
	char buffer[100];
	memset(buffer, 0, sizeof(buffer));
	struct tm timeinfo;
	localtime_s(&timeinfo, &t);
	std::strftime(buffer, sizeof(buffer), "%c", &timeinfo);
	std::cout << "This program is running on " << PLATFORM_NAME << " at " << buffer << std::endl;

#elif __linux__
	auto now = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(now);
	std::cout << "This program is running on " << PLATFORM_NAME << std::endl;
    std::cout << "Current Time and Date: " << ctime(&end_time) << std::endl;
#endif


	// read save
	std::ifstream file_users("users.txt");
	if (file_users.is_open()) {
		std::string login, name, pass;
		while (file_users >> login >> name >> pass) {
			users_.emplace_back(login, name, pass);
		}
		file_users.close();
	}
	else {
		std::cout << "file users not open " << std::endl;
	}

	std::ifstream file_messages("messages.txt");
	if (file_messages.is_open()) {
		std::string from, to, text;
		while (std::getline(file_messages, from) && std::getline(file_messages, to) && std::getline(file_messages, text)) {
			messages_.emplace_back(from, to, text);
		}
		file_messages.close();
}
	else {
		std::cout << "file messages not open " << std::endl;
	}

	is_chat_work_ = true;
}

void Chat::show_login_menu(int client_socket)
{
	

	std::string message = "Number for choise: \n 1 - Login \n 2 - SignUp \n q - Out";
	send_string(client_socket, message);

	int data_size = get_data_size(client_socket);
	if (data_size > 0) {
		auto data = receive_data(client_socket, data_size);
		//std::cout << std::endl << data << std::endl;
		char operation = data[0];


		switch (operation) {
		case '1': {
			login(client_socket);
			break;
		}
		case '2': {
			try
			{
				sign_up(client_socket);
			}
			catch (const std::exception& e)
			{
				//std::cout << e.what() << std::endl;
				std::string message = e.what();
				send_string(client_socket, message);
			}
			break;
		}
		case 'q': {
			// save users and messages
			std::ofstream file_users("users.txt");
			if (file_users.is_open()) {
				for (auto& user : users_) {
					file_users << user.get_login() << ' ' << user.get_name() << ' ' << user.get_password() << '\n';
				}
				file_users.close();
			}
			else {
				std::cout << "file users not open " << std::endl;
			}

			std::ofstream file("messages.txt");
			if (file.is_open()) {
				for (auto& message : messages_) {
					file << message.get_from() << std::endl << message.get_to() << std::endl << message.get_text() << std::endl;
				}
				file.close();
			}
			else {
				std::cout << "file messages not open " << std::endl;
			}

			// Set file permissions  600 (rw-------)
#ifdef _WIN64
#elif __linux__
			chmod("users.txt", S_IRUSR | S_IWUSR);
			chmod("messages.txt", S_IRUSR | S_IWUSR);
#endif

			std::string message = "/End Work";
			send_string(client_socket, message);

			is_chat_work_ = false;
			break;
		}
		default: {
			//std::cout << "enter 1 - 2 or q" << std::endl;

			std::string message = "enter 1 - 2 or q\n";
			send_string(client_socket, message);

			break;
		}
		}
	}
}

void Chat::login(int client_socket)
{
	std::string message;
	do {
		message = "Enter login: ";
		send_string(client_socket, message);

		int data_size = get_data_size(client_socket);
		if (data_size > 0) {
			auto data = receive_data(client_socket, data_size);
			std::string login(data.get(), data_size);

			message = "Enter password: ";
			send_string(client_socket, message);

			int data_size = get_data_size(client_socket);
			if (data_size > 0) {
				auto data = receive_data(client_socket, data_size);
				std::string password(data.get(), data_size);
				
				current_user_ = get_user_login(login);
				
				if (current_user_ == nullptr || current_user_->get_password() != password) {
					if (current_user_)
						current_user_ = nullptr;
					message = "Login or password error \nq for exit, any for retry ";
					send_string(client_socket, message);

					int data_size = get_data_size(client_socket);
					if (data_size > 0) {
						auto data = receive_data(client_socket, data_size);
						char operation = data[0];
						if (operation == 'q')
							return;
					}
			}
			}
		}
	} while (!current_user_);
}

void Chat::sign_up(int client_socket)
{
	std::string login, name, password, message;

	message = "Enter login: ";
	send_string(client_socket, message);

	int data_size = get_data_size(client_socket);
	if (data_size > 0) {
		auto data = receive_data(client_socket, data_size);
		std::string login(data.get(), data_size);

		if (get_user_login(login))
			throw UserLoginExp(); // checking is new login unique

		if (login == "q")
			throw UserLoginExp(); // q is resrved for right quit from Login



		message = "Enter name: ";
		send_string(client_socket, message);

		data_size = get_data_size(client_socket);
		if (data_size > 0) {
			auto data = receive_data(client_socket, data_size);
			std::string name(data.get(), data_size);

			if (get_user_name(name) || name == "All")
				throw UserNameExp(); // checking is new name unique


			message = "Enter password: ";
			send_string(client_socket, message);

			data_size = get_data_size(client_socket);
			if (data_size > 0) {
				auto data = receive_data(client_socket, data_size);
				std::string password(data.get(), data_size);
				users_.emplace_back(login, name, password);
			}
		}
	}
	
}

void Chat::show_user_menu(int client_socket) //showing what user can do in chat
{
	//char operation{ 0 };
	//std::cout << "Enter: \n 1 - Read message \n 2 - Add message \n 3 - View users \n 4 - Change name \n 5 - Change password \n q - Logout"
	//	<< std::endl;
	//std::cin >> operation;

	std::string message = "Enter: \n 1 - Read message \n 2 - Add message \n 3 - View users \n 4 - Change name \n 5 - Change password \n q - Logout";
	send_string(client_socket, message);

	int data_size = get_data_size(client_socket);
	if (data_size > 0) {
		auto data = receive_data(client_socket, data_size);
		//std::cout << std::endl << data << std::endl;

		char operation = data[0];
		switch (operation) {
		case '1': {
			show_chat(client_socket);
			break;
		}
		case '2': {
			add_message(client_socket);
			break;
		}
		case '3': {
			show_all_user_name(client_socket);
			break;
		}
		case '4': {
			try
			{
				change_name(client_socket);
			}
			catch (const std::exception& e)
			{
				//std::cout << e.what() << std::endl;
				std::string message = e.what();
				send_string(client_socket, message);
			}
			break;
		}
		case '5': {
			change_password(client_socket);
			break;
		}
		case 'q': {
			current_user_ = nullptr;
			break;
		}
		default: {
			//std::cout << "enter 1 - 5 or q" << std::endl;
			std::string message = "enter 1 - 5 or q\n";
			send_string(client_socket, message);
			break;
		}
		}
	}
}

void Chat::change_name(int client_socket)
{
	//std::string name;
	//std::cout << "Enter new name: " << std::endl;
	//std::cin >> name;
	std::string message = "Enter new name: ";
	send_string(client_socket, message);

	int data_size = get_data_size(client_socket);
	if (data_size > 0) {
		auto data = receive_data(client_socket, data_size);
		std::string name(data.get(), data_size);
		if (get_user_name(name) || name == "All")
			throw UserNameExp();
		for (auto& user : users_) {
			if (current_user_->get_login() == user.get_login()) {
				user.set_name(name);
				// current variant
				current_user_->set_name(name);

				// alternative variant. force user relog
				// current_user_ = nullptr;

				// alternative variant. if use need add Rule of five.
				//current_user_ = move(get_user_login(current_user_->get_login()));
				return;
			}
		}
	}

}

void Chat::change_password(int client_socket)
{
	//std::string password;
	//std::cout << "Enter new password: " << std::endl;
	//std::cin >> password;

	std::string message = "Enter new password: ";
	send_string(client_socket, message);

	int data_size = get_data_size(client_socket);
	if (data_size > 0) {
		auto data = receive_data(client_socket, data_size);
		std::string password(data.get(), data_size);

		for (auto& user : users_) {
			if (current_user_->get_login() == user.get_login()) {
				user.set_password(password);
				// current variant
				current_user_->set_password(password);

				// alternative variant. force user relog
				// current_user_ = nullptr;

				// alternative variant. if use need add Rule of five.
				//current_user_ = move(get_user_login(current_user_->get_login()));
				return;
			}
		}
	}
}

void Chat::show_chat(int client_socket) const // showing all messages
{
//#ifdef _WIN64
//	system("cls");
//#elif __linux__
//	system("clear");
//#endif	
	std::ostringstream ss;
	for (auto& message : messages_) {
		if (current_user_->get_name() == message.get_from())
			ss << message.get_from() << ": " << message.get_text() << std::endl;
		else if (current_user_->get_name() == message.get_to())
			ss << message.get_from() << " to me(private): " << message.get_text() << std::endl;
		else if (message.get_to() == "All")
			ss << message.get_from() << ": " << message.get_text() << std::endl;
	}

	std::string serialized_data = ss.str();
	if (send(client_socket, serialized_data.c_str(), serialized_data.size(), 0) == -1) {
		std::cerr << "Failed to send data to client" << std::endl;
	}

	/*for (auto& message : messages_) {
		if (current_user_->get_name() == message.get_from())
			std::cout << "\x1b[33m" << message.get_from() << ": " << message.get_text() << "\x1b[0m" << std::endl;
		else if (current_user_->get_name() == message.get_to())
			std::cout << "\x1b[31m" << message.get_from() << " to me(private): " << message.get_text() << "\x1b[0m" << std::endl;
		else if (message.get_to() == "All")
			std::cout << "\x1b[32m" << message.get_from() << ": " << message.get_text() << "\x1b[0m" << std::endl;
	}*/
}

void Chat::show_all_user_name(int client_socket) const // showing all users in chat
{
	/*for (auto& user : users_)
	{
		std::cout << user.get_name() << std::endl;
	}*/

	std::ostringstream ss;
	for (const auto& user : users_) {
		ss << user.get_name() << "\n";
	}
	std::string serialized_data = ss.str();
	if (send(client_socket, serialized_data.c_str(), serialized_data.size(), 0) == -1) {
		std::cerr << "Failed to send data to client" << std::endl;
	}

}

void Chat::add_message(int client_socket)
{
	std::string from, to, text;
	bool flag{ true };
	from = current_user_->get_name();
	do {
		flag = false;
		//std::cout << "Enter All to send to everyone, or enter a name" << std::endl;
		//std::cin >> to;

		std::string message = "Enter All to send to everyone, or enter a name";
		send_string(client_socket, message);

		int data_size = get_data_size(client_socket);
		if (data_size > 0) {
			auto data = receive_data(client_socket, data_size);
			std::string to(data.get(), data_size);

			if (get_user_name(to) == nullptr && to != "All") {
				flag = true;
				/*std::cout << "this name not found" << std::endl;
				std::cout << "q for exit, any for retry" << std::endl;
				std::cin >> operation;*/

				std::string message = "this name not found\nq for exit, any for retry";
				send_string(client_socket, message);

				int data_size = get_data_size(client_socket);
				if (data_size > 0) {
					auto data = receive_data(client_socket, data_size);
					char operation = data[0];
					if (operation == 'q')
						return;
				}
			}
		}
	} while (flag);
	//std::cout << "Enter your message" << std::endl;

	std::string message = "Enter your message";
	send_string(client_socket, message);


	//char ch;
	//while (std::cin.get(ch) && ch != '\n');
	////std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore garbage remaining in the buffer
	//std::getline(std::cin, text);

	int data_size = get_data_size(client_socket);
	if (data_size > 0) {
		auto data = receive_data(client_socket, data_size);
		std::string text(data.get(), data_size);


		messages_.emplace_back(from, to, text);
	}
}

std::shared_ptr<User<std::string>> Chat::get_user_login(const std::string& login) const // searching user login among all users in chat
{
	for (auto& user : users_)
	{
		if (login == user.get_login())
			return std::make_shared<User<std::string>>(user);

	}
	return nullptr;
}

std::shared_ptr<User<std::string>> Chat::get_user_name(const std::string& name) const // searching user name among all users in chat
{
	for (auto& user : users_)
	{
		if (name == user.get_name())
			return std::make_shared<User<std::string>>(user);

	}
	return nullptr;
}

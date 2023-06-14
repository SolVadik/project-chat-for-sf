#include "Chat.h"

void Chat::start()
{
	// system info
#ifdef _WIN32

	std::time_t t = std::time(nullptr);
	char buffer[100];
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
		std::cout << "file not open " << std::endl;
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
		std::cout << "file not open " << std::endl;
	}

	is_chat_work_ = true;
	
}

void Chat::show_login_menu()
{
	char operation;
	std::cout << "Number for choise: \n 1 - Login \n 2 - SignUp \n q - Out " << std::endl;
	std::cin >> operation;
	switch (operation) {
	case '1': {
		login();
		break;
	}
	case '2': {
		try
		{
			sign_up();
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
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
			std::cout << "file not open " << std::endl;
		}

		std::ofstream file("messages.txt");
		if (file.is_open()) {
			for (auto& message : messages_) {
				file << message.get_from() << std::endl << message.get_to() << std::endl << message.get_text() << std::endl;
			}
			file.close();
		}
		else {
			std::cout << "file not open " << std::endl;
		}

		// Set file permissions  600 (rw-------)
#ifdef _WIN32
#elif __linux__
		chmod("users.txt", S_IRUSR | S_IWUSR);
		chmod("messages.txt", S_IRUSR | S_IWUSR);
#endif

		is_chat_work_ = false;
		break;
	}
	default: {
		std::cout << "enter 1 - 2 or q" << std::endl;
		break;
	}
	}
}

void Chat::login()
{
	std::string login, password;
	char operation{ 0 };
	do {
		std::cout << "Enter login: " << std::endl;
		std::cin >> login;
		std::cout << std::endl;

		std::cout << "Enter password: " << std::endl;
		std::cin >> password;
		std::cout << std::endl;

		current_user_ = get_user_login(login);

		if (current_user_ == nullptr || current_user_->get_password() != password) {
			if (current_user_)
				current_user_ = nullptr;
			std::cout << "Login or password error" << std::endl;
			std::cout << "q for exit, any for retry" << std::endl;
			std::cin >> operation;
			if (operation == 'q')
				return;
		}
	} while (!current_user_);
}

void Chat::sign_up()
{
	std::string login, name, password;

	std::cout << "Enter login: " << std::endl;
	std::cin >> login;

	if (get_user_login(login))
		throw UserLoginExp(); // checking is new login unique

	if (login == "q")
		throw UserLoginExp(); // q is resrved for right quit from Login

	std::cout << "Enter name: " << std::endl;
	std::cin >> name;

	if (get_user_name(name) || name == "All")
		throw UserNameExp(); // checking is new name unique

	std::cout << "Enter password: " << std::endl;
	std::cin >> password;
	std::cout << std::endl;

	users_.emplace_back(login, name, password);
}

void Chat::show_user_menu() //showing what user can do in chat
{
	char operation{ 0 };
	std::cout << "Enter: \n 1 - Read message \n 2 - Add message \n 3 - View users \n 4 - Change name \n 5 - Change password \n q - Logout"
		<< std::endl;
	std::cin >> operation;
	switch (operation) {
	case '1': {
		show_chat();
		break;
	}
	case '2': {
		add_message();
		break;
	}
	case '3': {
		show_all_user_name();
		break;
	}
	case '4': {
		try
		{
			change_name();
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		break;
	}
	case '5': {
		change_password();
		break;
	}
	case 'q': {
		current_user_ = nullptr;
		break;
	}
	default: {
		std::cout << "enter 1 - 5 or q" << std::endl;
		break;
	}
	}
}

void Chat::change_name()
{
	std::string name;
	std::cout << "Enter new name: " << std::endl;
	std::cin >> name;
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

void Chat::change_password()
{
	std::string password;
	std::cout << "Enter new password: " << std::endl;
	std::cin >> password;
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

void Chat::show_chat() const // showing all messages
{
#ifdef _WIN32
	system("cls");
#elif __linux__
	system("clear");
#endif
	

	for (auto& message : messages_) {
		if (current_user_->get_name() == message.get_from())
			std::cout << "\x1b[33m" << message.get_from() << ": " << message.get_text() << "\x1b[0m" << std::endl;
		else if (current_user_->get_name() == message.get_to())
			std::cout << "\x1b[31m" << message.get_from() << " to me(private): " << message.get_text() << "\x1b[0m" << std::endl;
		else if (message.get_to() == "All")
			std::cout << "\x1b[32m" << message.get_from() << ": " << message.get_text() << "\x1b[0m" << std::endl;
	}
}

void Chat::show_all_user_name() const // showing all users in chat
{
	for (auto& user : users_)
	{
		std::cout << user.get_name() << std::endl;
	}
}

void Chat::add_message()
{
	std::string from, to, text;
	bool flag{ true };
	char operation;
	from = current_user_->get_name();
	do {
		flag = false;
		std::cout << "Enter All to send to everyone, or enter a name" << std::endl;
		std::cin >> to;
		if (get_user_name(to) == nullptr && to != "All") {
			flag = true;
			std::cout << "this name not found" << std::endl;
			std::cout << "q for exit, any for retry" << std::endl;
			std::cin >> operation;
			if (operation == 'q')
				return;
		}
	} while (flag);
	std::cout << "Enter your message" << std::endl;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore garbage remaining in the buffer
	std::getline(std::cin, text);
	messages_.emplace_back(from,to,text);
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

#pragma once
#include <vector>
#include <exception>
#include <fstream>
#include <filesystem>
#include <sstream>
#ifdef _WIN64
constexpr auto PLATFORM_NAME = "Windows";
#elif __linux__
constexpr auto PLATFORM_NAME = "Linux";
#include <chrono>
//#include <limits>
#include <sys/stat.h>
#endif
#include "Message.h"
#include "User.h"
#include "Server.h"
struct UserLoginExp : public std::exception
{
	const char* what() const noexcept override { return "error: user login is busy\n"; }
};

struct UserNameExp : public std::exception
{
	const char* what() const noexcept override { return "error: user name is busy\n"; }
};

class Chat
{
	bool is_chat_work_ = false;
	std::vector<User<std::string>> users_; // saving all users in chat
	std::vector<Message<std::string>> messages_; // saving all messages in chat
	std::shared_ptr<User<std::string>> current_user_;

	void login(int client_socket);
	void sign_up(int client_socket);
	void show_chat(int client_socket) const;
	void show_all_user_name(int client_soket) const;
	void add_message(int client_socket);

	std::vector<User<std::string>>& get_all_users() { return users_; }
	std::vector<Message<std::string>>& get_all_messages() { return messages_; }
	std::shared_ptr<User<std::string>> get_user_login(const std::string& login) const;
	std::shared_ptr<User<std::string>> get_user_name(const std::string& name) const;

public:
	void start();
	bool is_chat_work() const { return is_chat_work_; }
	std::shared_ptr<User<std::string>> get_current_user() const { return current_user_; }
	void show_login_menu(int client_socket);
	void show_user_menu(int client_socket);
	void change_name(int client_socket);
	void change_password(int client_socket);
};

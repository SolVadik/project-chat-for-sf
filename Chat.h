#pragma once
#include <vector>
#include <memory>
#include <exception>
#include <iostream>
#include <string>
#include "Message.h"
#include "User.h"

struct UserLoginExp : public std::exception
{
	const char* what() const noexcept override { return "error: user login is busy"; }
};

struct UserNameExp : public std::exception
{
	const char* what() const noexcept override { return "error: user name is busy"; }
};

class Chat
{
	bool is_chat_work_ = false;
	std::vector<User<std::string>> users_; // saving all users in chat
	std::vector<Message<std::string>> messages_; // saving all messages in chat
	std::shared_ptr<User<std::string>> current_user_;

	void login();
	void sign_up();
	void show_chat() const;
	void show_all_user_name() const;
	void add_message();

	std::vector<User<std::string>>& get_all_users() { return users_; }
	std::vector<Message<std::string>>& get_all_messages() { return messages_; }
	std::shared_ptr<User<std::string>> get_user_login(const std::string& login) const;
	std::shared_ptr<User<std::string>> get_user_name(const std::string& name) const;

public:
	void start();
	bool is_chat_work() const { return is_chat_work_; }
	std::shared_ptr<User<std::string>> get_current_user() const { return current_user_; }
	void show_login_menu();
	void show_user_menu();
	void change_name();
	void change_password();
};
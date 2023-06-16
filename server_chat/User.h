#pragma once

template <class T>
class User
{
	const T login_; // cannot be change
	T name_;
	T password_;
public:
	User(const T& login, const T& name, const T& password) :
		login_(login), name_(name), password_(password) {};
	
	//~User() = default;

	const T& get_login() const { return login_; };
	const T& get_name() const { return name_; };
	const T& get_password() const { return password_; };

	void set_name(const T& name) { name_ = name; };
	void set_password(const T& password) { password_ = password; };
};


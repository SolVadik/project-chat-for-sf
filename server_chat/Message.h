#pragma once

template <class T>
class Message
{
	const T from_;
	const T to_;
	const T text_;
public:
	Message(T& from, T& to, T& text) :
		from_(from), to_(to), text_(text) {};
	
	//~Message() = default;

	const T& get_from() const { return from_; };
	const T& get_to() const { return to_; };
	const T& get_text() const { return text_; };
};

#pragma once
#include <variant>

template <typename T>
struct Ok {
	Ok(T& val): val(val) {}
	Ok(T&& val): val(std::move(val)) {}

	T&& unwrap() && {
		return std::move(this->val);
	}

private:
	T val;
};

template <typename E>
struct Err {
	Err(E& val): val(val) {}
	Err(E&& val): val(std::move(val)) {}

private:
	E val;
};

template <typename T, typename E>
struct Result
{
	Result(Ok<T>&& ok): val(std::move(ok)) {}
	Result(Err<E>&& err): val(std::move(err)) {}

	T&& unwrap() && {
		return std::get<Ok<T>>(std::move(this->val)).unwrap();
	}

	explicit operator bool() const {
		return std::holds_alternative<Ok<T>>(this->val);
	}

	// return std::visit([](auto&& val) -> T { return val.unwrap(); }, this->val);

private:
	std::variant<Ok<T>, Err<E>> val;
};

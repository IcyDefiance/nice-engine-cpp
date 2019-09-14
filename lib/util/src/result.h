#pragma once
#include "opt.h"

template <typename T>
struct Ok {
	explicit Ok(T& val): val(val) {}
	explicit Ok(T&& val): val(std::move(val)) {}

	T&& unwrap() && {
		return std::move(this->val);
	}

private:
	T val;
};

template <typename E>
struct Err {
	explicit Err(E& val): val(val) {}
	explicit Err(E&& val): val(std::move(val)) {}

private:
	E val;
};

template <typename T, typename E>
struct Result
{
private:
	template <typename F>
	using FRet = std::invoke_result_t<F, T&&>;

public:
	Result(Ok<T>&& ok): val(std::move(ok)) {}
	Result(Err<E>&& err): val(std::move(err)) {}

	template <typename F>
	Result<FRet<F>, E> map(F cb) {
		if (auto val = std::get_if<Ok<T>>(&this->val)) {
			return Ok(cb(move(*val).unwrap()));
		} else {
			return std::get<Err<E>>(move(this->val));
		}
	}

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

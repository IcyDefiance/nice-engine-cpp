#pragma once
#include <variant>

template<typename T>
struct Some {
	Some(T& val): val(val) {}
	Some(T&& val): val(std::move(val)) {}

	T&& unwrap() && { return std::move(this->val); }

	T& operator*() { return this->val; }
	const T& operator*() const { return this->val; }

private:
	T val;
};

struct None {};

template<typename T>
struct Opt
{
private:
	template <typename F>
	using FRet = std::invoke_result_t<F, T&&>;

public:
	Opt(Some<T>&& some) : val(std::move(some)) {}
	Opt(None&& none) : val(none) {}

	template <typename F>
	Opt<FRet<F>> map(F cb) {
		if (auto val = std::get_if<Some<T>>(&this->val)) {
			return Opt<FRet<F>>(Some(cb(move(*val).unwrap())));
		} else {
			return Opt<FRet<F>>(None());
		}
	}

	T&& unwrap() && {
		return std::get<Some<T>>(std::move(this->val)).unwrap();
	}

	explicit operator bool() const { return std::holds_alternative<Some<T>>(this->val); }
	T& operator*() { return *std::get<Some<T>>(this->val); }
	const T& operator*() const { return *std::get<Some<T>>(this->val); }

private:
	std::variant<Some<T>, None> val;
};

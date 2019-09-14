#pragma once
#include "opt.h"

template <typename Cur, typename End>
struct StdIter {
	using Item = std::reference_wrapper<typename std::iterator_traits<Cur>::value_type>;

	StdIter() = default;
	StdIter(Cur cur, End end) : cur(cur), end(end) {}

	Opt<Item> next() {
		if (cur != end) {
			return Some(std::reference_wrapper(*cur++));
		} else {
			return None();
		}
	}

private:
	Cur cur;
	End end;
};

template <typename T>
struct OwnedIter {
	using Item = typename T::value_type;

	explicit OwnedIter(T&& source) : source(move(source)) {
		iter = StdIter(source.begin(), source.end());
	}
	OwnedIter(OwnedIter&& other) : source(move(other.source)) {
		iter = StdIter(source.begin(), source.end());
	}

	Opt<Item> next() {
		return iter.next().map([](auto& x) { return move(x.get()); });
	}

private:
	StdIter<typename T::iterator, typename T::iterator> iter;
	T source;
};

template <typename T>
OwnedIter<T> iter(T&& source) {
	return OwnedIter(move(source));
}

template <typename T>
StdIter<typename T::iterator, typename T::iterator> iter(T& source) {
	return StdIter(source.begin(), source.end());
}

template <typename I, typename F>
struct TransformIter {
	using Item = decltype(std::declval<F>()(std::declval<I::Item>()));

	TransformIter(I&& iter, F func) : iter(move(iter)), func(func) {}

	Opt<Item> next() {
		if (const auto value = iter.next()) {
			return func(*value);
		} else {
			return None();
		}
	}

private:
	I iter;
	F func;
};

template <typename F>
struct Transform {
	F func;

	explicit Transform(F func) : func(func) {}
};

template <typename I, typename F>
TransformIter<I, F> operator|(I iter, Transform<F> proxy) {
	return TransformIter(move(iter), proxy.func);
}

struct Count {};

template <typename I>
usize operator|(I& iter, Count proxy) {
	usize count = 0;
	while (iter.next()) {
		count++;
	}
	return count;
}

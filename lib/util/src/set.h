#include <unordered_set>

template <typename S, typename I>
struct Intersection;

template<typename Key, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
struct Set  {
	using value_type = Key;
	using size_type = size_t;
	using key_equal = KeyEqual;

	std::unordered_set<Key, Hash, KeyEqual> inner;

	Set() = default;

	explicit Set(size_type bucket_count, const Hash& hash = Hash(), const key_equal& equal = key_equal())
		: inner(bucket_count, hash, equal) {}

	template<class InputIt>
	Set(
		InputIt first,
		InputIt last,
		size_type bucket_count = 0,
		const Hash& hash = Hash(),
		const key_equal& equal = key_equal()
	) : inner(first, last, bucket_count, hash, equal) {}

	Set(const Set& other) = delete;

	Set(Set&& other) noexcept : inner(other.inner) {}

	Set(
		std::initializer_list<value_type> init,
		size_type bucket_count = 0,
		const Hash& hash = Hash(),
		const key_equal& equal = key_equal()
	) : inner(init, bucket_count, hash, equal) {}

	bool contains(const Key& key) const { return inner.find(key) != inner.end(); }

	template <typename I>
	Intersection<Set, I> intersection(I&& other) const { return Intersection(*this, move(other)); }
};

template <typename S, typename I>
struct Intersection {
	using Item = typename I::Item;

	Intersection(const S& set, I&& iter) : set(set), iter(move(iter)) {}

	Opt<Item> next() {
		while (const auto value = iter.next()) {
			if (set.contains(*value)) {
				return value;
			}
		}

		return None();
	}

private:
	const S& set;
	I iter;
};

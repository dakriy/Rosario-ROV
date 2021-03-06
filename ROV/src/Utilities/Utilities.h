#pragma once
#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>

/**
 * @brief
 * C++ Implementation of Python's enumerate, can be used like this:
 * 
 * for (auto [i, thing] : enumerate(things))
 * {
 *     // i gets the index and thing gets the Thing in each iteration
 * }
 * Credit to:
 * http://reedbeta.com/blog/python-like-enumerate-in-cpp17/
 */
template <typename T,
	typename TIter = decltype(std::begin(std::declval<T>())),
	typename = decltype(std::end(std::declval<T>()))>
	constexpr auto enumerate(T && iterable)
{
	struct iterator
	{
		size_t i;
		TIter iter;
		bool operator != (const iterator & other) const { return iter != other.iter; }
		void operator ++ () { ++i; ++iter; }
		auto operator * () const { return std::tie(i, *iter); }
	};
	struct iterable_wrapper
	{
		T iterable;
		auto begin() { return iterator{ 0, std::begin(iterable) }; }
		auto end() { return iterator{ 0, std::end(iterable) }; }
	};
	return iterable_wrapper{ std::forward<T>(iterable) };
}


// https://stackoverflow.com/questions/207976/how-to-easily-map-c-enums-to-strings
template<typename T> struct map_init_helper
{
	T& data;
	explicit map_init_helper(T& d) : data(d) {}
	map_init_helper& operator() (typename T::key_type const& key, typename T::mapped_type const& value)
	{
		data[key] = value;
		return *this;
	}
};

template<typename T> map_init_helper<T> map_init(T& item)
{
	return map_init_helper<T>(item);
}

std::vector<uint8_t> yeetFileIntoByteVector(std::ifstream & file);

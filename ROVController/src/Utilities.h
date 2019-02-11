#pragma once
#include <random>
#include <iostream>
#include "Globals.h"
#include <SFML/System/Vector2.hpp>
#include <tuple>

static std::mt19937 rnd = std::mt19937();

int GetRandomNumberInRange(int min, int max);

unsigned GetSeed(const std::string& seed);

void QuitWithError(const char * error, int exit_code);

/**
 * Performs x % y but works proerply with negative numbers
 */
constexpr int ProperModulus(int x, int y)
{
	return ((x % y) + y) % y;
}

/**
 * @brief
 * Vector lerping between two vectors
 * When t = 1, it returns v1
 * When t = 0, it returns v0
 * 
 * @param a first vector
 * @param b second vector
 * @param t value to extrapolate at
 */
template<typename T>
sf::Vector2<T> lerp_v(const sf::Vector2<T> a, const sf::Vector2<T> b, T t)
{
	return (1 - t) * a + t * b;
}

/**
 * @brief slightly faster version of lerp_v but less accurate in edge cases
 * When t = 1, it returns v1
 * When t = 0, it returns v0
 * 
 * @param a first vector
 * @param b second vector
 * @param t value to extrapolate at
 */
template<typename T>
sf::Vector2<T> lerp_v_f(const sf::Vector2<T> a, const sf::Vector2<T> b, T t)
{
	return a + (b - a) * t;
}

/**
 * @brief
 * For lerping between two given points, and a third x value 
 * that you want the y value at.
 * 
 * @param y0 first y value
 * @param y1 second y value
 * @param x0 first x value
 * @param x1 second x value
 * @param x the point you want the y value at
 */
template<typename T>
T lerp(const T y0, const T y1, const T x0, const T x1, const T x)
{
	return y0 + ((x - x0)*(y1 - y0))/(x1 - x0);
}

/**
* @brief
* slightly faster version of lerp_n but less accurate in edge cases
* When t = 1, it returns v1
* When t = 0, it returns v0
*
*
* @param t is a value between 0 and 1
* @param v0 is the starting value
* @param v1 is the ending value
*/
template<typename T>
T lerp_n_f(const T v0, const T v1, const T t)
{
	return v0 + (v1 - v0) * t;
}

/**
 * @brief
 * For lerping between two values
 * When t = 1, it returns v1
 * When t = 0, it returns v0
 * 
 * 
 * @param t is a value between 0 and 1
 * @param v0 is the starting value
 * @param v1 is the ending value
 */
template<typename T>
T lerp_n(const T v0, const T v1, const T t)
{
	return (1 - t) * v0 + t * v1;
}

/**
 * @brief
 * C++ Implmentation of Python's enumerate, can be used like this:
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



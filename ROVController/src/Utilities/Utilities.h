#pragma once
#include <random>
#include <iostream>
#include "../Globals.h"
#include <SFML/System/Vector2.hpp>
#include <tuple>
#include <SFML/Graphics/Image.hpp>

static std::mt19937 rnd = std::mt19937();

int GetRandomNumberInRange(int min, int max);

unsigned GetSeed(const std::string& seed);

void QuitWithError(const char * error, int exit_code);

sf::Vector2f convertToScreenCoords(sf::Rect<double> bounds, sf::Vector2<double> coords);

/**
 * Performs x % y but works properly with negative numbers
 */
constexpr int ProperModulus(int x, int y)
{
	return ((x % y) + y) % y;
}

// Use with integer types
template<typename T>
T RountToNearestWhole(T x)
{
	return static_cast<T>(x + 0.5);
}

/*inline int RoundToNearest(int x, int multiple)
{
	return ((x + multiple / 2) / multiple) * multiple;
}*/

template<typename T>
inline T RoundToNearest(T x, T multiple)
{
	return static_cast<T>(floor(static_cast<double>(x / multiple + 0.5)) * multiple);
}

inline double error(double x1, double x2)
{
	return abs(x2 - x1) / x2;
}

constexpr unsigned GetNumberOfDigits(unsigned i)
{
	if (i > 300)
		return 300;
	// Multiply going up rather than divide going down
	// because multiplies are faster than divides.
	unsigned digits = 1, pten = 10;
	while (pten < i)
	{
		digits++;
		pten *= 10;
	}
	return digits;
}

constexpr unsigned GetNumberOfDigits(unsigned long long int i)
{
	if (i > 300)
		return 300;
	// Multiply going up rather than divide going down
	// because multiplies are faster than divides.
	unsigned digits = 1, pten = 10;
	while (pten < i)
	{
		digits++;
		pten *= 10;
	}
	return digits;
}

// Only for windows and not being used
// void copyScreenshotToClipboard(const sf::Image & image);

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
sf::Vector2<T> lerp_v(const sf::Vector2<T> a, const sf::Vector2<T> b, const float t)
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
sf::Vector2<T> lerp_v_f(const sf::Vector2<T> a, const sf::Vector2<T> b, const float t)
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
T lerp_n_f(const T v0, const T v1, const float t)
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
T lerp_n(const T v0, const T v1, const float t)
{
	return (1 - t) * v0 + t * v1;
}

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

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

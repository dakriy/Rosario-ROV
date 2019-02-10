#pragma once
#include <SFML/System.hpp>

namespace Interfaces
{
	class IUpdateable
	{
	public:
		IUpdateable() = default;
		// Do not rely on this being called framerate times per second!
		virtual void update(const sf::Time&) = 0;
		virtual ~IUpdateable() = default;
	};

}
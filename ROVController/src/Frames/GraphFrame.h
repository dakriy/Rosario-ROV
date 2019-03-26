#pragma once

/**
 * @file GraphFrame.h
 * Graph taken data or functions
 *
 * @author dakriy
 */

#include "../UI/Options.h"

namespace Frames
{
	class GraphFrame : public IFrame
	{
	protected:
		const std::vector<int> graphIntervals = {
			1,
			2,
			5
		};

		// Each delta scroll either zooms in 10% or out 6%
		const double zoomTickAmount = .06;

		const sf::Vector2i linesPerScreenTarget;

		sf::Vector2u windowSize;

		EVENT_FUNC_INDEX(sf::Event, sf::Event::EventType::Count) resizeHook;
		EVENT_FUNC_INDEX(sf::Event, sf::Event::EventType::Count) scrollHook;

		sf::View g;

		// Position of top left
		sf::Rect<double> graphBounds;
	public:

		GraphFrame();

		/**
		 * Draws the graph frame
		 *
		 * @param target the render target
		 * @param states the render states
		 */
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void drawGrid(sf::RenderTarget& target, sf::RenderStates states) const;

		double calculateGridScale(double targetScale) const;

		/**
		 * Updates the frame
		 *
		 * @param dt time elapsed since last update
		 */
		void update(const sf::Time& dt) override;

		void setHooks();

		void zoomRelative(int x, int y, float amount);

		void setZoom(sf::Vector2<double> xRange, sf::Vector2<double> yRange);

		double getScaleX() const;
		
		double getScaleY() const;

		sf::Vector2u convertToScreenCoords(sf::Vector2<double> coords) const;

		bool xAxisVisible() const;

		bool yAxisVisible() const;

		FrameType get_type() const override;

		~GraphFrame() override;
	};
}

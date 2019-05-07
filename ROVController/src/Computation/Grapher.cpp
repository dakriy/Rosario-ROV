#include <imgui.h>
#include "Grapher.h"
#include "../Utilities/Utilities.h"
#include "../Core/GlobalContext.h"

// TODO: Combine points and lines so that I only need one vertex array...
Computation::Grapher::Grapher() : points(sf::LineStrip, array_count), type(Computation::GraphingHint::Squares), debug(sf::Points), dataPoints(sf::Points) {}

Computation::Grapher::Grapher(sf::Rect<double> bounds, Computation::Expression *expr) : expr(expr), points(sf::LineStrip, array_count), bounds(bounds), debug(sf::Points), dataPoints(sf::Points) {
	type = expr->graphingSuggestion();
}

void Computation::Grapher::setExpression(Computation::Expression *expression) {
	if (expression) {
		delete expr;
		expr = new Expression(*expression);
		type = expr->graphingSuggestion();
	}
}

Computation::Grapher::~Grapher() {
	delete expr;
}

void Computation::Grapher::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	if (expr)
	{
		target.draw(points);
		if (flag && type == Computation::GraphingHint::Squares) {
			target.draw(debug);
		}
	}
	if (graphingData) {
		target.draw(dataPoints);
	}
}

void Computation::Grapher::update() {
	ImGui::Begin("Graph Data");

	auto cols = GlobalContext::get_device()->getMission().getDataCols();

	std::vector<const char *> separatedByZeroes;
	separatedByZeroes.reserve(cols.size());

	for (auto & str : cols) {
		separatedByZeroes.push_back(str.c_str());
	}

	ImGui::Combo("Select X Axis", &xAxis, separatedByZeroes.data(), static_cast<int>(separatedByZeroes.size()));
	ImGui::Combo("Select Y Axis", &yAxis, separatedByZeroes.data(), static_cast<int>(separatedByZeroes.size()));


	if (ImGui::Button("Graph")) {
		graphingData = true;
	}

	if (ImGui::Button("Stop Graph")) {
		graphingData = false;
	}

	ImGui::End();

	if (graphingData) {
		auto data = GlobalContext::get_device()->getMission().getData();

		dataPoints.resize(data.size());
		dataPoints.clear();

		for (auto & datum : data) {
			dataPoints.append(sf::Vertex(convertToScreenCoords(bounds, sf::Vector2<double>(datum[xAxis], datum[yAxis])), sf::Color::Cyan));
		}
	}

	if (expr)
	{
		// Set the type
		if (type == Computation::GraphingHint::Horizontal || type == Computation::GraphingHint::Vertical) {
			points.resize(array_count);
			points.setPrimitiveType(sf::LineStrip);
		} else if (type == Computation::GraphingHint::Squares) {
			// 16 times bigger? Maybe?
			points.resize(array_count * 16);
			points.setPrimitiveType(sf::Lines);
			debug.resize(array_count * 16);
		}

		if(type == Computation::GraphingHint::Horizontal)
		{
			for (auto i = 0; i < array_count; ++i)
			{
				// For now assume y = something
				// So have to flip sign if y is negative
				// Because in the tree 0 - right side = -right
				auto x = bounds.left + bounds.width / array_count * i;
				auto y = - expr->compute(x, 0);
				points[i] = sf::Vertex(convertToScreenCoords(bounds, sf::Vector2<double>(x, y)), sf::Color::Red);
			}
		}

		if (type == Computation::GraphingHint::Vertical)
		{
			for (auto i = 0; i < array_count; ++i)
			{
				// For now assume x = something
				// So have to flip sign if x is negative
				// Because in the tree 0 - right side = -right
				auto y = bounds.top - bounds.height / array_count * i;
				auto x = - expr->compute(0, y);
				points[i] = sf::Vertex(convertToScreenCoords(bounds, sf::Vector2<double>(x, y)), sf::Color::Red);
			}
		}

		if (type == Computation::GraphingHint::Squares)
		{
			// clear doesn't deallocate the corresponding memory, so that adding new vertices after clearing doesn't involve reallocating all the memory.
			points.clear();

			static int squaresX = 100;
			static int iterations = 3;

			ImGui::Begin("Marching Squares");
			ImGui::SliderInt("Squares along the x axis", &squaresX, 1, 200);
			ImGui::SliderInt("Iterations", &iterations, 1, 10);
			ImGui::SliderInt("Iteration Squares", &iterationSquares, 2, 10);
			ImGui::Checkbox("Debug Dots", &flag);
			ImGui::End();

			marchingSquares(bounds, squaresX, iterations);
		}
	}
}

void Computation::Grapher::updateBounds(sf::Rect<double> newBounds) {
	bounds = newBounds;
}

void Computation::Grapher::move(double dx, double dy)
{
	updateBounds(sf::Rect<double>(bounds.left + dx, bounds.top + dy, bounds.width, bounds.height));
}

void Computation::Grapher::move(sf::Vector2<double> d) {
	move(d.x, d.y);
}

void Computation::Grapher::marchingSquares(sf::Rect<double> area, int squaresX, int max, unsigned current) {
	auto squareSide = area.width / static_cast<double>(squaresX);

	int squaresY = static_cast<int>(std::round(area.height / squareSide));

	// Structure to hold all the points of each square
	// one more point than square for each edge, e.g. 1 square, 2 points for each side
	// or 2 squares on one axis, 3 points along each axis because the middle points are shared.
	std::vector<int> grid((squaresX + 1) * (squaresY + 1), 0);

	// Tips for indexing grid because this messed me up for the longest time...
	// it's y * (row size) + x right?
	// Thing that messed me up was that row size is squaresX + 1 not squaresX. Led to some interesting bugs
	// that were really hard to debug...

	// Calculate grid
	// Idea for chungus being the variable name accredited to Lucas Kahler...
	// Chungus is x...
	for (auto y = 0; y <= squaresY; ++y) {
		for (auto chungus = 0; chungus <= squaresX; ++chungus) {
			// §4.7/4 from the C++ Standard says (Integral Conversion)
			// If the source type is bool, the value false is converted to zero and the value true is converted to one.
			grid[y * (squaresX + 1) + chungus] = expr->compute(area.left + chungus * squareSide, area.top - y * squareSide) > 0;
			if (flag) {
				sf::Color color = (grid[y * (squaresX + 1) + chungus]) ? sf::Color::Green : sf::Color::Magenta;
				debug.append(sf::Vertex(convertToScreenCoords(bounds, sf::Vector2<double>(area.left + chungus * squareSide, area.top - y * squareSide)), color));
			}
		}
	}

	// Process squares
	for (auto y = 0; y < squaresY; ++y) {
		for (auto x = 0; x < squaresX; ++x) {
			// Indexing top left corner of square
			auto topLeft = grid[y * (squaresX + 1) + x];
			auto topRight = grid[y * (squaresX + 1) + x + 1];
			auto bottomLeft = grid[(y + 1) * (squaresX + 1) + x];
			auto bottomRight = grid[(y + 1) * (squaresX + 1) + x + 1];

			/*
			 * Square worth
			 * 8----4
			 * |	|
			 * |	|
			 * 1----2
			 */

			//				       1			2					4			8
			auto squareType = bottomLeft + bottomRight * 2 + topRight * 4 + topLeft * 8;

			if (squareType != 0 && squareType != 15) {
				// Just doing squares for now, if that ever changes this needs to be generalized for not square
				// TODO: Generalize for not being a square
				// Very low importance
				auto square = sf::Rect<double>(area.left + x * squareSide, area.top - y * squareSide, squareSide, squareSide);

				if (current == max) {
					applyLut(square, squareType);
				} else {
					marchingSquares(square, iterationSquares, max, current + 1);
				}
			}
		}
	}
}

void Computation::Grapher::applyLut(sf::Rect<double> square, int squareType) {
	// The square variable in this function does not have to be a square.
	assert(squareType < 16 && squareType > -1);

	sf::Vector2<double> p1;
	sf::Vector2<double> p2;
	sf::Vector2<double> p3;
	sf::Vector2<double> p4;
	bool twoLines = false;

	// Implement lookup table as seen in
	// https://en.wikipedia.org/wiki/Marching_squares

	// TODO: Implement lerping to smooth out edges
	// Priority: VERY LOW
	switch(squareType) {
		case 14:
			// Case 14 and 1 are identical
		case 1:
			/*
			 * o-----o
			 * |     |
			 * x     | p1
			 * | \   |
			 * o--x--o p2
			 */
			p1.x = square.left;
			p1.y = lerp_n_f(square.top, square.top - square.height, .5f);
			p2.x = lerp_n_f(square.left, square.left + square.width, .5f);
			p2.y = square.top - square.height;
			break;
		case 13:
			// Case 13 and 2 are identical
		case 2:
			/*
			 * o-----o
			 * |     |
			 * |     x p1
			 * |   / |
			 * o--x--o p2
			 */
			p1.x = square.left + square.width;
			p1.y = lerp_n_f(square.top, square.top - square.height, .5f);
			p2.x = lerp_n_f(square.left, square.left + square.width, .5f);
			p2.y = square.top - square.height;
			break;
		case 12:
			// Cases 12 and 3 are identical
		case 3:
			/*
			 * o-----o
			 * |     |
			 * x-----x p1 p2
			 * |     |
			 * o-----o
			 */
			p1.x = square.left;
			p2.x = square.left + square.width;
			p1.y = p2.y = lerp_n_f(square.top, square.top - square.height, .5f);
			break;
		case 11:
			// Cases 11 and 4 are identical
		case 4:
			/*
			 * o--x--o p1
			 * |   \ |
			 * |     x p2
			 * |     |
			 * o-----o
			 */
			p1.x = lerp_n_f(square.left, square.left + square.width, .5f);
			p1.y = square.top;
			p2.x = square.left + square.width;
			p2.y = lerp_n_f(square.top, square.top - square.height, .5f);
			break;
		case 5:
			twoLines = true;
			/*
			 * o--x--o p1
			 * | /   |
			 * x     x p2 p3
			 * |   / |
			 * o--x--o p4
			 */
			p1.x = p4.x = lerp_n_f(square.left, square.left + square.width, .5f);
			p1.y = square.top;
			p2.x = square.left;
			p3.x = square.left + square.width;
			p2.y = p3.y = lerp_n_f(square.top, square.top - square.height, .5f);
			p4.y = square.top - square.height;
			break;
		case 9:
			// Cases 6 and 9 are the same
		case 6:
			/*
			 * o--x--o p1
			 * |  |  |
			 * |  |  |
			 * |  |  |
			 * o--x--o p2
			 */
			p1.x = p2.x = lerp_n_f(square.left, square.left + square.width, .5f);
			p1.y = square.top;
			p2.y = square.top - square.height;
			break;
		case 8:
			// case 8 and 7 are identical
		case 7:
			/*
			 * o--x--o p1
			 * | /   |
			 * x     | p2
			 * |     |
			 * o-----o
			 */
			p1.x = lerp_n_f(square.left, square.left + square.width, .5f);
			p1.y = square.top;
			p2.x = square.left;
			p2.y = lerp_n_f(square.top, square.top - square.height, .5f);
			break;
		case 10:
			twoLines = true;
			/*
			 * o--x--o p1
			 * |   \ |
			 * x     x p3 p2
			 * | \   |
			 * o--x--o p4
			 */
			p1.x = p4.x = lerp_n_f(square.left, square.left + square.width, .5f);
			p1.y = square.top;
			p3.x = square.left;
			p2.x = square.left + square.width;
			p2.y = p3.y = lerp_n_f(square.top, square.top - square.height, .5f);
			p4.y = square.top - square.height;
			break;
		case 0:
			// Do nothing want to return right after, so no break or return here
		case 15:
		default:
			// Return as we are given a case that should never exist
			return;
	}

	// p1 goes with p2 and p3 goes with p4

	// Append vertices for the lines
	points.append(sf::Vertex(convertToScreenCoords(bounds, p1), sf::Color::Red));
	points.append(sf::Vertex(convertToScreenCoords(bounds, p2), sf::Color::Red));
	if (twoLines) {
		points.append(sf::Vertex(convertToScreenCoords(bounds, p3), sf::Color::Red));
		points.append(sf::Vertex(convertToScreenCoords(bounds, p4), sf::Color::Red));
	}
}

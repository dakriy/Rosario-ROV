#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Expression.h"

namespace Computation{
    const unsigned array_count = 1000;

    class Grapher : public sf::Drawable {
        protected:
            Expression * expr = nullptr;

            sf::VertexArray points;
            sf::VertexArray debug;
            bool flag = false;

            sf::Rect<double> bounds;

            GraphingHint type;

            void marchingSquares(sf::Rect<double> area, int squaresX, int max, unsigned current = 1);

            void applyLut(sf::Rect<double> square, int type);

        public:
            Grapher();

            explicit Grapher(sf::Rect<double> bounds, Expression * expr);

            // Copies the passed in expression
            void setExpression(Expression * expression);

            void updateBounds(sf::Rect<double> bounds);

            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

            void update();

            ~Grapher() override;
    };
}

#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Expression.h"
#include "../Interfaces/IUpdateable.h"

namespace Computation{
    const unsigned array_count = 1000;
class Grapher : public sf::Drawable, Interfaces::IUpdateable{
    protected:
        Expression * expr = nullptr;

        sf::VertexArray points;

        sf::Rect<double> bounds;

        GraphingHint type;
        bool boundsChanged = true;
    public:
        Grapher();
        explicit Grapher(sf::Rect<double> bounds, Expression * expr);

        // Copies the passed in expression
        void setExpression(Expression * expression);

        void updateBounds(sf::Rect<double> bounds);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        void update(sf::Time & dt) override;

        ~Grapher() override;
    };
}

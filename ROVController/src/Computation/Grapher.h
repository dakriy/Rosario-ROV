#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Expression.h"

namespace Computation{
    class Grapher : public sf::Drawable{
    protected:
        Expression * expr = nullptr;
    public:
        Grapher();
        explicit Grapher(Expression * expr);

        // Copies the passed in expression
        void setExpression(Expression * expression);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        ~Grapher() override;
    };
}

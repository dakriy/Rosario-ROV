#include "Grapher.h"
#include "../Utilities/Utilities.h"

Computation::Grapher::Grapher() : points(sf::LineStrip, array_count), type(Computation::GraphingHint::Squares) {}

Computation::Grapher::Grapher(sf::Rect<double> bounds, Computation::Expression *expr) : expr(expr), points(sf::LineStrip, array_count), bounds(bounds), type(Computation::GraphingHint::Squares) {}

void Computation::Grapher::draw(sf::RenderTarget &target, sf::RenderStates states) const {

}

void Computation::Grapher::setExpression(Computation::Expression *expression) {
    if (expression) {
        expr = new Expression(*expression);
        type = expr->graphingSuggestion();
    }
}

Computation::Grapher::~Grapher() {
    delete expr;
}

void Computation::Grapher::update(const sf::Time &dt) {
    if (expr && expr->graphingSuggestion() == Computation::GraphingHint::Horizontal)
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
}

void Computation::Grapher::updateBounds(sf::Rect<double> newBounds) {
    bounds = newBounds;
}

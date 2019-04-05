#include <imgui.h>
#include "Grapher.h"
#include "../Utilities/Utilities.h"

Computation::Grapher::Grapher() : points(sf::LineStrip, array_count), type(Computation::GraphingHint::Squares) {}

Computation::Grapher::Grapher(sf::Rect<double> bounds, Computation::Expression *expr) : expr(expr), points(sf::LineStrip, array_count), bounds(bounds) {
    type = expr->graphingSuggestion();
}

void Computation::Grapher::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (expr)
    {
        if (type == Computation::GraphingHint::Horizontal || type == Computation::GraphingHint::Vertical) {
            target.draw(points);
        }
    }
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

void Computation::Grapher::update() {
    if (expr)
    {
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

        static int squaresX = 1;

        ImGui::Begin("Marching Squares");
        ImGui::SliderInt("Squares along the x axis", &squaresX, 1, 500);
        ImGui::End();


        if (type == Computation::GraphingHint::Squares)
        {

        }

    }
}

void Computation::Grapher::updateBounds(sf::Rect<double> newBounds) {
    bounds = newBounds;
}

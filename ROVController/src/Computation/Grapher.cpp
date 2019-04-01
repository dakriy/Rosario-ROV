#include "Grapher.h"

Computation::Grapher::Grapher() {}

Computation::Grapher::Grapher(Computation::Expression *expr) : expr(expr) {}

void Computation::Grapher::draw(sf::RenderTarget &target, sf::RenderStates states) const {

}

void Computation::Grapher::setExpression(Computation::Expression *expression) {
    if (expression) {
        expr = new Expression(expression);
    }
}

Computation::Grapher::~Grapher() {
    delete expr;
}

#include "Expression.h"

Computation::Expression::~Expression() {
    delete head;
}

double Computation::Expression::compute(double x, double y) {
    return head->compute(x, y);
}

void Computation::Expression::setRhs(Computation::Token *r) {
    head->setRight(r);
}

void Computation::Expression::setLhs(Computation::Token *l) {
    head->setLeft(l);
}

Computation::Expression::Expression(bool equality, Computation::Token *lhs, Computation::Token *rhs) : equation(equality) {
    head = new Token(Operator::Subtract, lhs, rhs);
}

Computation::Token *Computation::Expression::getHead() {
    return head;
}

Computation::GraphingHint Computation::Expression::graphingSuggestion() {

    if (!head || !head->getRight() || !head->getLeft())
    {
        // No equals sign because stuff is null meaning we were not fully initialized.
        return GraphingHint::Constant;
    }

    // constant if rhs is 0 or null and left is not
    // Don't want to get in here for something like x+y = 0
    if (countType(head, TokenType::Y) == 0 && countType(head, TokenType::X) == 0)
    {
        // Constant means no equals sign, so not an equality
        return GraphingHint::Constant;
    }

    // Can do horizontal scan graphing if in either of these configurations:
    // y = any expression without a y
    // any expression without a y = y
    if ((head->getRight()->getType() == TokenType::Y && countType(head->getLeft(), TokenType::Y) == 0) ||
        (head->getLeft()->getType() == TokenType::Y && countType(head->getRight(), TokenType::Y) == 0))
    {
        return GraphingHint::Horizontal;
    }

    // Can do vertical scan graphing if in either of these configurations:
    // x = any expression without a x
    // any expression without a x = x
    if ((head->getRight()->getType() == TokenType::X && countType(head->getLeft(), TokenType::X) == 0) ||
        (head->getLeft()->getType() == TokenType::X && countType(head->getRight(), TokenType::X) == 0))
    {
        return GraphingHint::Vertical;
    }

    // Otherwise we need to do marching squares
    return GraphingHint::Squares;
}

unsigned Computation::Expression::countType(Computation::Token *h, TokenType type) {
    // Traversal order is not a big deal, so we'll just do pre order.

    if (h == nullptr)
        return 0;

    unsigned count = 0;
    if (h->getType() == type)
        ++count;

    count += countType(h->getLeft(), type);
    count += countType(h->getRight(), type);
    return count;
}

Computation::Expression::Expression(Computation::Expression & expression) : equation(expression.equation) {
    if (expression.head)
        head = new Token(*expression.getHead());
}

bool Computation::Expression::isEquation() {
    return equation;
}

void Computation::Expression::setEquation(bool eq) {
    equation = eq;
}

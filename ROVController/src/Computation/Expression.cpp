#include "Expression.h"

Computation::Expression::~Expression() {
    delete head;
}

double Computation::Expression::compute(double x, double y) {
    if (type == GraphingHint::Constant)
    {
        return head->getLeft()->compute(x, y);
    }
    return head->compute(x, y);
}

void Computation::Expression::setRhs(Computation::Token *r) {
    head->setRight(r);
    resolveGraphingType();
}

void Computation::Expression::setLhs(Computation::Token *l) {
    head->setLeft(l);
    resolveGraphingType();
}

Computation::Expression::Expression(Computation::Token *lhs, Computation::Token *rhs) {
    head = new Token(Operator::Subtract, lhs, rhs);
    resolveGraphingType();
}

Computation::Token *Computation::Expression::getHead() {
    return head;
}

Computation::GraphingHint Computation::Expression::grpahingSuggestion() {
    return type;
}

void Computation::Expression::resolveGraphingType() {
    // constant if rhs is 0 or null and left is not

    if (head->getRight() && head->getRight()->isZero())
    {
        type = GraphingHint::Constant;
        return;
    }

    // TODO: Traverse tree and count x's and y's
    type = GraphingHint::Squares;
}


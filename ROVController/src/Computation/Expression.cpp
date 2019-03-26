#include "Expression.h"

void Computation::Expression::normalize() {
    if (!head)
        return;
    auto sub = new Token(Operator::Subtract, head->getLeft(), head->getRight());
    head->setLeft(sub);
    // Nullptr means value of 0
    head->setRight(nullptr);
}

Computation::Expression::~Expression() {
    delete head;
}

void Computation::Expression::setHead(Token *newHead) {
    newHead->setLeft(head);
    head = newHead;
}

double Computation::Expression::compute(double x, double y) {
    return head->compute(x, y);
}

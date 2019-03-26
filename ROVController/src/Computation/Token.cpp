#include "Token.h"
#include "../Utilities/Utilities.h"
#include <algorithm>
#include <cmath>

double Computation::Token::compute(double x, double y) {
    switch (t) {
        case Computation::TokenType::Operator:
            return operate(x, y);
        case Computation::Function:
            return exec(x, y);
        case Computation::TokenType::Number:
            return v.n;
        case Computation::TokenType::X:
            return x;
        case Computation::TokenType::Y:
            return y;
    }
}


Computation::TokenType Computation::Token::getType() {
    return t;
}

void Computation::Token::setOperator(Operator o) {
    t = Computation::TokenType::Operator;
    v.o = o;
}

void Computation::Token::setNumber(double n) {
    t = Computation::TokenType::Number;
    v.n = n;
}

void Computation::Token::setLeft(Token * l) {
    left = l;
}

void Computation::Token::setRight(Token * r) {
    right = r;
}

double Computation::Token::operate(double x, double y) {
    double lval = 0;
    double rval = 0;
    // Nullptr means value of 0
    if (left)
        lval = left->compute(x, y);
    if (right)
        rval = right->compute(x, y);
    switch (v.o){
        case Operator::Add:
            return lval + rval;
        case Operator::Subtract:
            return lval - rval;
        case Operator::Divide:
            return lval / rval;
        case Operator::Multiply:
            return lval * rval;
        case Operator::Power:
            return pow(lval, rval);
        case Operator::Modulus:
            return fmod(lval, rval);
    }
}


double Computation::Token::exec(double x, double y) {
    double lval = 0;
    double rval = 0;
    // Nullptr means value of 0
    if (left)
        lval = left->compute(x, y);
    if (right)
        rval = right->compute(x, y);
    switch (v.f) {
        case Function::Logarithm:
            // log left (right)
            // So left is base
            // Right is number
            return log10(rval) / log10(lval);
        case Function::Sin:
            return sin(lval);
        case Function::Cos:
            return cos(lval);
        case Function::Tan:
            return tan(lval);
        case Function::Abs:
            return abs(lval);
        case Function::Exp:
            return exp(lval);
        case Function::Ln:
            return log(lval);
        case Function::Log10:
            return log10(lval);
        case Function::Sqrt:
            return sqrt(lval);
        case Function::Pi:
            return PI;
        case Function::E:
            return E;
        case Function::Step:
            return (lval >= 0) ? 1. : 0.;
        case Function::Max:
            return max(lval, rval);
        case Function::Min:
            return min(lval, rval);
        case Function::Sign:
            return sgn(lval);
    }
}

Computation::Token::~Token() {
    delete left;
    delete right;
}

Computation::Token *Computation::Token::getLeft() {
    return left;
}

Computation::Token *Computation::Token::getRight() {
    return right;
}

void Computation::Token::setY() {
    t = Computation::TokenType::Y;
}

void Computation::Token::setX() {
    t = Computation::TokenType::X;
}

Computation::Token::Token(Computation::Token *l, Computation::Token *r) : left(l), right(r) {}

Computation::Token::Token(double n, Computation::Token *l, Computation::Token *r) : left(l), right(r), t(Computation::TokenType::Number) {
    v.n = n;
}

Computation::Token::Token(Computation::TokenType type, Computation::Token *l, Computation::Token *r) : left(l), right(r), t(type) {}

Computation::Token::Token(Computation::Operator o, Computation::Token *l, Computation::Token *r) : left(l), right(r), t(Computation::TokenType::Operator) {
    v.o = o;
}

void Computation::Token::setFunction(Computation::Function f) {
    t = Computation::TokenType::Function;
    v.f = f;
}

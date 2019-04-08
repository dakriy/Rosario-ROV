#pragma once

#include "Token.h"

namespace Computation {

    enum class GraphingHint {
        Vertical,
        Horizontal,
        Squares,
        Constant
    };

    class Expression {
    protected:
        Token *head = nullptr;
        bool equation = false;
    public:
        Expression(Expression & expression);
        explicit Expression(bool equality = false, Token * lhs = nullptr, Token * rhs = nullptr);
        double compute(double x, double y);
        void setLhs(Token * l);
        void setRhs(Token * r);
        void setEquation(bool eq);
        bool isEquation();
        Token * getHead();
		unsigned countType(Token *h, TokenType type);
        GraphingHint graphingSuggestion();
        ~Expression();
    };
}

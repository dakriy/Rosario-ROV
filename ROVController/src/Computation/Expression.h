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
    public:
        explicit Expression(Token * lhs = nullptr, Token * rhs = nullptr);
        double compute(double x, double y);
        void setLhs(Token * l);
        void setRhs(Token * r);
        Token * getHead();
		unsigned countType(Token *h, TokenType type);
        GraphingHint graphingSuggestion();
        ~Expression();
    };
}

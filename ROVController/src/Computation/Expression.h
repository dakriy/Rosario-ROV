#pragma once

#include "Token.h"

namespace Computation {
    class Expression {
    protected:
        Token * head = nullptr;
        void normalize();
    public:
        double compute(double x, double y);
        void setHead(Token * newHead);
        ~Expression();
    };
}

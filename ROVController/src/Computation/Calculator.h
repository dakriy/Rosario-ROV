#pragma once
#include "Token.h"
#include "Expression.h"
#include <array>
#include <unordered_map>
#include <stack>
#include <vector>

// TODO: https://en.wikipedia.org/wiki/Shunting-yard_algorithm

namespace Computation {
    struct OpInfo {
        enum associativity {
            Right,
            Left
        };

        Operator op;
        int precedence;
        associativity ass;

        OpInfo() = default;
        explicit OpInfo(Operator op);
    };

    struct ParseToken {
        enum type {
            op,
            fn,
            // left parantheses
            lp,
            // right parantheses
            rp,
            x,
            y,
            n
        };

        union tok {
            OpInfo op;
            Function f;
            double n;
        };

        tok token;
        type t;
    };

    class Calculator {
    protected:
        std::unordered_map<Operator, char> OperatorMap;
        std::unordered_map<Function, const char *> FunctionMap;
        std::stack<ParseToken> operatorStack;
        const char * expression;
        int bufferLen;
        // Iterator is left at the start of what we want to parse next
        const char * iterator;
        bool noMoreTokens = false;
        ParseToken getNextToken();
    public:
        Calculator(const char * expr, unsigned len);
        Expression parse();
    };
}

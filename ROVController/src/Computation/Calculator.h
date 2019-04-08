#pragma once
#include "Token.h"
#include "Expression.h"
#include <array>
#include <unordered_map>
#include <stack>
#include <vector>
#include <queue>

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
            // Operator
            op,
            // Function
            fn,
            // left parentheses
            lp,
            // right parentheses
            rp,
            x,
            y,
            n,
            comma
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
        std::stack<ParseToken> outputQueue;

        const char * expression = nullptr;
        // Iterator is left at the start of what we want to parse next
        const char * iterator = nullptr;
        bool noMoreTokens = false;
        ParseToken getNextToken();
        void advanceIterator();

        // initally to something we don't care about
        ParseToken::type lastTokenType = ParseToken::comma;
    public:
        Calculator();
        Expression* parse(const char * expr);
    };
}

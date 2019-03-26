#include "Calculator.h"
#include "../Utilities/Utilities.h"

Computation::Expression Computation::Calculator::parse() {
    // Implemetning the shunting yard algorithm into an Abstract Syntax Tree
    // https://en.wikipedia.org/wiki/Shunting-yard_algorithm



    return Computation::Expression();
}

Computation::Calculator::Calculator(const char *expr, unsigned len) : expression(expr), bufferLen(len), iterator(expr) {
    // Setup the token map
    map_init(OperatorMap)
            (Computation::Operator::Add, '+')
            (Computation::Operator::Subtract, '-')
            (Computation::Operator::Divide, '/')
            (Computation::Operator::Multiply, '*')
            (Computation::Operator::Power, '^')
            (Computation::Operator::Modulus, '%');

    map_init(FunctionMap)
            (Computation::Function::Logarithm, "log")
            (Computation::Function::Sin, "sin")
            (Computation::Function::Cos, "cos")
            (Computation::Function::Tan, "tan")
            (Computation::Function::Abs, "abs")
            (Computation::Function::Exp, "exp")
            (Computation::Function::Ln, "ln")
            (Computation::Function::Log10, "log10")
            (Computation::Function::Sqrt, "sqrt")
            (Computation::Function::Pi, "pi")
            (Computation::Function::E, "e")
            (Computation::Function::Step, "u")
            (Computation::Function::Max, "max")
            (Computation::Function::Min, "min")
            (Computation::Function::Sign, "sgn");

}

Computation::ParseToken Computation::Calculator::getNextToken() {

    // Advance iterator to next token
    while(*iterator == ' ') iterator++;

    // First check to see if the next token is an operator or parenthesis

    // Check parenthesis
    if (*iterator == '(')
    {
        ParseToken token;
        token.t = ParseToken::lp;
        ++iterator;
        return token;
    }

    if (*iterator == ')')
    {
        ParseToken token;
        token.t = ParseToken::rp;
        return token;
    }

    // Check to see if it's an operator
    for (const auto& [i, thing] : OperatorMap)
    {
        if (thing == *iterator)
        {
            ParseToken token;
            token.t = ParseToken::op;
            OpInfo o(i);
            token.token.op = o;
            ++iterator;
            return token;
        }
    }

    // Next check for variable x or y
    if (*iterator == 'x')
    {
        ParseToken token;
        token.t = ParseToken::x;
        ++iterator;
        return token;
    }

    if (*iterator == 'y')
    {
        ParseToken token;
        token.t = ParseToken::y;
        ++iterator;
        return token;
    }

    // At this point it is known that the next token is not an operator or a parenthesis, and it is not an x or a y
    // Check for number
    if (*iterator == '.' || isdigit(*iterator))
    {
        ParseToken token;
        // TODO: use strod() here
        token.token.n = atof(iterator);
        token.t = ParseToken::n;
        ++iterator;
        return token;
    }

    // Must be a function
    // TODO: Get next function token
    // TODO: Put scoping checks here
    int it;
    while (isalnum(*(iterator + it))) it++;

    if (it > 9)
    {
        // Problem here, no functions larger than 9
        // TODO: Throw parse error
    }

    // Copy out a substring
    char subbuff[10];
    memcpy( subbuff, iterator, static_cast<size_t>(it));
    subbuff[it] = '\0';

    // Check all the functions
    for (const auto& [i, thing] : FunctionMap)
    {
        if (strcmp(subbuff, thing) == 0)
        {
            ParseToken token;
            token.t = ParseToken::fn;
            token.token.f = i;
            iterator += it;
            return token;
        }
    }

    // Ok, it's not a known function or operator or anything HALP
    // TODO: Handle errors here
}

Computation::OpInfo::OpInfo(Computation::Operator op) {
    switch (op) {
        case Operator::Add:
            ass = Left;
            precedence = 2;
            break;
        case Operator::Subtract:
            ass = Left;
            precedence = 2;
            break;
        case Operator::Divide:
            ass = Left;
            precedence = 3;
            break;
        case Operator::Multiply:
            ass = Left;
            precedence = 3;
            break;
        case Operator::Power:
            ass = Right;
            precedence = 4;
            break;
        case Operator::Modulus:
            ass = Right;
            precedence = 4;
            break;
    }
}

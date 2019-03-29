#include "Calculator.h"
#include "../Utilities/Utilities.h"
#include <cstdlib>

Computation::Expression * Computation::Calculator::parse(const char * expr) {
    expression = iterator = expr;
    lastTokenType = ParseToken::comma;
	noMoreTokens = false;

    // Empty any queues if somehow there's something in them
    while(!outputQueue.empty())
        outputQueue.pop();
    while(!operatorStack.empty())
        operatorStack.pop();


    // Implemetning the shunting yard algorithm into an Abstract Syntax Tree
    // https://en.wikipedia.org/wiki/Shunting-yard_algorithm
    while (!noMoreTokens) {
        auto token = getNextToken();
        switch (token.t) {
            case ParseToken::op:
            {
                // while ((there is a function at the top of the operator stack) or
				if (!operatorStack.empty()) {
					auto nextElem = operatorStack.top();
					while (((nextElem.t == ParseToken::fn) ||
						// (there is an operator at the top of the operator stack with greater precedence) or
						(nextElem.t == ParseToken::op && nextElem.token.op.precedence > token.token.op.precedence) ||
						// (the operator at the top of the operator stack has equal precedence and is left associative)) and
						(nextElem.t == ParseToken::op && nextElem.token.op.precedence == token.token.op.precedence && nextElem.token.op.ass == OpInfo::Left)) &&
						// (the operator at the top of the operator stack is not a left parenthesis):
						(nextElem.t != ParseToken::lp)
						)
					{
						// pop operators from the operator stack onto the output queue.
						outputQueue.push(nextElem);
						operatorStack.pop();
						if (operatorStack.empty())
							break;
						nextElem = operatorStack.top();
					}
				}
				// Then push it onto the operator stack
				operatorStack.push(token);
                break;
            }
            case ParseToken::fn:
                operatorStack.push(token);
                break;
            case ParseToken::lp:
                operatorStack.push(token);
                break;
            case ParseToken::rp:
            {
                // while the operator at the top of the operator stack is not a left paren:
                while (!operatorStack.empty() && operatorStack.top().t != ParseToken::lp) {

                    // pop the operator from the operator stack onto the output queue.
                    outputQueue.push(operatorStack.top());
                    operatorStack.pop();
                }
				// if the stack runs out without finding a left paren, then there are mismatched parentheses.
				if (operatorStack.empty())
				{
					throw "Mismatched parentheses!";
				}
                // Has to be left parenthesis at the top of the stack if we get here
                // Discard the left parenthesis
                operatorStack.pop();
                break;
            }
            case ParseToken::x:
                // X is the same as a number
                outputQueue.push(token);
                break;
            case ParseToken::y:
                // Y is the same as a number
                outputQueue.push(token);
                break;
            case ParseToken::n:
                outputQueue.push(token);
                break;
            case ParseToken::comma:
                // Ignore commas
                break;
        }
        lastTokenType = token.t;
    }

	// No more tokens to read
	while (!operatorStack.empty()) {
	    auto top = operatorStack.top();
		// if the operator token on the top of the stack is a paren, then there are mismatched parentheses
	    if (top.t == ParseToken::lp || top.t == ParseToken::rp)
        {
            throw "Mismatched parentheses!";
        }
        outputQueue.push(top);
        operatorStack.pop();
	}


    Token *left = nullptr, *right = nullptr;

    if (!outputQueue.empty())
    {
        auto top = outputQueue.top();
        // Check to see if it is an equality
        if (!(top.t == ParseToken::op && top.token.op.op == Operator::Equals))
        {
            // Not an equality
            // Set right hand value in the expression to be 0 so nothing is subtracted when it is evaluated
            // Tokens default to be 0
            right = new Token;
        }
    }

    auto ex = new Expression(left, right);


    while(!outputQueue.empty())
    {
        auto top = outputQueue.top();
        if (top.t == ParseToken::op && top.token.op.op == Operator::Equals)
        {
            throw "Equals in wrong position, or multiple equals signs.";
        }
        Token * token;
        switch (top.t) {
            case ParseToken::op:
                token = new Token(top.token.op.op);
                break;
            case ParseToken::fn:
                token = new Token(top.token.f);
                break;
            case ParseToken::x:
                token = new Token();
                token->setX();
                break;
            case ParseToken::y:
                token = new Token();
                token->setY();
                break;
            case ParseToken::n:
                token = new Token(top.token.n);
                break;
            default: // Shouldn't have anything else otherwise we'll have problems
                throw "Something went wrong when parsing your expression. Not sure what. There is a leftover parenthesis or comma though.";
        }

        if (!ex->getHead()->addToken(token)) {
            throw "Syntax error!";
        }

        outputQueue.pop();
    }

    return ex;
}

Computation::Calculator::Calculator() {
    // Setup the token map
    map_init(OperatorMap)
            (Computation::Operator::Add, '+')
            (Computation::Operator::Subtract, '-')
            (Computation::Operator::Divide, '/')
            (Computation::Operator::Multiply, '*')
            (Computation::Operator::Power, '^')
            (Computation::Operator::Modulus, '%')
            (Computation::Operator::Equals, '=')
            (Computation::Operator::Negate, '_');

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
    // Advance iterator to next token if needed
    while(*iterator == ' ') ++iterator;

    // First check to see if the next token is an operator, parenthesis, or comma

    // Check parenthesis
    if (*iterator == '(')
    {
        ParseToken token;
        token.t = ParseToken::lp;
        advanceIterator();
        return token;
    }

    if (*iterator == ')')
    {
        ParseToken token;
        token.t = ParseToken::rp;
        advanceIterator();
        return token;
    }

    if (*iterator == ',')
    {
        ParseToken token;
        token.t = ParseToken::comma;
        advanceIterator();
        return token;
    }

    // Check to see if it's an operator
    for (const auto& [i, thing] : OperatorMap)
    {
        if (thing == *iterator)
        {
            ParseToken token;
            token.t = ParseToken::op;

            // A negation shows up after an operator or a left parenthesis, or a comma
            if (*iterator == OperatorMap[Operator::Subtract] && (lastTokenType == ParseToken::op || lastTokenType == ParseToken::lp || lastTokenType == ParseToken::comma))
            {
                token.token.op = OpInfo(Operator::Negate);
            } else {
                token.token.op = OpInfo(i);
            }
            advanceIterator();
            return token;
        }
    }

    // Next check for variable x or y
    if (*iterator == 'x')
    {
        ParseToken token;
        token.t = ParseToken::x;
        advanceIterator();
        return token;
    }

    if (*iterator == 'y')
    {
        ParseToken token;
        token.t = ParseToken::y;
        advanceIterator();
        return token;
    }

    // At this point it is known that the next token is not an operator or a parenthesis, and it is not an x or a y
    // Check for number
    if (*iterator == '.' || isdigit(*iterator))
    {
        ParseToken token;
        char * dEnd;
        token.token.n = std::strtod(iterator, &dEnd);
        token.t = ParseToken::n;
        // Need to reverse iterator once, because advanceIterator puts it forward again.
        iterator = --dEnd;
        advanceIterator();
        return token;
    }

    // Must be a function
    int it = 0;
    while (isalnum(*(iterator + it))) {
        ++it;
        if (it > 9)
        {
            // Problem here, no functions larger than 9
            throw "Unknown function or variable";
        }
    }

    // Copy out a substring
    char subbuff[10];
    memcpy(subbuff, iterator, static_cast<size_t>(it));
    subbuff[it] = '\0';

    // Check all the functions
    for (const auto& [i, thing] : FunctionMap)
    {
        if (strcmp(subbuff, thing) == 0)
        {
            ParseToken token;
            token.t = ParseToken::fn;
            token.token.f = i;
            // Need minus 1 because we iterate it once already in advanceIterator()
            iterator += it - 1;
            advanceIterator();
            return token;
        }
    }

    // Ok, it's not a known function or operator or anything HALP
    throw "Unknown function";
}

void Computation::Calculator::advanceIterator() {
    if (noMoreTokens)
        return;

    // Increment once for next round;
    ++iterator;

    // Advance iterator to next token
    while(*iterator == ' ') ++iterator;

    if (!*iterator)
    {
        noMoreTokens = true;
        return;
    }
}

Computation::OpInfo::OpInfo(Computation::Operator op) : op(op) {
    switch (op) {
        case Operator::Add:
            ass = Left;
            precedence = 5;
            break;
        case Operator::Subtract:
            ass = Left;
            precedence = 5;
            break;
        case Operator::Divide:
            ass = Left;
            precedence = 8;
            break;
        case Operator::Multiply:
            ass = Left;
            precedence = 8;
            break;
        case Operator::Power:
            ass = Right;
            precedence = 9;
            break;
        case Operator::Modulus:
            ass = Left;
            precedence = 8;
            break;
        case Operator::Equals:
            ass = Left;
            precedence = 0;
            break;
        case Operator::Negate:
            ass = Right;
            precedence = 9;
            break;
    }
}

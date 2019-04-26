#include "Token.h"
#include "../Utilities/Utilities.h"
#include <algorithm>
#include <cmath>

double Computation::Token::compute(double x, double y) {
	switch (t) {
		case Computation::TokenType::Operator:
			return operate(x, y);
		case Computation::TokenType::Function:
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

void Computation::Token::setFunction(Computation::Function f) {
	t = Computation::TokenType::Function;
	v.f = f;
}

void Computation::Token::setLeft(Token * l) {
	delete left;
	left = l;
}

void Computation::Token::setRight(Token * r) {
	delete right;
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
		case Operator::Equals:
			return abs(lval - rval) < zero;
		case Operator::Negate:
			return -lval;
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
			return fmax(lval, rval);
		case Function::Min:
			return fmin(lval, rval);
		case Function::Sign:
			return sgn(lval);
		case Function::Floor:
			return floor(lval);
		case Function::Ceil:
			return ceil(lval);
		case Function::Atan:
			return atan(lval);
		case Function::Acos:
			return acos(lval);
		case Function::Asin:
			return asin(lval);
		case Function::Cosh:
			return cosh(lval);
		case Function::Tanh:
			return tanh(lval);
		case Function::Sinh:
			return sinh(lval);
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

Computation::Token::Token() {
	setNumber(0.);
}

Computation::Token::Token(double n, Computation::Token *l, Computation::Token *r) : left(l), right(r) {
	setNumber(n);
}

Computation::Token::Token(Computation::Operator o, Computation::Token *l, Computation::Token *r) : left(l), right(r) {
	setOperator(o);
	v.o = o;
}

Computation::Token::Token(Computation::Function f, Computation::Token *l, Computation::Token *r) : left(l), right(r) {
	setFunction(f);
}

bool Computation::Token::addToken(Computation::Token *nt) {
	// Construct tree starting from right assuming we are receiving PN so we can build tree top down

	// Leveraging short circuit evaluation. I hope...
	// No room to add it anywhere, so just go down the chain.
	if (right && left)
		return right->addToken(nt) || left->addToken(nt);

	// Cannot add if we are a number with no arguments
	if (t == TokenType::Number || t == TokenType::X || t == TokenType::Y)
		return false;

	// At this point, there is room to add it as a child, and we know we're not a number

	// Check to see if we are a function or operator that only takes 1 argument
	if (t == TokenType::Function)
	{
		switch (v.f){
			// Functions that take 2 arguments
			case Function::Logarithm: // No break
			case Function::Max: // No break
			case Function::Min: // These functions are the only ones that two arguments
				break;
			case Function::Pi: // No break
			case Function::E: // These functions are the only ones that take no arguments so they cannot have children
				return false;
			default: // Most functions take one argument
				if (!left)
				{
					left = nt;
					return true;
				}
				return left->addToken(nt);
		}
	}

	if (t == TokenType::Operator)
	{
		switch(v.o) {
			case Operator::Negate:
				// Do we already have a token there?
				if (!left) // No, add it and say that we added it.
				{
					left = nt;
					return true;
				}
				// If something has been added send the request down the chain
				return left->addToken(nt);
			default: // Everything has 2 arugments by default
				break;
		}
	}

	// At this point we know there is room and that we take 2 arguments, so find the place and add it.

	// Try to add to the right first as we are processing polish notation
	if (!right) {
		right = nt;
		return true;
	}
	if (right->addToken(nt))
		return true;
	if (!left) {
		left = nt;
		return true;
	}

	throw "Points that don't exist do now...";
}

bool Computation::Token::isZero() const {
	return t == TokenType::Number && abs(v.n) < zero;
}

Computation::Token::Token(Computation::Token &token) : t(token.t), v(token.v) {
	deepCopyNode(token.right, right);
	deepCopyNode(token.left, left);
}

void Computation::Token::deepCopyNode(Computation::Token *original, Computation::Token *&copyTo) {
	if (original == nullptr) {
		copyTo = nullptr;
	} else {
		copyTo = new Token();
		copyTo->t = original->t;
		copyTo->v = original->v;
		deepCopyNode(original->right, copyTo->right);
		deepCopyNode(original->left, copyTo->left);
	}
}

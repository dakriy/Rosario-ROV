#pragma once
namespace Computation {
    enum class TokenType {
        Operator,
        Function,
        Number,
        X,
        Y
    };

    enum class Operator {
        Add,
        Subtract,
        Divide,
        Multiply,
        Power,
        Modulus,
        Equals,
        Negate
    };

    enum class Function {
        Logarithm,
        Sin,
        Cos,
        Tan,
        Abs,
        Exp,
        Ln,
        Log10,
        Sqrt,
        Pi,
        E,
        Step,
        Max,
        Min,
        Sign,
        Floor,
        Ceil,
        Atan,
        Acos,
        Asin,
        Cosh,
        Tanh,
        Sinh
    };

    class Token {
    protected:
        static constexpr auto zero = 0.0000001;

        union value {
            Operator o;
            Function f;
            double n;
        };

        value v;

        double operate(double x, double y);
        double exec(double x, double y);

        TokenType t;

        Token * left = nullptr;
        Token * right = nullptr;
    public:
        Token();
        explicit Token(double n, Token *l = nullptr, Token *r = nullptr);
        explicit Token(Operator o, Token *l = nullptr, Token *r = nullptr);
        explicit Token(Function f, Token *l = nullptr, Token *r = nullptr);
        double compute(double x, double y);
        TokenType getType();
        void setOperator(Operator o);
        void setFunction(Function f);
        void setNumber(double n);
        void setX();
        void setY();
        void setLeft(Token * l);
        void setRight(Token * r);
        bool isZero();
        bool addToken(Token * t);
        Token * getLeft();
        Token * getRight();
        ~Token();
    };
}

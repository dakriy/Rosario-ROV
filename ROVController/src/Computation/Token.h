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
        static constexpr auto zero = 0.000000001;

        union value {
            Operator o;
            Function f;
            double n;
        };

        value v;

        TokenType t;

        Token * left = nullptr;
        Token * right = nullptr;

        double operate(double x, double y);
        double exec(double x, double y);
    public:
        Token();
        // Performs deep copy
        Token(Token &token);
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
        bool isZero() const;
        bool addToken(Token * t);
        static void deepCopyNode(Token * original, Token * &copyTo);
        Token * getLeft();
        Token * getRight();
        ~Token();
    };
}

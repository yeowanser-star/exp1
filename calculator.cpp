#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cctype>
#include <stdexcept>
#include <functional>
#include <map>
#include <iomanip>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define N_OPTR 9 
typedef enum {ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE} Operator; 

const char operChar[N_OPTR] = {'+', '-', '*', '/', '^', '!', '(', ')', '\0'};

const char pri[N_OPTR][N_OPTR] = {
    /*              |-------------- 当前运算符 --------------| */
    /*              +    -    *    /    ^    !    (    )   \0 */
    /* -- + */    {'>', '>', '<', '<', '<', '<', '<', '>', '>'},
    /* | - */     {'>', '>', '<', '<', '<', '<', '<', '>', '>'},
    /* 栈 * */    {'>', '>', '>', '>', '<', '<', '<', '>', '>'},
    /* 顶 / */    {'>', '>', '>', '>', '<', '<', '<', '>', '>'},
    /* 运 ^ */    {'>', '>', '>', '>', '>', '<', '<', '>', '>'},
    /* 算 ! */    {'>', '>', '>', '>', '>', '>', ' ', '>', '>'},
    /* 符 ( */    {'<', '<', '<', '<', '<', '<', '<', '=', ' '},
    /* | ) */     {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    /* -- \0 */   {'<', '<', '<', '<', '<', '<', '<', ' ', '='}
};

Operator char2optr(char c) {
    for (int i = 0; i < N_OPTR; i++) {
        if (c == operChar[i]) {
            return (Operator)i;
        }
    }
    return EOE; 
}

template<typename T>
class Stack {
private:
    std::vector<T> data;
    
public:
    Stack() {}
    
    int size() const {
        return data.size();
    }
    
    bool empty() const {
        return data.empty();
    }
    
    T& top() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return data.back();
    }
    
    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return data.back();
    }
    
    // 入栈
    void push(const T& element) {
        data.push_back(element);
    }
    
    // 出栈
    void pop() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        data.pop_back();
    }
    
    // 清空栈
    void clear() {
        data.clear();
    }
};

// 阶乘函数
double factorial(double n) {
    if (n < 0 || n != (int)n) {
        throw std::runtime_error("Factorial only defined for non-negative integers");
    }
    if (n == 0 || n == 1) {
        return 1;
    }
    double result = 1;
    for (int i = 2; i <= (int)n; i++) {
        result *= i;
    }
    return result;
}

// 执行二元运算
double calculate(double a, Operator op, double b) {
    switch (op) {
        case ADD: return a + b;
        case SUB: return a - b;
        case MUL: return a * b;
        case DIV: 
            if (b == 0) {
                throw std::runtime_error("Division by zero");
            }
            return a / b;
        case POW: return pow(a, b);
        default:
            throw std::runtime_error("Invalid binary operation");
    }
}

// 执行一元运算（阶乘）
double calculate(Operator op, double a) {
    if (op == FAC) {
        return factorial(a);
    }
    throw std::runtime_error("Invalid unary operation");
}

bool isDigit(char c) {
    return std::isdigit(c) || c == '.';
}

char getPriority(Operator op1, Operator op2) {
    return pri[op1][op2];
}

std::pair<double, int> getNextNumber(const std::string& expr, int start) {
    std::string numStr = "";
    int i = start;
    
    // 处理负号
    if (i < expr.length() && expr[i] == '-' && 
        (i == 1 || expr[i-1] == '(' || expr[i-1] == '+' || expr[i-1] == '-' || 
         expr[i-1] == '*' || expr[i-1] == '/' || expr[i-1] == '^')) {
        numStr += expr[i];
        i++;
    }
    
    // 收集数字字符（包括小数点）
    while (i < expr.length() && (isDigit(expr[i]) || expr[i] == '.')) {
        numStr += expr[i];
        i++;
    }
    
    if (numStr.empty()) {
        throw std::runtime_error("Invalid number format");
    }
    
    try {
        double num = std::stod(numStr);
        return std::make_pair(num, i);
    } catch (...) {
        throw std::runtime_error("Invalid number format");
    }
}

double evaluateBasicExpression(const std::string& expression) {
    if (expression.empty()) {
        return 0;
    }
    
    std::string expr = '\0' + expression + '\0';
    
    Stack<double> operandStack; 
    Stack<Operator> operatorStack; 
    operatorStack.push(char2optr('\0')); 
    
    int i = 1; 
    
    while (i < expr.length()) {
        if (isDigit(expr[i]) || 
            (expr[i] == '-' && (i == 1 || expr[i-1] == '\0' || expr[i-1] == '(' || 
                                expr[i-1] == '+' || expr[i-1] == '-' || 
                                expr[i-1] == '*' || expr[i-1] == '/' || expr[i-1] == '^'))) {
       
            auto numInfo = getNextNumber(expr, i);
            operandStack.push(numInfo.first);
            i = numInfo.second;
        } else {
        
            Operator currOp = char2optr(expr[i]);
            
            if (currOp == EOE) {
                break;
            }
            
            switch (getPriority(operatorStack.top(), currOp)) {
                case '<': 
                    operatorStack.push(currOp);
                    i++;
                    break;
                    
                case '=': 
                    operatorStack.pop(); 
                    i++;
                    break;
                    
                case '>': 
                {
                    Operator op = operatorStack.top();
                    operatorStack.pop();
                    
                    if (op == FAC) {
                        if (operandStack.empty()) {
                            throw std::runtime_error("Invalid factorial operation");
                        }
                        double operand = operandStack.top();
                        operandStack.pop();
                        double result = calculate(op, operand);
                        operandStack.push(result);
                    } else {
                        if (operandStack.size() < 2) {
                            throw std::runtime_error("Invalid expression: not enough operands");
                        }
                        double b = operandStack.top();
                        operandStack.pop();
                        double a = operandStack.top();
                        operandStack.pop();
                        double result = calculate(a, op, b);
                        operandStack.push(result);
                    }
                    break;
                }
                
                default:
                    throw std::runtime_error("Invalid priority relation");
            }
        }
    }
    
    while (operatorStack.top() != char2optr('\0')) {
        Operator op = operatorStack.top();
        operatorStack.pop();
        
        if (op == FAC) {
            if (operandStack.empty()) {
                throw std::runtime_error("Invalid factorial operation");
            }
            double operand = operandStack.top();
            operandStack.pop();
            double result = calculate(op, operand);
            operandStack.push(result);
        } else {
            if (operandStack.size() < 2) {
                throw std::runtime_error("Invalid expression: not enough operands");
            }
            double b = operandStack.top();
            operandStack.pop();
            double a = operandStack.top();
            operandStack.pop();
            double result = calculate(a, op, b);
            operandStack.push(result);
        }
    }
    
    if (operandStack.size() != 1) {
        throw std::runtime_error("Invalid expression");
    }
    
    return operandStack.top();
}

double evaluateExtendedExpression(const std::string& expression);

class FunctionParser {
public:
    static double evaluateFunction(const std::string& func_name, double arg) {
        if (func_name == "sin") {
            return sin(arg * M_PI / 180); 
        } else if (func_name == "cos") {
            return cos(arg * M_PI / 180); 
        } else if (func_name == "tan") {
            return tan(arg * M_PI / 180); 
        } else if (func_name == "log") {
            if (arg <= 0) throw std::runtime_error("Log of non-positive number");
            return log10(arg);
        } else if (func_name == "ln") {
            if (arg <= 0) throw std::runtime_error("Ln of non-positive number");
            return log(arg);
        } else if (func_name == "sqrt") {
            if (arg < 0) throw std::runtime_error("Square root of negative number");
            return sqrt(arg);
        } else if (func_name == "abs") {
            return abs(arg);
        } else {
            throw std::runtime_error("Unknown function: " + func_name);
        }
    }
    
    static std::string parseAndReplaceFunctions(const std::string& expression) {
        std::string result = expression;
        bool changed = true;
        
        while (changed) {
            changed = false;
            
            std::vector<std::string> functions = {"sin", "cos", "tan", "log", "ln", "sqrt", "abs"};
            
            for (const std::string& func : functions) {
                size_t pos = 0;
                while ((pos = result.find(func, pos)) != std::string::npos) {

                    size_t func_end = pos + func.length();
                    if (func_end < result.length() && result[func_end] == '(') {
                        size_t start_pos = pos;
                        size_t paren_count = 0;
                        size_t i = func_end + 1; 
                        
                        while (i < result.length()) {
                            if (result[i] == '(') {
                                paren_count++;
                            } else if (result[i] == ')') {
                                if (paren_count == 0) {
                                    std::string arg_str = result.substr(func_end + 1, i - func_end - 1);
                                    try {

                                        double arg_value = evaluateExtendedExpression(arg_str);
                                        double func_result = evaluateFunction(func, arg_value);
                                        
                                        std::string replacement = std::to_string(func_result);
                                        result.replace(start_pos, i - start_pos + 1, replacement);
                                        
                                        changed = true;
                                        pos = start_pos + replacement.length();
                                        break; 
                                    } catch (const std::exception& e) {
                                        pos = i + 1;
                                        break;
                                    }
                                } else {
                                    paren_count--;
                                }
                            }
                            i++;
                        }
                        
                        if (i >= result.length()) {
                            pos = func_end + 1; 
                        }
                    } else {
                        pos++; 
                    }
                }
            }
        }
        
        return result;
    }
};

// 扩展版计算器，支持复杂函数
double evaluateExtendedExpression(const std::string& expression) {
    std::string processed_expr = FunctionParser::parseAndReplaceFunctions(expression);
    
    return evaluateBasicExpression(processed_expr);
}

void runTests() {
    std::cout << "=== 字符串计算器测试 ===" << std::endl;
    
    std::vector<std::string> testCases = {
        "2+3",           // 基础加法
        "2*3+5",         // 混合运算
        "(2+3)*5",       // 括号运算
        "2^3",           // 幂运算
        "5!",            // 阶乘
        "2+3*4",         // 运算优先级
        "(2+3)*4-5",     // 复杂表达式
        "((2+3)*4-5)/2", // 嵌套括号
        "2+-3",          // 负数
        "-5+3",          // 表达式以负号开始
    };
    
    std::cout << "\n基础运算测试：" << std::endl;
    for (const auto& test : testCases) {
        try {
            double result = evaluateBasicExpression(test);
            std::cout << test << " = " << std::fixed << std::setprecision(6) << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << test << " -> 错误: " << e.what() << std::endl;
        }
    }
    
    // 错误情况测试
    std::cout << "\n错误处理测试：" << std::endl;
    std::vector<std::string> errorCases = {
        "2++3",          // 连续运算符
        "2)",            // 缺少左括号
        "(2+3",          // 缺少右括号
        "5!",            // 有效阶乘
    };
    
    for (const auto& test : errorCases) {
        try {
            double result = evaluateBasicExpression(test);
            std::cout << test << " = " << std::fixed << std::setprecision(6) << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << test << " -> 错误: " << e.what() << std::endl;
        }
    }
    
    std::cout << "\n复杂函数测试：" << std::endl;
    std::vector<std::string> functionTests = {
        "sin(30)",      // sin(30°) = 0.5
        "cos(60)",      // cos(60°) = 0.5
        "tan(45)",      // tan(45°) = 1
        "log(100)",     // log(100) = 2
        "ln(2.718)",    // ln(e) ≈ 1
        "sqrt(16)",     // sqrt(16) = 4
        "abs(-5)",      // abs(-5) = 5
    };
    
    for (const auto& test : functionTests) {
        try {
            double result = evaluateExtendedExpression(test);
            std::cout << test << " = " << std::fixed << std::setprecision(6) << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << test << " -> 错误: " << e.what() << std::endl;
        }
    }
    
    std::cout << "\n混合运算测试：" << std::endl;
    std::vector<std::string> mixedTests = {
        "2+sin(30)",    // 2 + 0.5 = 2.5
        "sqrt(16)*2",   // 4 * 2 = 8
    };
    
    for (const auto& test : mixedTests) {
        try {
            double result = evaluateExtendedExpression(test);
            std::cout << test << " = " << std::fixed << std::setprecision(6) << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << test << " -> 错误: " << e.what() << std::endl;
        }
    }
}

int main() {
    runTests();
    
    std::cout << "\n=== 交互式计算器 ===" << std::endl;
    std::cout << "输入表达式进行计算（输入'quit'退出）：" << std::endl;
    
    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        
        if (input == "quit" || input == "exit") {
            break;
        }
        
        try {
            double result = evaluateExtendedExpression(input);
            std::cout << "= " << std::fixed << std::setprecision(6) << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << "错误: " << e.what() << std::endl;
        }
    }
    
    return 0;
}
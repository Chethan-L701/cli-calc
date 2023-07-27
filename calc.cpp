#include <cmath>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

void operator<<(std::ostream &out, std::vector<std::string> &vec) {
    for (std::string st : vec)
        std::cout << st << '\t';
    std::cout << std::endl;
}

std::vector<char> op = {'+', '-', '*', '/', '^', '(', ')'};
std::string char_to_string(char ch) {
    auto ops = {"+", "-", "*", "/", "^", "(", ")"};
    for (auto o : ops)
        if (ch == *o)
            return o;
    return std::to_string(ch);
}

bool is_operator(char ch) {
    for (char v : op)
        if (ch == v)
            return true;
    return false;
}

std::unordered_map<std::string, int> oprec = {
    {"^", 4}, {"*", 3}, {"/", 3}, {"+", 2}, {"/", 2}, {"=", 0}, {"(", 0}};
bool high_priority(std::string op1, std::string op2) {
    if (oprec[op1] >= oprec[op2])
        return true;
    return false;
}

std::vector<std::string> to_postfix(std::vector<std::string> vec) {
    std::vector<std::string> postfix;
    std::vector<std::string> opr;
    for (auto s : vec) {
        if (is_operator(s[0])) {
            if (s != ")") {
                if (!opr.empty() && s != "(")
                    while (!opr.empty() &&
                           high_priority(opr[opr.size() - 1], s)) {
                        postfix.push_back(opr[opr.size() - 1]);
                        opr.pop_back();
                    }
                opr.push_back(s);
            } else if (s == ")") {
                while (!opr.empty() && opr[opr.size() - 1] != "(") {
                    postfix.push_back(opr[opr.size() - 1]);
                    opr.pop_back();
                }
                opr.pop_back();
            }
        } else {
            postfix.push_back(s);
        }
    }
    while (!opr.empty()) {
        postfix.push_back(opr[opr.size() - 1]);
        opr.pop_back();
    }
    return postfix;
}

std::vector<std::string> parse(std::string str) {
    std::vector<std::string> vec;
    std::string num = "";
    for (char ch : str) {
        if (ch == ' ' || ch == '\n')
            continue;
        if (is_operator(ch)) {
            if (num != "")
                vec.push_back(num);
            num = "";
            vec.push_back(char_to_string(ch));
        } else if (std::isdigit(ch) || ch == '.') {
            num += ch;
        }
    }
    if (num != "")
        vec.push_back(num);
    return vec;
}

double solve(std::vector<std::string> vec) {
    std::vector<double> oprad;
    for (auto s : vec) {
        if (is_operator(s[0])) {
            double op2 = oprad[oprad.size() - 1];
            oprad.pop_back();
            double op1 = oprad[oprad.size() - 1];
            oprad.pop_back();
            switch (s[0]) {
            case '+':
                oprad.push_back(op1 + op2);
                break;
            case '-':
                oprad.push_back(op1 - op2);
                break;
            case '*':
                oprad.push_back(op1 * op2);
                break;
            case '/':
                oprad.push_back(op1 / op2);
                break;
            case '^':
                oprad.push_back(pow(op1, op2));
                break;
            }
        } else
            oprad.push_back(std::stod(s));
    }
    return oprad[oprad.size() - 1];
}

int main() {
start:
    std::cout << ">";
    std::string expr;
    std::getline(std::cin, expr);
    if (expr == "clear") {
        system("clear");
        goto start;
    } else if (expr == "quit")
        exit(0);
    else if (expr == "" || expr == "\n")
        goto start;
    else {
        std::cout << std::setprecision(15) << solve(to_postfix(parse(expr)));
        std::cout << std::endl;
        goto start;
    }
    return 0;
}

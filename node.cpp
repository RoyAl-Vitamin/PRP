//
// Created by roal95@mail.ru on 15.04.2020.
//

#include <cctype>
#include <utility>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "node.h"

// Парсит уравнения и раскладывает его в бинарное дерево
// D=5*C+2*B^2+A^3
// E=5*(5*C+2*B+A)+2*C+B
// F=3*(8*C+5)*((9+(3*2+1))+(3+2*(3+1)))
node::node(std::string sub_expression) {
    expression = std::move(sub_expression);
    p_left_node = NULL;
    p_right_node = NULL;

    // Убирает лишние пробелы
    while (expression.find(' ') != std::string::npos) {
        expression.erase(expression.find(' '), 1);
    }

    char index = expression.find('=');
    if (index != std::string::npos) {
        type = node_type::EQUALS;
        p_left_node = new node(expression.substr(0, index));
        p_right_node = new node(expression.substr(index + 1, expression.length() - index - 1));
        return;
    }

    // Удаление лишних скобок
    bool end = false;
    while (expression.find('(') == 0 && expression.rfind(')') == expression.length() - 1) {
        size_t count = 0;
        for (size_t i = 0; i < expression.length(); i++) {
            if (count == 0 && i != 0 && i != expression.length() - 1) {
                end = true;
                break;
            }
            if (expression[i] == '(') count++;
            if (expression[i] == ')') count--;
        }
        if (end) break;
        if (count == 0) {
            // Если количество открытых и закрытых скобочек уравновешено, то удаляем лишние скобки в начале и в конце
            expression.erase(0, 1);
            expression.erase(expression.length() - 1, 1);
        }
    }

    // Если в строке остались мат. операции (без учёта унарных операций)
    char find_char = '+';
    if (expression.find(find_char) != std::string::npos) {
        size_t count = 0; // Индекс скобки: +1 - открыта; -1 - закрыта
        for (size_t i = 0; i < expression.length(); i++) {
            if (expression[i] == find_char && count == 0 && i > 0) {
                type = node_type::SUMM;
                p_left_node = new node(expression.substr(0, i));
                p_right_node = new node(expression.substr(i + 1, expression.length() - i - 1));
                return;
            }
            if (expression[i] == '(') count++;
            if (expression[i] == ')') count--;
        }
    }

    find_char = '*';
    if (expression.find(find_char) != std::string::npos) {
        size_t count = 0; // Индекс скобки: +1 - открыта; -1 - закрыта
        for (size_t i = 0; i < expression.length(); i++) {
            if (expression[i] == find_char && count == 0 && i > 0) {
                type = node_type::PRODUCE;
                p_left_node = new node(expression.substr(0, i));
                p_right_node = new node(expression.substr(i + 1, expression.length() - i - 1));
                return;
            }
            if (expression[i] == '(') count++;
            if (expression[i] == ')') count--;
        }
    }

    find_char = '/';
    if (expression.find(find_char) != std::string::npos) {
        size_t count = 0; // Индекс скобки: +1 - открыта; -1 - закрыта
        for (size_t i = 0; i < expression.length(); i++) {
            if (expression[i] == find_char && count == 0 && i > 0) {
                type = node_type::DIVIDE;
                p_left_node = new node(expression.substr(0, i));
                p_right_node = new node(expression.substr(i + 1, expression.length() - i - 1));
                return;
            }
            if (expression[i] == '(') count++;
            if (expression[i] == ')') count--;
        }
    }

    find_char = '^';
    if (expression.find(find_char) != std::string::npos) {
        size_t count = 0; // Индекс скобки: +1 - открыта; -1 - закрыта
        for (size_t i = 0; i < expression.length(); i++) {
            if (expression[i] == find_char && count == 0 && i > 0) {
                type = node_type::POWER;
                p_left_node = new node(expression.substr(0, i));
                p_right_node = new node(expression.substr(i + 1, expression.length() - i - 1));
                return;
            }
            if (expression[i] == '(') count++;
            if (expression[i] == ')') count--;
        }
    }

    // Если это переменная
    if (expression.length() == 1 && std::isdigit((int) expression.c_str()[0]) == 0) {
        type = node_type::VARIABLE;
    } else {
        // Если там число
        type = node_type::NUMBER;
//        value = atoi(expression->c_str());
    }
}

// Обёртка std::map<char, size_t> в std::map<char, double>
double node::calc(const std::map<char, size_t> &variables) {
    std::map<char, double> map;
    for (auto iterator = variables.begin(); iterator != variables.end(); ++iterator) {
        map.insert(std::pair<char, double>(iterator->first, (double) iterator->second));
    }
//    for (auto iterator = map.begin(); iterator != map.end(); iterator++) {
//        printf("%c :: %f\n", iterator->first, iterator->second);
//    }
//    printf("\n");
    return node::calc(map);
}

double node::calc(const std::map<char, double> &variables) {
    switch (type) {
        case node_type::POWER:
            return pow(p_left_node->calc(variables), p_right_node->calc(variables));
        case node_type::DIVIDE:
            return p_left_node->calc(variables) / p_right_node->calc(variables);
        case node_type::PRODUCE:
            return p_left_node->calc(variables) * p_right_node->calc(variables);
        case node_type::SUMM:
            return p_left_node->calc(variables) + p_right_node->calc(variables);
        case node_type::EQUALS: {
//            double l_result = p_left_node->calc(variables);
            double r_result = p_right_node->calc(variables);

//            return l_result > r_result ? l_result : r_result;
            return r_result;
        }
        case node_type::NUMBER:
            return strtod(expression.c_str(), NULL);
//            return atoi(expression.c_str());
        case node_type::VARIABLE: {
            auto p = variables.find(expression[0]);
            if (p != variables.end()) {
                return p->second;
            } else {
                printf("ERROR: WHILE OBTAIN VALUE!\n");
                return 0;
            }
        }
    }
}

node::~node() {
    delete p_left_node;
    delete p_right_node;
}

std::string node::print_expression() {
    std::string result;
    if (p_left_node) {
        if (p_left_node->type != node_type::NUMBER && p_left_node->type != node_type::VARIABLE && p_left_node->type < type) {
            result += '(' + p_left_node->print_expression() + ')';
        } else {
            result += p_left_node->print_expression();
        }
    }
    // Определить приоритет операций и расставить скобки (необходимое количество)
    switch (type) {
        case node_type::EQUALS:
            result += '=';
            break;
        case node_type::SUMM:
            result += '+';
            break;
        case node_type::PRODUCE:
            result += '*';
            break;
        case node_type::DIVIDE:
            result += '/';
            break;
        case node_type::POWER:
            result += '^';
            break;
        case node_type::NUMBER:
        case node_type::VARIABLE:
            result += expression;
    }
    if (p_right_node) {
        if (p_right_node && p_right_node->type != node_type::NUMBER && p_right_node->type != node_type::VARIABLE &&
            p_right_node->type < type) {
            result += '(' + p_right_node->print_expression() + ')';
        } else {
            result += p_right_node->print_expression();
        }
    }
    return result;
}

std::set<char> node::get_variables(std::set<char> &result) {
    if (type == node_type::VARIABLE) {
//        result.insert(expression.c_str());
        result.insert(expression[0]);
    } else {
        if (p_left_node) {
            p_left_node->get_variables(result);
        }
        if (p_right_node) {
            p_right_node->get_variables(result);
        }
    }

    return result;
}

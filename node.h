//
// Created by roal95@mail.ru on 15.04.2020.
//

#ifndef PRP_NODE_H
#define PRP_NODE_H


#include "node_type.h"
#include <string>
#include <map>
#include <set>

extern size_t q;


/*
 * Внимание!
 * Считается только правая часть!
 * Переменные задаются только одним латинским символом с учётом регистра!
 */
class node {
    // Исходное для узла выражение
    std::string expression;
    // Тип узла
    node_type   type;
    // Левый и правый узел, если имеются
    node        *p_left_node,
                *p_right_node;
public:
    // Создаёт дерево для дальнейших вычислений
    node(std::string sub_expression);
    ~node();
    // Обёртка
    double calc(const std::map<char, size_t> &variables);
    // Производит вычисления, взяв значения переменных для подсчёта из map
    double calc(const std::map<char, double> &variables);
    // Возвращает множество уникальных переменных из уравнения
    std::set<char> get_variables(std::set<char> &result);
    // Выводит уравнение, собранное по узлам
    std::string print_expression();
};


#endif //PRP_NODE_H

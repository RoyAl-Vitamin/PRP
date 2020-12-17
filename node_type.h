//
// Created by roal95@mail.ru on 15.04.2020.
//

#ifndef PRP_NODE_TYPE_H
#define PRP_NODE_TYPE_H


// Тип узла с учётом приоритета
enum node_type {

    // =
    EQUALS,

    // +
    SUMM,

    // *
    PRODUCE,

    // /
    DIVIDE,

    // ^
    POWER,

    // Число
    NUMBER,

    // Переменная
    VARIABLE,
};


#endif //PRP_NODE_TYPE_H

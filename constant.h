//
// Created by roal95@mail.ru on 20.04.2020.
//

#ifndef PRP31_CONSTANT_H
#define PRP31_CONSTANT_H


#include <cstdio>
#include <new>

class constant {
    static int K_q2[2][2];

    static int K_q3[3][3];

    static int K_q4[4][4];

    static int K_q5[5][5];

    static int K_q6[6][6];

    // Возвращает инвертированную матрицу K по заданному q
    static double **get_inverse_K_by_q(const size_t q);

    // Считает обратную матрицу
    static int **inverse_matrix(double **M, const size_t q);

    // Считает алгебраическое дополнение
    static double alg_dop(double **M, const size_t q, const size_t i, const size_t j);

    // Считает определитель матрицы
    static double det(double **M, const size_t q);

    // Считает факториал
    static double fact(size_t i);
public:

    // Возвращает матрицу K по заданному q
    static int **get_K_by_q(const size_t q);
};


#endif //PRP31_CONSTANT_H

//
// Created by roal95@mail.ru on 27.04.2020.
//

#include <cmath>
#include "constant.h"

int constant::K_q2[2][2] = {
        {1,  0},
        {-1, 0}
};

int constant::K_q3[3][3] = {
        {2,  0,  0},
        {-3, 4,  -1},
        {1,  -2, 1}
};

int constant::K_q4[4][4] = {
        {6,   0,   0,  0},
        {-11, 18,  -9, 2},
        {6,   -15, 12, -3},
        {-1,  3,   -3, 1}
};

int constant::K_q5[5][5] = {
        {24,  0,    0,   0,   0},
        {-50, 96,   -72, 32,  -6},
        {35,  -104, 114, -56, 11},
        {-10, 36,   -48, 28,  -6},
        {1,   -4,   6,   -4,  1}
};

int constant::K_q6[6][6] = {
        {120,  0,    0,    0,    0,    0},
        {-274, 660,  -660, 400,  -150, 24},
        {225,  -770, 1070, -780, 305,  -20},
        {-85,  355,  -590, 490,  -205, 35},
        {15,   -70,  130,  -120, 55,   -10},
        {-1,   5,    -10,  10,   -5,   1}
};

int **constant::get_K_by_q(const size_t q) {
    // Говнокод
    switch (q) {
        case 2: {
            int **temp = new (std::nothrow) int *[q];
            if (!temp) {
                printf("Can't allocate memory: bad_alloc!");
                return NULL;
            }
            for (size_t i = 0; i < q; i++) {
                temp[i] = new (std::nothrow) int[q];
                if (!temp[i]) {
                    printf("Can't allocate memory: bad_alloc!");
                    return NULL;
                }
                for (size_t j = 0; j < q; j++) {
                    temp[i][j] = K_q2[i][j];
                }
            }
            return temp;
        }
        /*case 3: {
            int **temp = new (std::nothrow) int *[q];
            if (!temp) {
                printf("Can't allocate memory: bad_alloc!");
                return NULL;
            }
            for (size_t i = 0; i < q; i++) {
                temp[i] = new (std::nothrow) int[q];
                if (!temp[i]) {
                    printf("Can't allocate memory: bad_alloc!");
                    return NULL;
                }
                for (size_t j = 0; j < q; j++) {
                    temp[i][j] = K_q3[i][j];
                }
            }
            return temp;
        }
        case 4: {
            int **temp = new (std::nothrow) int *[q];
            if (!temp) {
                printf("Can't allocate memory: bad_alloc!");
                return NULL;
            }
            for (size_t i = 0; i < q; i++) {
                temp[i] = new (std::nothrow) int[q];
                if (!temp[i]) {
                    printf("Can't allocate memory: bad_alloc!");
                    return NULL;
                }
                for (size_t j = 0; j < q; j++) {
                    temp[i][j] = K_q4[i][j];
                }
            }
            return temp;
        }
        case 5: {
            int **temp = new (std::nothrow) int *[q];
            if (!temp) {
                printf("Can't allocate memory: bad_alloc!");
                return NULL;
            }
            for (size_t i = 0; i < q; i++) {
                temp[i] = new (std::nothrow) int[q];
                if (!temp[i]) {
                    printf("Can't allocate memory: bad_alloc!");
                    return NULL;
                }
                for (size_t j = 0; j < q; j++) {
                    temp[i][j] = K_q5[i][j];
                }
            }
            return temp;
        }
        case 6: {
            int **temp = new (std::nothrow) int *[q];
            if (!temp) {
                printf("Can't allocate memory: bad_alloc!");
                return NULL;
            }
            for (size_t i = 0; i < q; i++) {
                temp[i] = new (std::nothrow) int[q];
                if (!temp[i]) {
                    printf("Can't allocate memory: bad_alloc!");
                    return NULL;
                }
                for (size_t j = 0; j < q; j++) {
                    temp[i][j] = K_q6[i][j];
                }
            }
            return temp;
        }*/
        default: {
            double **temp_m = get_inverse_K_by_q(q);
            if (!temp_m) {
                printf("ERROR! While getting inverse matrix K!");
                return NULL;
            }
            int **result = inverse_matrix(temp_m, q);

            for (size_t k = 0; k < q; k++) {
                delete temp_m[k];
            }
            delete temp_m;
            return result;
        }
    }
}

double **constant::get_inverse_K_by_q(const size_t q) {
    double **inverse_K = new (std::nothrow) double *[q];
    if (!inverse_K) {
        printf("Can't allocate memory: bad_alloc!");
        return NULL;
    }
    for (size_t i = 0; i < q; i++) {
        inverse_K[i] = new (std::nothrow) double[q];
        if (!inverse_K[i]) {
            printf("Can't allocate memory: bad_alloc!");
            return NULL;
        }
    }
    for (size_t i = 0; i < q; i++) {
        inverse_K[i][0] = 1;
        for (size_t j = 1; j < q; j++) {
            inverse_K[i][j] = inverse_K[i][j - 1] * i;
        }
    }
    /*printf("K inverse matrix:\n");
    for (size_t i = 0; i < q; i++) {
        for (size_t j = 0; j < q; j++) {
            printf("%f\t", inverse_K[i][j]);
        }
        printf("\n");
    }*/
    return inverse_K;
}

// Инвертирует квадратную матрицу
int **constant::inverse_matrix(double **M, const size_t q) {
    if (!M) {
        printf("ERROR! Matrix can't be a null!");
        return NULL;
    }
    double N = fact(q - 1);
    int **result = new (std::nothrow) int*[q];
    if (!result) {
        printf("Can't allocate memory: bad_alloc!");
        return NULL;
    }

    double **M_transpose = new (std::nothrow) double*[q];
    if (!M_transpose) {
        printf("Can't allocate memory: bad_alloc!");
        return NULL;
    }

    double determine = det(M, q);
    if (determine == 0.) {
        printf("ERROR! Determinant can't be equals 0!");
        return NULL;
    }
    //printf("determine == %f\n", determine);

//    printf("transpose Matrix / N\n");
    // Транспонированная матрица
    for (size_t i = 0; i < q; i++) {
        result[i] = new (std::nothrow) int[q];
        if (!result[i]) {
            printf("Can't allocate memory: bad_alloc!");
            return NULL;
        }

        M_transpose[i] = new (std::nothrow) double[q];
        if (!M_transpose[i]) {
            printf("Can't allocate memory: bad_alloc!");
            return NULL;
        }

        for (size_t j = 0; j < q; j++) {
            M_transpose[i][j] = M[j][i] / N;
//            printf("\t%f", M_transpose[i][j]);
        }
//        printf("\n");
    }

//    printf("result Matrix:\n");
    for (size_t i = 0; i < q; i++) {
        for (size_t j = 0; j < q; j++) {
            result[i][j] = round(alg_dop(M_transpose, q, i, j) * ((i + j) % 2 == 0 ? 1 : -1) /*/ determine*/);
//            printf("\t%f", alg_dop(M_transpose, q, i, j));
        }
//        printf("\n");
    }
    for (size_t i = 0; i < q; i++) {
        delete[] M_transpose[i];
    }
    delete[] M_transpose;
    return result;
}

double constant::alg_dop(double **M, const size_t q, const size_t i, const size_t j) {
    if (!M) {
        printf("ERROR! Matrix can't be a null!");
        return 0;
    }
    double **r = new(std::nothrow) double *[q - 1];
    if (!r) {
        printf("Can't allocate memory: bad_alloc!");
        return 0;
    }
    for (size_t k = 0; k < q - 1; k++) {
        r[k] = new(std::nothrow) double[q - 1];
        if (!r[k]) {
            printf("Can't allocate memory: bad_alloc!");
            return 0;
        }
        for (size_t l = 0; l < q - 1; l++) {
            r[k][l] = M[k >= i ? k + 1 : k][l >= j ? l + 1 : l];
        }
    }
    return det(r, q - 1);
}

double constant::det(double **M, const size_t q) {
    if (!M) {
        printf("ERROR! Matrix can't be a null!");
        return 0;
    }
    if (q == 1) { return M[0][0]; }
    if (q == 2) { return M[0][0] * M[1][1] - M[1][0] * M[0][1]; }
    if (q == 3) {
        return M[0][0] * M[1][1] * M[2][2]
             + M[0][1] * M[1][2] * M[2][0]
             + M[0][2] * M[1][0] * M[2][1]
             - M[2][0] * M[1][1] * M[0][2]
             - M[2][1] * M[1][2] * M[0][0]
             - M[2][2] * M[1][0] * M[0][1];
    }
    double **temp = new(std::nothrow) double *[q - 1];
    if (!temp) {
        printf("Can't allocate memory: bad_alloc!");
        return 0.;
    }
    for (size_t i = 0; i < q - 1; i++) {
        temp[i] = new(std::nothrow) double[q - 1];
        if (!temp[i]) {
            printf("Can't allocate memory: bad_alloc!");
            return 0.;
        }
    }
    double result = 0.;
    for (size_t k = 0; k < q; k++) {
        if (M[k][0] == 0.) { continue; }
        printf("submatrix for M[%f]:\n", M[k][0]);
        for (size_t i = 0; i < q - 1; i++) {
            for (size_t j = 0; j < q - 1; j++) {
                temp[i][j] = M[i >= k ? i + 1 : i][j + 1];
                printf("\t%f", temp[i][j]);
            }
            printf("\n");
        }
        result += M[k][0] * (k % 2 == 0 ? 1. : -1.) * det(temp, q - 1);
    }
    for (size_t i = 0; i < q - 1; i++) {
        delete[] temp[i];
    }
    delete[] temp;
//    printf("det === %f", result);
    return result;
}

double constant::fact(size_t n) {
    double result = n;
    for (size_t i = 2; i < n; i++) {
        result *= i;
    }
    return result;
}

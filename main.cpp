#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>
#include "constant.h"
#include "node.h"

int** produce_kronekers(int **A, const size_t A_row_count, const size_t A_col_count,
                        int **B, const size_t B_row_count, const size_t B_col_count);

int *produce_matrix_vector(int **K, size_t row_count, size_t col_count, const int *s, size_t s_length);

int *get_S(node *p_node, const std::map<char, size_t> &start_value, size_t count_elements);

void print_matrix(const std::string& message, size_t **M, const size_t row_count, const size_t col_count);

void print_matrix(const std::string& message, int **M, const size_t row_count, const size_t col_count);

void print_vector(const std::string &message, int *v, const size_t length);

// Размерность поля Галуа
size_t q = 3;

// Максимальная степень полинома
size_t r = 2;

int main() {
    printf("function:\n");
    std::string expression;
    std::getline(std::cin, expression);
    printf("q =\n");
    std::cin >> q;
    printf("r =\n");
    std::cin >> r;
//    // test
////    int **origin_K2 = constant::get_K_by_q(q);
////    print_matrix("result Matrix:", origin_K2, q, q);
////    return 0;
    node *p_node = new (std::nothrow) node(expression);
    if (!p_node) {
        printf("Can't allocate memory: bad_alloc!");
        return 0;
    }
//    std::cout << p_node->print_expression() << std::endl;

    std::set<char> variables;
    p_node->get_variables(variables);

    printf("init value:\n");
    std::map<char, size_t> map_value;
    for (std::set<char>::iterator variable = variables.begin(); variable != variables.end(); variable++) {
        size_t temp_var;
        printf("%c==\n", *variable);
        scanf("%u", &temp_var);
        map_value.insert(std::pair<char, size_t>(*variable, temp_var));
    }
    int *s = get_S(p_node, map_value, (int) pow(q, r));
    if (!s) {
        printf("Can't allocate memory: bad_alloc!");
        return 0;
    }
//    print_vector("S vector:", s, (size_t) pow(q, r));

//    for (std::map<char, size_t>::iterator iterator = map_value.begin(); iterator != map_value.end(); ++iterator) {
//        printf("%c == %u\n", iterator->first, iterator->second);
//    }

    // Получаем исходную матрицу K
    int **origin_K = constant::get_K_by_q(q);
    if (!origin_K) {
        printf("Error while getting origin K matrix");
        return 0;
    }

    // Исходные размеры матрицы K
    size_t A_row_count = q, A_col_count = q, B_row_count = q, B_col_count = q;

    // Возведение в степень
    int **K = origin_K;
    for (size_t k = 0; k < r - 1; k++) {
        int **temp_K = produce_kronekers(K, A_row_count, A_col_count, origin_K, B_row_count, B_col_count);
        if (!temp_K) {
            printf("ERROR! While Kronecker's produce");
            return 0;
        }
        if (K != origin_K) {
            // Отчистка результата умножения предыдущего шага на исходную матрицу K
            for (size_t i = 0; i < A_row_count; i++) {
                delete[] K[i];
            }
            delete[] K;
        }
        A_row_count *= B_row_count;
        A_col_count *= B_col_count;
        K = temp_K;
    }

    // Отчистка исходной матрицы K
    for (size_t i = 0; i < q; i++) {
        delete[] origin_K[i];
    }
    delete[] origin_K;

    if (!K) {
        printf("ERROR! While Kronecker's produce");
        return 0;
    }

//    print_matrix("K matrix:", K, A_row_count, A_col_count);

    const size_t s_length = A_col_count;
    int *A = produce_matrix_vector(K, A_row_count, A_col_count, s, s_length);
    if (!A) {
        printf("ERROR! While produce K*s");
        return 0;
    }

    // Считаем степени переменных в полиноме
    const size_t count_variables = variables.size(); // Количество уникальных переменных. Берётся из исходного МФАЛ
    size_t **pow_array = new (std::nothrow) size_t *[s_length];
    if (!pow_array) {
        printf("Can't alloc memory: bad_alloc!");
        return 0;
    }
    for (size_t i = 0; i < s_length; i++) {
        pow_array[i] = new (std::nothrow) size_t[count_variables];
        if (!pow_array[i]) {
            printf("Can't alloc memory: bad_alloc!");
            return 0;
        }
        for (size_t j = 0; j < count_variables; j++) {
            pow_array[i][j] = 0;
        }
    }
    size_t v = 1;
    for (size_t j = count_variables - 1; j + 1 > 0; j--) {
        size_t k = 0;
        for (size_t i = 0; i < s_length; i++) {
            if (k == q) {
                k = 0;
            }
            pow_array[i][j] = k;
            if ((i + 1) % v == 0) {
                k++;
            }
        }
        v *= q;
    }

//    print_matrix("print POW ARRAY:", pow_array, s_length, count_variables);

    // Собираем полином
    std::stringstream sstream;
    sstream << "(";
    for (size_t i = 0; i < s_length; i++) {
        if (A[i] == 0) {
            continue;
        }
        if (A[i] > 1) {
            sstream << A[i];
        } else if (A[i] < 0) {
            sstream << "(" << A[i] << ")";
        }
        std::set<char>::const_iterator element;
        size_t j = 0;
        std::stringstream s_sub_stream;
        for (element = variables.begin(); element != variables.end(); j++) {
            if (pow_array[i][j] == 0) {
                ++element;
                continue;
            } else if (pow_array[i][j] == 1) {
                s_sub_stream << *element;
            } else {
                s_sub_stream << *element << "^" << pow_array[i][j];
            }
            bool has_next_not_zero_element = false;
            for (size_t k = j + 1; k < count_variables; k++) {
                if (pow_array[i][k] != 0) {
                    has_next_not_zero_element = true;
                    break;
                }
            }
            if (++element != variables.end() && has_next_not_zero_element) {
                s_sub_stream << "*";
            }
        }
        if (!s_sub_stream.str().empty()) {
            if (A[i] != 1) {
                sstream << "*";
            }
            sstream << s_sub_stream.str();
        }

        bool one_next_value_not_zero = false;
        for (size_t k = i + 1; k < s_length; k++) {
            if (A[k] != 0) {
                one_next_value_not_zero = true;
                break;
            }
        }

        // если это не последний элемент и есть хотя бы один коэффициент A после индекса i не равный 0
        if (i != s_length - 1 && one_next_value_not_zero) {
            sstream << "+";
        }
    }
    if (sstream.str().length() == 1) {
        sstream << "0";
    }
    // Уточнить формулу получения N_q
//    sstream << ")/" << pow(2, r);
    sstream << ")/" << pow(q - 1, r);
    printf("result string == %s\n", sstream.str().c_str());

    // Примрер получения значений полинома при заданных значениях переменных
    /*node *result_node = new node(sstream.str());
    std::map<char, double> map;
    map.insert(std::pair<char, double>('A', 2.));
    map.insert(std::pair<char, double>('B', 2.0));
    map.insert(std::pair<char, double>('C', 2.));
    printf("result == %f", result_node->calc(map));

    delete result_node;*/

    delete[] A;

    delete[] s;

    for (size_t i = 0; i < s_length; i++) {
        delete[] pow_array[i];
    }
    delete[] pow_array;

    for (size_t i = 0; i < A_col_count; i++) {
        delete[] K[i];
    }
    delete[] K;

    delete p_node;
    system("pause");
    return 0;
}

int *get_S(node *p_node, const std::map<char, size_t>& start_value, size_t count_elements) {
    int *result = new (std::nothrow) int[count_elements];

    if (!result) {
        printf("Can't alloc memory: bad_alloc!");
        return NULL;
    }

    // Copy start value in result array
    size_t i = 0;
    for (auto iterator = start_value.begin(); iterator != start_value.end(); iterator++, i++) {
        result[i] = iterator->second;
    }
    std::map<char, size_t> value(start_value);
    size_t *temp_array = new (std::nothrow) size_t[start_value.size()];

    if (!temp_array) {
        printf("Can't alloc memory: bad_alloc!");
        return NULL;
    }

    // WARNING! Подумать: можно ли так делать? Суммирование ?= суммирование по модулю
    for (i = start_value.size(); i < count_elements; i++) {
        // Расчёт
//        printf("iteration == %d\n", i);
//        for (auto iterator = value.begin(); iterator != value.end(); iterator++) {
//            printf("%c :: %d\n", iterator->first, iterator->second);
//        }
        // WARNING! Сужение области!!!
        double next_double = p_node->calc(value);
        int next_value = (int) next_double % q;

        // Копирование новых значений в старый value
        result[i] = (int) next_value;
        size_t j = 0; // копируем со второго элемента
        for (auto iterator = ++(value.begin()); iterator != value.end(); ++iterator) {
            temp_array[j++] = iterator->second;
        }
        temp_array[j] = next_value;
        j = 0;
        for (auto iterator = value.begin(); iterator != value.end(); iterator++) {
            iterator->second = temp_array[j++];
        }
    }
    delete[] temp_array;
    return result;
}

// Кронекеровское умножение => A * B = [a[i, j] * B]
// A - исходная матрица
// A_row_count - кол-во строк в матрице A
// A_col_count - кол-во столбцов в матрице A
// B - исходная матрица
// B_row_count - кол-во строк в матрице B
// B_col_count - кол-во столбцов в матрице B
// q - размерность поля Галуа
int **produce_kronekers(int **A, const size_t A_row_count, const size_t A_col_count,
                        int **B, const size_t B_row_count, const size_t B_col_count) {

    if (!A || !B) {
        printf("ERROR! Matrix can't be a null!");
        return NULL;
    }
    int **result = new (std::nothrow) int*[A_row_count * B_row_count];
    if (!result) {
        printf("Can't alloc memory: bad_alloc!");
        return NULL;
    }
    for (size_t i = 0; i < A_row_count * B_row_count; i++) {
        result[i] = new (std::nothrow) int[A_col_count * B_col_count];
        if (!result[i]) {
            printf("Can't alloc memory: bad_alloc!");
            return NULL;
        }
    }

    size_t s = 0;
    for (size_t i = 0; i < A_row_count; i++) {
        size_t t = 0;
        for (size_t j = 0; j < A_col_count; j++) {
            for (size_t k = 0; k < B_row_count; k++) {
                for (size_t l = 0; l < B_col_count; l++) {
                    result[s + k][t + l] = A[i][j] * B[k][l];
                }
            }
            t += B_col_count;
        }
        s += B_row_count;
    }

    return result;
}

// Умножает матрицу на вектор
int *produce_matrix_vector(int **K, size_t row_count, const size_t col_count, const int *s, const size_t s_length) {
    if (!K || !s) {
        printf("ERROR: argument is NULL!");
        return NULL;
    }

    int *result = new (std::nothrow) int[s_length];
    if (!result) {
        printf("Can't alloc memory: bad_alloc!");
        return NULL;
    }

    for (size_t i = 0; i < row_count; i++) {
        result[i] = 0;
        for (size_t j = 0; j < col_count; j++) {
            result[i] += K[i][j] * s[j];
        }
    }

    return result;
}

// Выводит матрицу на экран
void print_matrix(const std::string& message, size_t **M, const size_t row_count, const size_t col_count) {
    if (!M) {
        printf("ERROR! While print matrix\n");
        return;
    }
    printf("%s\n", message.c_str());
    for (size_t i = 0; i < row_count; i++) {
        for (size_t j = 0; j < col_count; j++) {
            printf("\t%u", M[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_matrix(const std::string& message, int **M, const size_t row_count, const size_t col_count) {
    if (!M) {
        printf("ERROR! While print matrix\n");
        return;
    }
    printf("%s\n", message.c_str());
    for (size_t i = 0; i < row_count; i++) {
        for (size_t j = 0; j < col_count; j++) {
            printf("\t%d", M[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Выводит вектор на экран
void print_vector(const std::string &message, int *v,const size_t length) {
    if (!v) {
        printf("ERROR! While print vector\n");
        return;
    }
    printf("%s\n", message.c_str());
    for (size_t i = 0; i < length; i++) {
        printf("\t%d", v[i]);
    }
    printf("\n");
}

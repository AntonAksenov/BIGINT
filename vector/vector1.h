#ifndef BIGINT_VECTOR_H
#define BIGINT_VECTOR_H

#include "CoW_array.h"

class vector1 {
    bool big;

    size_t vector_size;

    uint32_t *pointer;

    union data {
        uint32_t small;
        CoW_array big;

        data() {};

        ~data() {};
    };

    data data;

    void make_big();

public:

    vector1();

    vector1(size_t size);

    vector1(const vector1 &other);


    void push_back(uint32_t value);


    void toString();

};


#endif //BIGINT_VECTOR_H

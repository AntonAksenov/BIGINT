#ifndef BIGINT_LAST_VECTOR_H
#define BIGINT_LAST_VECTOR_H

#include "CoW.h"

class vector {
    bool big;
    size_t vector_size;
    
    uint32_t small_data = 0;

    CoW *big_data = nullptr;

    void make_big();

    void make_small();

public:

    vector();

    vector(size_t size);

    vector(size_t size, uint32_t value);

    vector(const vector &other);

    ~vector();


    size_t size() const;

    bool empty() const;


    void push_back(uint32_t value);

    uint32_t pop_back();

    void push_front(uint32_t value);


    uint32_t &operator[](size_t index);

    const uint32_t &operator[](size_t index) const;

    uint32_t &back();

    const uint32_t &back() const;


    friend void swap(vector &a, vector &b);

    friend bool operator==(vector const &a, vector const &b);

    void resize(size_t capacity);

    void clear();


    void toString() const;
};

void swap(vector &a, vector &b);

bool operator==(vector const &a, vector const &b);

#endif //BIGINT_LAST_VECTOR_H


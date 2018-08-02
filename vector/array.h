#ifndef UNTITLED4_ARRAY1_H
#define UNTITLED4_ARRAY1_H

#include <cstdlib>
#include <cstdint>

#include <string>

class array {
    const size_t MIN_CAPACITY = 2;
    size_t capacity;
    size_t vector_size;

    void actual_resize(size_t new_capacity);

public:
    uint32_t *pointer;


    array();

    array(size_t capacity);

    array(array &other);

    ~array();

    size_t size() const;

    bool empty() const;

    void resize(size_t new_capacity);


    void push_back(uint32_t value);

    void push_front(uint32_t value);

    uint32_t pop_back();

    uint32_t back();

    friend void swap(array &a, array &b);



    void toString()const;

};

void swap(array &a, array &b);

#endif //UNTITLED4_ARRAY1_H

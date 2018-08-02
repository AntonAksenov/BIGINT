#include <iostream>
#include "vector1.h"

vector1::vector1() : big(false), vector_size(0), pointer(&data.small) {}

vector1::vector1(size_t size) : big(size > 1), vector_size(size) {
    if (big) {
        data.big = *new CoW_array();
        pointer = data.big.const_get()->pointer;
    } else {
        pointer = &data.small;
    }
}

vector1::vector1(const vector1 &other) :
        big(other.big), vector_size(other.vector_size), data() {
    data.small = other.data.small;
    if (big) {
        data.big = CoW_array(other.data.big);
    }
}


void vector1::make_big() {
    if (big == true) {
        std::cout << "making big bigger";
    }
    big = true;
    data.big = CoW_array();

    pointer = data.big.const_get()->pointer;
    /*
    vector1 tmp(2);
    tmp.data.big.get()->push_back(data.small);
    tmp.big = true;
    swap(*this, tmp);
    */
}


void vector1::push_back(uint32_t value) {
    if (vector_size == 0) {
        data.small = value;
    } else {
        if (vector_size == 1) {
            make_big();
        }
        data.big.get()->push_back(value);
    }
    vector_size++;
}


void vector1::toString() {
    if (big) {
        std::cout << "big\t" << vector_size << "\t{";
        data.big.toString();
        std::cout << ")";
    } else {
        std::cout << "sml\t" << vector_size << "\t{" << data.small << "}";
    }
    std::cout << std::endl;
}

#include <algorithm>
#include <iostream>
#include "array.h"

void array::actual_resize(size_t new_capacity) {
    uint32_t *new_pointer = new uint32_t[new_capacity];
    std::copy(pointer, pointer + vector_size, new_pointer);
    delete[] (pointer);
    capacity = new_capacity;
    pointer = new_pointer;
}

array::array() : capacity(MIN_CAPACITY), vector_size(0), pointer(new uint32_t[MIN_CAPACITY]) {
    std::cout << "arr initialization" << std::endl;
}

array::array(size_t capacity) : capacity(std::min(capacity, MIN_CAPACITY)), vector_size(0),
                                pointer(new uint32_t[this->capacity]) {
    std::cout << "arr initialization" << std::endl;
}

array::array(array &other) : capacity(other.capacity), vector_size(other.vector_size),
                             pointer(new uint32_t[capacity]) {
    std::copy(other.pointer, other.pointer + vector_size, pointer);
    std::cout << "arr initialization" << std::endl;
}

array::~array() {
    delete[] (pointer);
}

size_t array::size() const {
    return vector_size;
}

bool array::empty() const {
    return size() == 0;
}


void array::resize(size_t new_capacity) {
    if (new_capacity > capacity) {
        actual_resize(new_capacity);
    }
}

void array::push_back(uint32_t value) {
    if (size() == capacity) {
        actual_resize(capacity * 2);
    }
    pointer[size()] = value;
    vector_size++;
}

void array::push_front(uint32_t value) {
    if (size() == capacity) {
        actual_resize(capacity * 2);
    }
    for (size_t i = size(); i > 0; --i) {
        pointer[i] = pointer[i - 1];
    }
    pointer[0] = value;
    vector_size++;
}

uint32_t array::pop_back() {
    uint32_t ans = back();
    vector_size--;
    if (vector_size <= capacity / 4 && capacity > MIN_CAPACITY) {
        actual_resize(capacity / 2);
    }
    return ans;
}

uint32_t array::back() {
    return pointer[size() - 1];
}


void array::toString() const {
  //  std::cout << capacity << "\t" << size() << "\t";
    for (size_t i = 0; i < size(); i++) {
        std::cout << static_cast<int>(pointer[i]) << " ";
    }
    for (size_t i = size(); i < capacity; i++) {
        std::cout << "_ ";
    }
}
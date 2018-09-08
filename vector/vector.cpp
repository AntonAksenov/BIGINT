#include <iostream>
#include <cassert>
#include "vector.h"

vector::vector() : big(false), vector_size(0) {}

vector::vector(size_t size) : big(size > SMALL_SIZE), vector_size(size) {
    if (big) {
        big_data = new CoW();
        big_data->get()->resize(size);
    }
}

vector::vector(size_t size, uint32_t value) : big(size > SMALL_SIZE), vector_size(size) {
    if (big) {
        big_data = new CoW();
        big_data->get()->resize(size, value);
    } else {
        std::fill(small_data, small_data + SMALL_SIZE, value);
        //small_data = value;
    }
}

vector::vector(const vector &other) : big(other.big), vector_size(other.vector_size) {
    if (big) {
        big_data = new CoW(*other.big_data);
    } else {
        std::copy(other.small_data, other.small_data + other.SMALL_SIZE, small_data);
        //small_data = other.small_data;
    }
}


vector::~vector() {
    if (big) {
        make_small();
    }
}

size_t vector::size() const {
    return vector_size;
}

bool vector::empty() const {
    return vector_size == 0;
}

void vector::make_big() {
    big = true;
    big_data = new CoW();

    for (size_t i = 0; i < SMALL_SIZE; i++) {
        big_data->get()->push_back(small_data[i]);
    }
    //big_data->get()->push_back(small_data);
}

void vector::make_small() {
    big = false;
    for (size_t i = 0; i < SMALL_SIZE; i++) {
        small_data[i] = big_data->const_get()->at(i);
        //big_data->get()->push_back(small_data[i]);
    }

    delete big_data;
}


void vector::push_back(uint32_t value) {
    if (vector_size < SMALL_SIZE) {
        small_data[vector_size] = value;
    } else {
        if (vector_size == SMALL_SIZE) {
            make_big();
        }
        big_data->get()->push_back(value);
    }
    vector_size++;
}

uint32_t vector::pop_back() {
    assert(vector_size != 0);
    if (vector_size <= SMALL_SIZE) {
        vector_size--;
        return small_data[vector_size];
    } else {
        uint32_t t = big_data->get()->back();
        big_data->get()->pop_back();
        if (vector_size == SMALL_SIZE + 1) {
            make_small();
        }
        vector_size--;
        return t;
    }
}

void vector::push_front(uint32_t value) {
    if (vector_size < SMALL_SIZE) {
        for (size_t i = SMALL_SIZE; i > 0; i--) {
            small_data[i] = small_data[i - 1];
        }
        small_data[0] = value;
    } else {
        if (vector_size == SMALL_SIZE) {
            make_big();
        }
        big_data->get()->insert(big_data->get()->begin(), value);
    }
    vector_size++;
}


uint32_t &vector::operator[](size_t index) {
    if (big) {
        return (big_data->get())->at(index);
    } else {
        assert(index < vector_size);
        return small_data[index];
    }
}

const uint32_t &vector::operator[](size_t index) const {
    if (big) {
        return (big_data->get())->at(index);
    } else {
        assert(index < vector_size);
        return small_data[index];
    }
}

uint32_t &vector::back() {
    if (big) {
        return (big_data->get())->at(vector_size - 1);
    } else {
        assert(vector_size > 0 && vector_size <= SMALL_SIZE);
        return small_data[vector_size - 1];
    }
}

const uint32_t &vector::back() const {
    if (big) {
        return (big_data->get())->at(vector_size - 1);
    } else {
        assert(vector_size > 0 && vector_size <= SMALL_SIZE);
        return small_data[vector_size - 1];
    }
}

void swap(vector &a, vector &b) {
    std::swap(a.vector_size, b.vector_size);
    std::swap(a.big, b.big);
    //std::swap(a.small_data, b.small_data);
    for (size_t i = 0; i < a.SMALL_SIZE; i++) {
        std::swap(a.small_data[i], b.small_data[i]);
    }
    std::swap(a.big_data, b.big_data);
}

bool operator==(vector const &a, vector const &b) {
    if (a.vector_size != b.vector_size) {
        return false;
    } else {
        for (size_t i = 0; i < a.vector_size; i++) {
            if (a[i] != b[i]) {
                return false;
            }
        }
    }
    return true;
}

void vector::resize(size_t capacity) {
    if (capacity >= SMALL_SIZE + 1) {
        if (!big) {
            make_big();
        }
        big_data->get()->resize(capacity);
    }
    vector_size = capacity;
}

void vector::clear() {
    if (big) {
        make_small();
    }
    vector_size = 0;
}


void vector::toString() const {
    if (big) {
        std::cout << "big\tsize " << vector_size << "\t{";
        for (size_t i = 0; i < big_data->const_get()->size(); i++) {
            std::cout << big_data->const_get()->at(i) << " ";
        }
        std::cout << "}";
    } else {
        std::cout << "sml\tsize " << vector_size << "\t{";

        for (size_t i = 0; i < vector_size; i++) {
            std::cout << small_data[i] << " ";
        }
        std::cout << "}";
    }
    std::cout << std::endl;
}

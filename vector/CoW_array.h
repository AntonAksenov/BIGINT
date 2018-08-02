#ifndef UNTITLED4_COW_ARRAY_H
#define UNTITLED4_COW_ARRAY_H

#include <memory>
#include "array.h"

class CoW_array {
    mutable std::shared_ptr<array> data;

    void ensure_unique() const;

    void ensure_init() const; // do we need it ?

public:
    CoW_array();

    CoW_array(const CoW_array &other);

//    CoW_array& operator=(const CoW_array &other);

    void reset();

//    void reset(CoW_array &other);

    array const *const_get() const;

    array *get() const;

    void toString() const;
};

#endif //UNTITLED4_COW_ARRAY_H

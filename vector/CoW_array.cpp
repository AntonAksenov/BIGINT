#include <iostream>
#include "CoW_array.h"

CoW_array::CoW_array() : data() {
    std::cout << "CoW initialization" << std::endl;
}

CoW_array::CoW_array(const CoW_array &other) : data(other.data) {
    std::cout << "CoW initialization" << std::endl;
}

/*CoW_array &CoW_array::operator=(const CoW_array &other) {
    data = new std::shared_ptr<array>(other.data);
}*/

array const *CoW_array::const_get() const {
    ensure_init();
    return data.get();
}

array *CoW_array::get() const {
    ensure_unique();
    return data.get();
}

void CoW_array::reset() {
    data.reset();
}

/*void CoW_array::reset(CoW_array &other) {
    data = new std::shared_ptr<array>(other.data);
}*/

void CoW_array::ensure_unique() const {
    ensure_init();

    if (!data.unique()) {
        data.reset(new array(*data));
    }
}

void CoW_array::ensure_init() const {
    if (!data) {
        data.reset(new array);
    }
}

void CoW_array::toString() const {
    //   std::cout << "count: " << data.use_count() << "\t";
    const_get()->toString();
}
#include <iostream>
#include "CoW.h"

CoW::CoW() : data() {}

CoW::CoW(const CoW &other) : data(other.data) {}

std::vector<uint32_t> const *CoW::const_get() const {
    ensure_init();
    return data.get();
}

std::vector<uint32_t> *CoW::get() const {
    ensure_unique();
    return data.get();
}

void CoW::ensure_unique() const {
    ensure_init();

    if (!data.unique()) {
        data.reset(new std::vector<uint32_t>(*data));
    }
}

void CoW::ensure_init() const {
    if (!data) {
        data.reset(new std::vector<uint32_t>);
    }
}

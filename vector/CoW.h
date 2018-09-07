#ifndef BIGINT_LAST_COW_H
#define BIGINT_LAST_COW_H


#include <cstdint>
#include <vector>
#include <memory>

class CoW {
    mutable std::shared_ptr<std::vector<uint32_t>> data;

    void ensure_unique() const;

    void ensure_init() const;

public:
    CoW();

    CoW(const CoW &other);

    std::vector<uint32_t> const *const_get() const;

    std::vector<uint32_t> *get() const;
};


#endif //BIGINT_LAST_COW_H


#ifndef BIGINT_OPTIMIZED_MY_VECTOR_H
#define BIGINT_OPTIMIZED_MY_VECTOR_H


#include <cstring>
#include <memory>

struct my_vector {
    my_vector();

    my_vector(size_t size, uint32_t x);

    my_vector(my_vector const &v);

    void push_back(uint32_t value);

    void pop_back();

    std::size_t size() const;

    uint32_t &operator[](size_t ind);

    const uint32_t &operator[](size_t ind) const;

    const uint32_t &back() const;

    my_vector &operator=(my_vector rhs);

    bool operator==(const my_vector &rhs) const;

    void swap(my_vector &other);

    ~my_vector();

private:
    size_t _size;
    bool is_big;

    struct big_obj {
        size_t capacity;
        std::shared_ptr<uint32_t> data;

        big_obj() : capacity(0), data(nullptr) {
        }

        big_obj(uint32_t *p, size_t new_capacity) {
            data = std::shared_ptr<uint32_t>(p, std::default_delete<uint32_t[]>());
            capacity = new_capacity;
        }

        big_obj(const std::shared_ptr<uint32_t> &p, size_t new_capacity) {
            data = p;
            capacity = new_capacity;
        }

        ~big_obj() = default;
    };

    union container {
        uint32_t small;
        big_obj big;

        container() {
            small = 0;
        }

        ~container() {
//            big.~big_obj();
        };

    } data;

    void detach();

    void ensure_capasity();
};


#endif //BIGINT_OPTIMIZED_MY_VECTOR_H

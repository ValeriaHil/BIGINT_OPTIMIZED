
#include "my_vector.h"

my_vector::my_vector() {
    _size = 1;
    is_big = false;
    data.small = 0;
}

my_vector::my_vector(size_t size, uint32_t x) {
    this->_size = size;
    if (size <= 1) {
        data.small = x;
        is_big = false;
    } else {
        is_big = true;
        new(&data.big)big_obj(new uint32_t[size], size);
        std::fill(data.big.data.get(), data.big.data.get() + size, x);
    }
}

void my_vector::push_back(uint32_t value) {
    detach();

    if (!is_big && data.small == 0) {
        data.small = value;
    } else {
        if (!is_big || _size == data.big.capacity) {
            ensure_capasity();
        }
        data.big.data.get()[_size] = value;
        _size++;
    }
}

std::size_t my_vector::size() const {
    return _size;
}

uint32_t &my_vector::operator[](size_t ind) {
    if (is_big) {
        detach();
        return data.big.data.get()[ind];
    } else {
        return data.small;
    }
}

const uint32_t &my_vector::operator[](size_t ind) const {
    if (is_big) {
        return data.big.data.get()[ind];
    } else {
        return data.small;
    }
}

void my_vector::detach() {
    if (!is_big || data.big.data.unique()) {
        return;
    }
    auto *clone = new uint32_t[data.big.capacity];
    std::copy(data.big.data.get(), data.big.data.get() + data.big.capacity, clone);

    data.big.data = std::shared_ptr<uint32_t>(
            clone,
            std::default_delete<uint32_t[]>()
    );
}

const uint32_t &my_vector::back() const {
    if (is_big) {
        return data.big.data.get()[_size - 1];
    } else {
        return data.small;
    }
}

void my_vector::pop_back() {
    _size--;
}

my_vector::~my_vector() {
    if (is_big) {
        data.big.~big_obj();
    }
}

my_vector::my_vector(const my_vector &v) {
    _size = v._size;
    is_big = v.is_big;
    if (is_big) {
        new(&data.big)big_obj(v.data.big.data, v.data.big.capacity);
    } else {
        data.small = v.data.small;
    }
}

my_vector &my_vector::operator=(my_vector rhs) {
    swap(rhs);
    return *this;
}

void my_vector::ensure_capasity() {
    if (is_big) {
        auto *clone = new uint32_t[2 * data.big.capacity];
        std::copy(data.big.data.get(), data.big.data.get() + data.big.capacity, clone);

        data.big.data = std::shared_ptr<uint32_t>(
                clone,
                std::default_delete<uint32_t[]>()
        );
    } else {
        uint32_t clone = data.small;
        new(&data.big)big_obj(new uint32_t[2], 1);
        data.big.data.get()[0] = clone;
        is_big = true;
    }
    data.big.capacity *= 2;
}

bool my_vector::operator==(const my_vector &rhs) const {
    if (_size != rhs.size()) {
        return false;
    }
    for (size_t i = 0; i < _size; i++) {
        if (operator[](i) != rhs[i]) {
            return false;
        }
    }
    return true;
}

void my_vector::swap(my_vector &other) {
    std::swap(is_big, other.is_big);
    std::swap(_size, other._size);

    char tmp[sizeof(container)];
    memcpy(tmp, &data, sizeof(container));
    memcpy(&data, &other.data, sizeof(container));
    memcpy(&other.data, tmp, sizeof(container));

}




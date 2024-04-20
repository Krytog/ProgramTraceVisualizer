#pragma once

#include <utility>
#include <cmath>

namespace {
template <typename T>
[[nodiscard]] static bool HasChanged(T lhs, T rhs) {
    return lhs == rhs;
}

template <>
[[nodiscard]] bool HasChanged(float lhs, float rhs) {
    const constexpr float kPrecision = 1e-6;
    return std::abs(lhs - rhs) >= kPrecision;
}

template <typename T>
[[nodiscard]] bool HasChangedMultiple(T* lhs, T* rhs, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        if (HasChanged(lhs[i], rhs[i])) {
            return true;
        }
    }
    return false;
}
};

template <typename T>
struct ValueWithPrev {
    T cur_value_;
    T prev_value_;
};

template <typename T>
[[nodiscard]] static std::pair<bool, T> GetValueAndChange(ValueWithPrev<T>& value) {
    if (HasChanged(value.cur_value_, value.prev_value_)) {
        value.prev_value_ = value.cur_value_;
        return {true, value.cur_value_};
    }
    return {false, value.cur_value_};
}

template <typename T>
[[nodiscard]] std::pair<bool, T[]> GetValueAndChange(ValueWithPrev<T[]>& value, size_t count) {
    if (HasChangedMultiple(value.cur_value_, value.prev_value_, count)) {
        value.prev_value_ = value.cur_value_;
        return {true, value.cur_value_};
    }
    return {false, value.cur_value_};
}
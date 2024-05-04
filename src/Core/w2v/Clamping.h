#pragma once

#include <cstddef>
#include <vector>

class Clamper {
public:
    Clamper(const std::vector<double>& mins, const std::vector<double>& maxs) : mins_(mins), maxs_(maxs) {
    }

    void ClampValue(std::vector<double>& value) {
        for (size_t i = 0; i < value.size(); ++i) {
            value[i] = 2 * (value[i] - mins_[i]) / (maxs_[i] - mins_[i]) - 1;
        }
    }

private:
    const std::vector<double>& mins_;
    const std::vector<double>& maxs_;
};
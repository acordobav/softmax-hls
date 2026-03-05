#pragma once

constexpr int N = 100;

extern "C" {
    void softmax_top(const float* in, float* out);
}
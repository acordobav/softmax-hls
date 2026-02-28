#include "softmax.h"
#include <iostream>
#include <cmath>

int main() {

    float in[N];
    float out[N];

    // Initialize input values
    for (int i = 0; i < N; i++) {
        in[i] = i * 0.01f;
    }

    // Call the DUT
    softmax_top(in, out);

    // Verify output sum = 1
    float sum = 0.0f;

    for (int i = 0; i < N; i++) {
        sum += out[i];
        std::cout << "out[" << i << "] = " << out[i] << std::endl;
    }

    std::cout << "Sum = " << sum << std::endl;

    return 0;
}
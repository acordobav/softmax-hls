#include "softmax.h"
#include <hls_math.h>

extern "C" {

void softmax_top(const float* in, float* out) {

//Interface pragmas for HLS
#pragma HLS INTERFACE m_axi port=in  offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=out offset=slave bundle=gmem

#pragma HLS INTERFACE s_axilite port=in
#pragma HLS INTERFACE s_axilite port=out
#pragma HLS INTERFACE s_axilite port=return

    // Local buffer for exponentials and sum
    float sum = 0.0f;
    float exp_buffer[N];

    // Compute exponentials and accumulate sum
    for (int i = 0; i < N; i++) {
        exp_buffer[i] = hls::expf(in[i]);
        sum += exp_buffer[i];
    }

    // Normalize to get softmax output
    for (int i = 0; i < N; i++) {
        out[i] = exp_buffer[i] / sum;
    }
}

}
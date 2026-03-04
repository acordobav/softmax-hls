#include "softmax.h"
#include <hls_math.h>

extern "C" {

void softmax_top(const float* in, float* out) {
#pragma HLS INTERFACE m_axi port=in  bundle=gmem0 depth=N offset=slave
#pragma HLS INTERFACE m_axi port=out bundle=gmem1 depth=N offset=slave
#pragma HLS INTERFACE s_axilite port=return

    float sum = 0.0f;
    float exp_buffer[N];
#pragma HLS ARRAY_PARTITION variable=exp_buffer cyclic factor=16 dim=1

    // Compute exponentials and accumulate
    exp_loop: for (int i = 0; i < N; i++) {
#pragma HLS PIPELINE II=1
#pragma HLS UNROLL factor=16
        exp_buffer[i] = hls::expf(in[i]);
        sum += exp_buffer[i];
    }

    // Normalize
    float inv_sum = 1.0f / sum;
    norm_loop: for (int i = 0; i < N; i++) {
#pragma HLS PIPELINE II=1
#pragma HLS UNROLL factor=16
        out[i] = exp_buffer[i] * inv_sum;
    }
}

}
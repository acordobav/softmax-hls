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

    float sum = 0.0f;
    float exp_buffer[N];
    
#pragma HLS array_partition variable=exp_buffer cyclic factor=4 dim=1
#pragma HLS bind_storage variable=exp_buffer type=ram_2p impl=bram

    // Compute exponentials and accumulate sum
    for (int i = 0; i < N; i++) {
#pragma HLS PIPELINE II=1
#pragma HLS allocation function instances=expf limit=8
        exp_buffer[i] = hls::expf(in[i]);
        sum += exp_buffer[i];
    }

    // Normalize to get softmax output
    for (int i = 0; i < N; i++) {
#pragma HLS PIPELINE II=1
#pragma HLS unroll factor=8
        out[i] = exp_buffer[i] / sum;
    }
}

}

#pragma once
#include <gnuradio/kernels/common/kernel.hpp>
#include <cstdint>

namespace gr {
namespace kernels {
namespace analog {
namespace cpu {

template <typename T>
struct agc : kernel_interface {
public:
    agc(float rate, float reference, float gain, float max_gain)
        : _rate(rate), _reference(reference), _gain(gain), _max_gain(max_gain){};

    void operator()(void* in_buffer, void* out_buffer, size_t num_items);
    void operator()(void* buffer, size_t num_items);

protected:
    float _rate;      // adjustment rate
    float _reference; // reference value
    float _gain;      // current gain
    float _max_gain;  // max allowable gain
};

template struct agc<float>;
// template struct agc<gr_complex>;

} // namespace cpu
} // namespace analog
} // namespace kernels
} // namespace gr

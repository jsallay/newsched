#pragma once
#include <gnuradio/kernels/common/kernel.hpp>
#include <complex>
#include <cstdint>

namespace gr {
namespace kernels {
namespace analog {
namespace cpu {

/**
 * @brief Automatic Gain Control
 * \ingroup level_controllers
 *
 *
 */
struct agc_ff : kernel_interface {
public:
    agc_ff(float rate, float reference, float gain, float max_gain)
        : _rate(rate), _reference(reference), _gain(gain), _max_gain(max_gain){};

    void operator()(void* in_buffer, void* out_buffer, size_t num_items);
    void operator()(void* buffer, size_t num_items)
    {
        operator()(buffer, buffer, num_items);
    };

protected:
    float _rate;      // adjustment rate
    float _reference; // reference value
    float _gain;      // current gain
    float _max_gain;  // max allowable gain
};

} // namespace cpu
} // namespace analog
} // namespace kernels
} // namespace gr

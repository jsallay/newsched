#include <gnuradio/kernels/cpu/agc.hpp>
#include <cmath>

namespace gr {
namespace kernels {
namespace analog {
namespace cpu {

template <>
void agc<float>::operator()(void* in_buffer, void* out_buffer, size_t num_items)
{
    float* in = reinterpret_cast<float*>(in_buffer);
    float* out = reinterpret_cast<float*>(out_buffer);
    for (unsigned int i = 0; i < num_items; i++) {
        out[i] = in[i] * _gain;
        _gain += (_reference - fabsf(out[i])) * _rate;
        if (_max_gain > 0.0 && _gain > _max_gain)
            _gain = _max_gain;
    }
}

template <>
void agc<float>::operator()(void* buffer, size_t num_items)
{
    this->operator()(buffer, buffer, num_items);
}

// template <>
// void agc<gr_complex>::operator()(gr_complex* in_buffer,
//                                  gr_complex* out_buffer,
//                                  size_t num_items)
// {
//     for (unsigned int i = 0; i < num_items; i++) {
//         out_buffer[i] = in_buffer[i] * _gain;
//         _gain += _rate * (_reference - std::sqrt(out_buffer.real() * out_buffer.real()
//         +
//                                                  out_buffer.imag() *
//                                                  out_buffer.imag()));
//         if (_max_gain > 0.0 && _gain > _max_gain)
//             _gain = _max_gain;
//     }
// }

// template <>
// void agc<gr_complex>::operator()(gr_complex* buffer, size_t num_items)
// {
//     this->operator()(buffer, buffer, num_items);
// }

} // namespace cpu
} // namespace analog
} // namespace kernels
} // namespace gr
#include <gnuradio/kernels/cpu/agc.hpp>
#include <cmath>

namespace gr {
namespace kernels {
namespace analog {
namespace cpu {

void agc_ff::operator()(void* in_buffer, void* out_buffer, size_t num_items)
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

// void agc<std::complex<double>>::operator()(void* in_buffer,
//                                            void* out_buffer,
//                                            size_t num_items)
// {
//     std::complex<double>* in = reinterpret_cast<std::complex<double>*>(in_buffer);
//     std::complex<double>* out = reinterpret_cast<std::complex<double>*>(out_buffer);
//     for (unsigned int i = 0; i < num_items; i++) {
//         out[i] = in[i] * _gain;
//         _gain += _rate * (_reference - std::sqrt(out[i].real() * out[i].real() +
//                                                  out[i].imag() * out[i].imag()));
//         if (_max_gain > 0.0 && _gain > _max_gain)
//             _gain = _max_gain;
//     }
// }

} // namespace cpu
} // namespace analog
} // namespace kernels
} // namespace gr
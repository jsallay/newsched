/* -*- c++ -*- */
/*
 * Copyright 2004,2009,2010,2012,2018 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include "pfb_channelizer_cuda.hh"

#include <gnuradio/helper_cuda.h>

namespace gr {
namespace filter {

template <class T>
typename pfb_channelizer<T>::sptr
pfb_channelizer<T>::make_cuda(const pfb_channelizer<T>::block_args& args)
{
    return std::make_shared<pfb_channelizer_cuda<T>>(args);
}

template <class T>
pfb_channelizer_cuda<T>::pfb_channelizer_cuda(
    const typename pfb_channelizer<T>::block_args& args)
    : pfb_channelizer<T>(args),
    d_nchans(args.numchans)
{
    d_in_items.resize(1);
    d_out_items.resize(d_nchans);

    auto new_taps = std::vector<gr_complex>(args.taps.size());
    for (size_t i = 0; i < args.taps.size(); i++) {
        new_taps[i] = gr_complex(args.taps[i], 0);
    }

    // quantize the overlap to the nchans
    d_overlap = d_nchans * ((args.taps.size() + d_nchans - 1) / d_nchans);
    checkCudaErrors(cudaMalloc(&d_dev_tail, d_overlap * sizeof(gr_complex)));
    checkCudaErrors(cudaMemset(d_dev_tail, 0, d_overlap * sizeof(gr_complex)));

    checkCudaErrors(
        cudaMalloc(&d_dev_buf, 16 * 1024 * 1024 * sizeof(gr_complex))); // 4M items max ??

    p_channelizer =
        std::make_shared<cusp::channelizer<gr_complex>>(new_taps, d_nchans);
    cudaStreamCreate(&d_stream);
    p_channelizer->set_stream(d_stream);

    p_deinterleaver =
        std::make_shared<cusp::deinterleave>(d_nchans, 1, sizeof(gr_complex));

    p_deinterleaver->set_stream(d_stream);
    // set_output_multiple(nchans);
    // set_min_noutput_items(d_overlap+1024);
}

template <class T>
work_return_code_t
pfb_channelizer_cuda<T>::work(std::vector<block_work_input>& work_input,
                              std::vector<block_work_output>& work_output)
{
    // std::scoped_lock guard(d_mutex);

    auto noutput_items = work_output[0].n_items;
    auto ninput_items = work_input[0].n_items;

    if ((size_t)ninput_items < noutput_items * d_nchans + d_overlap)
    {
        return work_return_code_t::WORK_INSUFFICIENT_INPUT_ITEMS;
    }

    size_t idx = 0;
    for (auto& wi : work_input) {
        d_in_items[idx++] = wi.items();
    }

    idx = 0;
    for (auto& wo : work_output) {
        d_out_items[idx++] = wo.items();
    }

    checkCudaErrors(p_channelizer->launch_default_occupancy(
        d_in_items , { d_dev_buf }, (noutput_items + d_overlap / d_nchans)));

    checkCudaErrors(p_deinterleaver->launch_default_occupancy(
        { (gr_complex*)d_dev_buf + d_overlap }, d_out_items, noutput_items * d_nchans));

    cudaStreamSynchronize(d_stream);

    this->consume_each(noutput_items * d_nchans, work_input);
    this->produce_each(noutput_items, work_output);
    return work_return_code_t::WORK_OK;
}

// template class pfb_channelizer<float>;
template class pfb_channelizer<gr_complex>;

} /* namespace filter */
} /* namespace gr */
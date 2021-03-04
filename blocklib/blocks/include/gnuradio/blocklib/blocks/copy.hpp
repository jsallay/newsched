#pragma once

#include <gnuradio/sync_block.hpp>
#include <gnuradio/kernels/cpu/copy.hpp>
                                    
namespace gr {
    namespace blocks {

    class copy : public sync_block
    {
    public:
        enum params : uint32_t { id_nports, num_params };
        typedef std::shared_ptr<copy> sptr;

        static sptr make(size_t itemsize)
        {
            auto ptr = std::make_shared<copy>(itemsize);

            ptr->add_port(untyped_port::make(
                "input", port_direction_t::INPUT, itemsize
            ));

            ptr->add_port(untyped_port::make(
                "out", port_direction_t::OUTPUT, itemsize
            ));

            return ptr;
        }

        copy(size_t itemsize) : sync_block("copy"), _itemsize(itemsize)
        {
            block_kernel = nullptr;
        }

        virtual work_return_code_t work(std::vector<block_work_input>& work_input,
                                        std::vector<block_work_output>& work_output)
        {
            unsigned int batch_size = work_input.size();
            for (unsigned int idx = 0; idx < batch_size; idx++){
                block_kernel(work_input[idx].items, work_output[idx].items, work_input[idx].n_items);
            }

            return work_return_code_t::WORK_OK;
        }

    private:
        size_t _itemsize;
    };
    } // namespace blocks
} // namespace gr

#pragma once

#include <gnuradio/buffer.hpp>
#include <gnuradio/graph.hpp>
#include <gnuradio/node.hpp>

namespace gr {

/**
 * @brief Defines whether the buffer is held by this domain adapter, or the connected one
 *
 */
enum class buffer_location_t { LOCAL = 0, REMOTE };

/**
 * @brief Establishes a preference for whether buffer will be constructed on the upstream
 * or downstream block
 *
 */
enum class buffer_preference_t { UPSTREAM, DOWNSTREAM };

enum class da_request_t : uint32_t {
    WRITE_INFO = 0,
    READ_INFO,
    POST_WRITE,
    POST_READ,
    GET_REMOTE_BUFFER
};

enum class da_response_t : uint32_t { OK = 0, ERROR = 1 };

/**
 * @brief Domain Adapter used internally by flowgraphs to handle domain crossings
 *
 * The Domain Adapter is both a node in that it is connected to blocks at the edges of a
 * subgraph as well as a buffer, since it is used for the scheduler to get the address
 * needed to read from or write to
 *
 * It holds a pointer to a buffer object which may be null if the adapter is not hosting
 * the buffer and relying on its peer to host the buffer
 */
class domain_adapter : public node, public buffer
{
protected:
    buffer_sptr _buffer = nullptr;
    buffer_location_t _buffer_loc;

    domain_adapter(buffer_location_t buf_loc, const std::string& name = "domain_adapter")
        : node(name), gr::buffer(0,0), _buffer_loc(buf_loc)
    {
    }

public:
    virtual ~domain_adapter() {}
    void set_buffer(buffer_sptr buf) { _buffer = buf; }
    buffer_sptr buffer() { return _buffer; }

    buffer_location_t buffer_location() { return _buffer_loc; }
    void set_buffer_location(buffer_location_t buf_loc) { _buffer_loc = buf_loc; }
};

typedef std::shared_ptr<domain_adapter> domain_adapter_sptr;


/**
 * @brief Domain Adapter Configuration
 *
 * A configuration used to generate a pair of domain adapters that will replace the edge
 * between two blocks
 *
 */
class domain_adapter_conf
{
protected:
    domain_adapter_conf(buffer_preference_t buf_pref) : _buf_pref(buf_pref) {}
    buffer_preference_t _buf_pref;

public:
    virtual ~domain_adapter_conf() {}
    virtual std::pair<domain_adapter_sptr, domain_adapter_sptr> make_domain_adapter_pair(
        port_sptr upstream_port, port_sptr downstream_port, const std::string& name = "")
    {
        throw std::runtime_error("Cannot create domain adapter pair from base class");
    };
};

typedef std::shared_ptr<domain_adapter_conf> domain_adapter_conf_sptr;
typedef std::vector<std::tuple<edge_sptr, domain_adapter_conf_sptr>>
    domain_adapter_conf_per_edge;


} // namespace gr

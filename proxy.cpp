#include <zmq.hpp>

int main()
{
    zmq::context_t ctx(1);
    zmq::socket_t xpub(ctx, zmq::socket_type::xpub);
    xpub.bind("tcp://127.0.0.1:24000");
    zmq::socket_t xsub(ctx, zmq::socket_type::xsub);
    xsub.bind("tcp://127.0.0.1:23000");
    zmq::proxy(xpub, xsub, 0);
    return 0;
}

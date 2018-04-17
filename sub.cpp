#include <zmq.hpp>
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <iostream>

bool poll(zmq::socket_t &sock)
{
#ifdef __GNUC__
    zmq::pollitem_t items[] = {{static_cast<void*>(sock), 0, ZMQ_POLLIN, 0}};
#else
    zmq::pollitem_t items[] = {{sock, 0, ZMQ_POLLIN, 0}};
#endif
    zmq::poll(&items[0], sizeof(items) / sizeof(items[0]), 0);
    return items[0].revents & ZMQ_POLLIN;
}

int main()
{
    zmq::context_t ctx(1);
    zmq::socket_t sub(ctx, zmq::socket_type::sub);
    sub.setsockopt(ZMQ_CONFLATE, 1);
    sub.connect("tcp://127.0.0.1:24000");
    std::string topic = "hello-";
    sub.setsockopt(ZMQ_SUBSCRIBE, topic.data(), topic.size());

    while(1)
    {
        while(poll(sub))
        {
            zmq::message_t msgh;
            sub.recv(&msgh);

            zmq::message_t msg;
            sub.recv(&msg);

            std::string msg_str(reinterpret_cast<const char*>(msg.data()), msg.size());
            std::cout << "received \"" << msg_str << "\"" << std::endl;

            // simulate a busy receiver here:
            boost::this_thread::sleep_for(boost::chrono::seconds{1});
        }
        std::cout << "..." << std::endl;
        boost::this_thread::sleep_for(boost::chrono::seconds{2});
    }
    return 0;
}

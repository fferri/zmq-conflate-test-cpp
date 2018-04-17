#include <zmq.hpp>
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <iostream>

int main()
{
    zmq::context_t ctx(1);
    zmq::socket_t pub(ctx, zmq::socket_type::pub);
    //pub.setsockopt(ZMQ_CONFLATE, 1);
    pub.connect("tcp://127.0.0.1:23000");
    std::string topic = "hello-";

    int i = 0;
    while(1)
    {
        std::string msg_str = (boost::format("hello-%d") % i).str();
        std::cout << "sending \"" << msg_str << "\"..." << std::endl;

        zmq::message_t msgh(topic.data(), topic.size());
        pub.send(msgh, ZMQ_SNDMORE);

        zmq::message_t msg(msg_str.data(), msg_str.size());
        pub.send(msg);

        boost::this_thread::sleep_for(boost::chrono::milliseconds{20});
        i++;
    }
    return 0;
}

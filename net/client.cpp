#include "client.h"
using namespace netcore;

Client::Client(EventLoop *loop)
	:loop_(loop),
	connector_(new Connector(loop)),
    connected_(false)
{

}

Client::~Client()
{
	stop();
}

void Client::start(const NetAddr & serveraddr)
{
	connector_->setConnectionCallback(connectionCallback_);
	connector_->setMessageCallback(messageCallback_);
	connector_->connect(serveraddr);
    connected_ = true;
}

void Client::stop()
{
	if (connected_)
    {
        connected_ = false;
        connector_->disconnect();
    }
    
}

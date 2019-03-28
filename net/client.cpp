#include "client.h"
using namespace netcore;

Client::Client(EventLoop *loop)
	:loop_(loop),
	connector_(new Connector(loop))
{

}

Client::~Client()
{
	stop();
}

void Client::start(const NetAddr & serveraddr)
{
	connector_->connect(serveraddr);
}

void Client::stop()
{
	connector_->disconnect();
}

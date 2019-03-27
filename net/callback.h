#ifndef __CALLBACK_H
#define __CALLBACK_H
#include <iostream>
#include <memory>
#include <functional>
namespace netcore
{
	class Connection;
	typedef std::shared_ptr<Connection> ConnectionPtr;
	typedef std::function<void(const ConnectionPtr & conn)> ConnectionCallback;
	typedef std::function<void(const ConnectionPtr & conn)> MessageCallback;
	typedef std::function<void(const ConnectionPtr & conn)> ClosedCallback;
}

#endif
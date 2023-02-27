#pragma once
#include "ClientServer.hpp"

namespace server
{
	struct ConnectionData : private boost::noncopyable
	{
		io__::ip::tcp::socket socket;
		std::string data;
		explicit ConnectionData(io__::io_service& ios);
		void shutdown();
		~ConnectionData();
	};
}



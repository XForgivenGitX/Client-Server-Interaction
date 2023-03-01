#pragma once
#include "ClientServer.hpp"

namespace server
{
	struct ConnectionData //: private boost::noncopyable
	{
		io__::ip::tcp::socket socket;
		std::string data;
		explicit ConnectionData(io__::io_service& ios);
		void shutdown();
		~ConnectionData();
	};
	typedef std::unique_ptr<ConnectionData> ConnectionDataPtr;
	
	void handler_server_send(ConnectionDataPtr&& dataPtr, const boost::system::error_code& error);
}



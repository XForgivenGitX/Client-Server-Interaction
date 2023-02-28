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

	template <typename Func>
	void async_write_data(ConnectionDataPtr&& dataPtr, const Func& func);
	
	void send(const std::string& address, const unsigned short port, const std::string& data);	
	void on_send(const boost::system::error_code& error, std::size_t bytes_transferred);
}



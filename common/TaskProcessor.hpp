#pragma once
#include <string>

#include <boost/asio/io_service.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/asio/write.hpp>


namespace io = boost::asio;

namespace detail
{
	template <typename T>
	struct Singleton : boost::noncopyable
	{
		static T& get_instance()
		{
			static T instance;
			return instance;
		}
	protected:
		Singleton() = default;
	};
}

namespace server 
{
	namespace used
	{
		using boost::asio::ip::address_v4;
		using boost::asio::async_write;
		using boost::asio::buffer;
	}
	
	class task_processor : public detail::Singleton<task_processor>
	{
		friend Singleton<task_processor>;
		task_processor() = default;
	
	protected:
		static used::io_service& get_ios()
		{
			static io::io_service ios;
			static io::io_service::work work(ios);
			return ios;
		}
		//______
	public:
		template <typename T>
		static void push_task(const T& task_unwrapped)
		{
			//get_ios().post(make_task_wrapped(task_unwrapped));
		}

		static void start() { get_ios().run(); }
		static void stop() { get_ios().stop(); }
		//______
	public: 
		//static connection_ptr create_connection(std::string address, unsigned short port_num)
		//{
			//connection_ptr connect = std::make_unique<connection_data>(get_ios());
			//connect->socket.connect(io::ip::tcp::endpoint(io::ip::address_v4::from_string(address), port_num));
			//return connect;
	//	}
	};
}

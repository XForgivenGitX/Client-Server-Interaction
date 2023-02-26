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
		static T& get_instance();
	protected:
		Singleton() = default;
	};

	class TaskProcessor : public detail::Singleton<TaskProcessor>
	{
		friend Singleton<TaskProcessor>;
		TaskProcessor() = default;

	protected:
		static io::io_service &get_ios();
		
	public:
		template <typename T>
		static void push_task(const T &task_unwrapped);
		static void start() { get_ios().run(); }
		static void stop() { get_ios().stop(); }

	public:
		// static connection_ptr create_connection(std::string address, unsigned short port_num)
		//{
		// connection_ptr connect = std::make_unique<connection_data>(get_ios());
		// connect->socket.connect(io::ip::tcp::endpoint(io::ip::address_v4::from_string(address), port_num));
		// return connect;
		//	}
	};
}

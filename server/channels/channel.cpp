#include "channel.hpp"
#include "send_receive.hpp"

namespace server
{
	void channel::enter_user(anet::socket_data_ptr &socketData)
	{
		insert_socket(socketData);
	}

	void channel::leave_user(anet::socket_data_ptr &socketData)
	{
		erase_socket(socketData);
	}

	void channel::leave_all()
	{
		for_each_socket([this](auto socketData)
						{ leave_user(socketData); });
	}

	void channel::send_all(const common::msg_type &msg)
	{
		for_each_socket([&msg, this](auto socketData)
						{
				socketData->send_buffer_ = msg;
				anet::send_receive::send(socketData, {&channel::send_message_handler, this}); });
	}

	void channel::send_message_handler(anet::socket_data_ptr &socketData, const anet::err_c &error_c)
	{
		if (error_c)
		{
			leave_user(socketData);
		}
	}
}
#include "channel.hpp"
#include "send_receive.hpp"

namespace server
{
	void channel::enter_user(anet::socket_data_ptr &socketData, IChat_member_ptr member)
	{
		insert_socket(socketData, member);
	}

	void channel::leave_user(channelMembers &socketMember_)
	{
		auto& [socket, member] = socketMember_;
		member->send_command
				(const_cast<anet::socket_data_ptr &>(socket), common::command::SUCCESS_DETACH_ROOM);
		erase_socket(socket);
		
	}

	void channel::leave_all()
	{
		for_each_member([this](auto& socketData)
						{ leave_user(socketData); });
	}

	void channel::send_all(const common::msg_type &msg)
	{
		for_each_member([&msg, this](auto& socketData)
						{
				socketData.first->send_buffer_ = msg;
				anet::send_receive::send(socketData.first, {&channel::send_message_handler, this}); });
	}

	void channel::send_message_handler(anet::socket_data_ptr &socketData, const anet::err_c &error_c)
	{
		if (error_c)
		{
			leave_user(socketData);
		}
	}
}
#include "channels.hpp"

namespace server
{
	ordinary_channel::ordinary_channel(const IConnections_manager_ptr &connectionManager)
		: IChannel(connectionManager) {}

	void ordinary_channel::enter_user(const anet::socket_data_ptr &socketData, const IChat_user_ptr &user)
	{
		auto newMember = std::make_unique<ordinary_channel_member>(shared_from_this(), user);
		newMember->send_command(socketData, common::command::SUCCESS_JOIN_ROOM);
		insert_chat_member(socketData, std::move(newMember));
		send_everyone("new member: " + user->get_name());
	}

	void ordinary_channel::normal_leave_user(const anet::socket_data_ptr &socketData)
	{
		erase_chat_member(socketData);
		connectionManager_->send_command(socketData, common::command::SUCCESS_DETACH_ROOM);
	}

	void ordinary_channel::forced_leave_user(const anet::socket_data_ptr &socketData)
	{
		erase_chat_member(socketData);
		connectionManager_->leave_user(socketData);
	}

	void ordinary_channel::leave_everyone()
	{
		for_each_member([this](auto &socketMember)
		{
			auto& [socket, member] = socketMember;
			normal_leave_user(socket); 
		});
	}

	void ordinary_channel::send_everyone(const common::msg_type &msg)
	{
		const auto assembledMsg = common::assemble_package(common::command::RECEIVE_MESSAGE, {msg});
		for_each_member([this, &assembledMsg](auto &socketMember)
		{
			auto& [socket, member] = socketMember;
			socket->send_buffer_ = assembledMsg;
			anet::send_receive::send(socket, {&IChannel_member::send_command_handler, member}); 
		});
		add_message_to_history(msg);
	}

	void ordinary_channel::send_history(const anet::socket_data_ptr& socketData, IChannel_member* recipient)
	{
		for_each_history([&socketData, recipient](const auto& msg)
		{	
			socketData->send_buffer_ = common::assemble_package(common::command::RECEIVE_MESSAGE, {msg});
			anet::send_receive::send(socketData, {&IChannel_member::send_command_handler, recipient});
		});
	}
}
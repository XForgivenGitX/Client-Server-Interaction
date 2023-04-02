#include "users.hpp"

namespace server
{
	ordinary_chat_user::ordinary_chat_user(const common::name_t &name, const common::pass_t &pass)
		: IChat_user(name, pass) {}

	common::name_t ordinary_chat_user::get_name() const
	{
		return name_;
	}

	common::pass_t ordinary_chat_user::get_pass() const
	{
		return pass_;
	}

	ordinary_channel_member::ordinary_channel_member(const IChannel_ptr &myChannel, const IChat_user_ptr &myData)
		: IChannel_member(myChannel, myData) {}

	void ordinary_channel_member::send_command
			(const anet::socket_data_ptr &socketData, common::command cmd)
	{
		socketData->send_buffer_ = common::assemble_package(cmd);
		anet::send_receive::send(socketData, {&ordinary_channel_member::send_command_handler, this});
	}

	void ordinary_channel_member::start_receiving_messages(const anet::socket_data_ptr &socketData)
	{
		anet::send_receive::receive(socketData, {&ordinary_channel_member::receive_command_handler, this});
	}

	void ordinary_channel_member::send_command_handler
			(anet::socket_data_ptr &socketData, const anet::err_c &error_c)
	{
		if (error_c)
		{
			myChannel_->forced_leave_user(socketData);
		}
	}

	void ordinary_channel_member::receive_command_handler
			(anet::socket_data_ptr &socketData, const anet::err_c &error_c)
	{
		using namespace common;
		transf_package splitedPack;
		splitedPack.disassemble(socketData->receive_buffer_);

		if (error_c || !splitedPack.isMatched())
		{
			myChannel_->forced_leave_user(socketData);
			return;
		}
		switch (splitedPack.get_command())
		{
			case command::SEND_MESSAGE:
			{	
				auto msg = splitedPack.get_arg(protocol::MESSAGE_INDEX);
				myChannel_->send_everyone(configure_message(msg));
				anet::send_receive::receive(socketData, {&ordinary_channel_member::receive_command_handler, this});
			}
			break;

			case command::DETACH_ROOM:
			{
				myChannel_->normal_leave_user(socketData);
			}
			break;

			case command::GET_CHANNEL_HISTORY:
			{
				myChannel_->send_history(socketData, this);
			}
			break;

			default:
			{
				myChannel_->forced_leave_user(socketData);
			}
			break;

		}
	}

	common::msg_type ordinary_channel_member::configure_message(const common::msg_type &usersMessage) const
	{
    	auto datetime = boost::posix_time::second_clock::universal_time();
    	std::ostringstream configuredMessage;
    	configuredMessage << "[" << datetime << "] " << myData_->get_name() << ": " << usersMessage;
    	return configuredMessage.str();
	}
}
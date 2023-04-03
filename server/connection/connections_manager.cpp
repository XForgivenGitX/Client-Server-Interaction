#include "connections_manager.hpp"

namespace server
{
	void connections_manager::enter_user(const anet::socket_data_ptr &socketData)
	{
#ifdef CONNECT_ENABLE_HANDLER_TRACKING
		BOOST_LOG_TRIVIAL(CONNECT_LOG_SEVERITY)
			<< lg::build_src("enter user")
#ifdef CONNECT_DETAIL_LOG
			<< lg::build_arg("socket  : " + std::to_string(socketData->get_handle()))
#endif
			;
#endif
		insert_socket(socketData);
		anet::send_receive::receive(socketData, {&connections_manager::receive_command_handler, this});
	}

	void connections_manager::leave_user(const anet::socket_data_ptr &socketData)
	{
#ifdef CONNECT_DETAIL_LOG
		auto handle = socketData->get_handle();
#endif
		anet::err_c error_c;
		socketData->shutdown(error_c);
		erase_socket(socketData);

#ifdef CONNECT_ENABLE_HANDLER_TRACKING
		BOOST_LOG_TRIVIAL(warning)
			<< lg::build_src("leave user")
#ifdef CONNECT_DETAIL_LOG
			<< lg::build_arg("socket  : " + std::to_string(handle))
			<< lg::build_arg("status  : " + error_c.message());
#endif
			;
#endif
	}

	void connections_manager::leave_everyone()
	{
		for_each_socket([&](auto &socketUserPair)
		{
            auto& [socketData, chatMember] = socketUserPair;
			leave_user(socketData);
		});
	}

	connections_manager::~connections_manager()
	{
		leave_everyone();
	}

	void connections_manager::send_command(const anet::socket_data_ptr &socketData, cmd_type cmd)
	{
		socketData->send_buffer_ = common::assemble_package(cmd);
		anet::send_receive::send(socketData, {&connections_manager::send_command_handler, this});
	}


	void connections_manager::send_command_handler(anet::socket_data_ptr &socketData, const anet::err_c &error_c) noexcept
	{
#ifdef CONNECT_ENABLE_HANDLER_TRACKING
		BOOST_LOG_TRIVIAL(CONNECT_LOG_SEVERITY)
			<< lg::build_src("send command handler")
#ifdef CONNECT_DETAIL_LOG
			<< lg::build_arg("socket  : " + std::to_string(socketData->get_handle()))
			<< lg::build_arg("package : " + socketData->send_buffer_)
			<< lg::build_arg("status  : " + error_c.message())
#endif
			;
#endif
		if (error_c)
		{
			leave_user(socketData);
		}
		else
		{
			anet::send_receive::receive(socketData, {&connections_manager::receive_command_handler, this});
		}
	}

	void connections_manager::receive_command_handler(anet::socket_data_ptr &socketData, const anet::err_c &error_c) noexcept
	{
		using namespace common;
		transf_package splitedPack;
		splitedPack.disassemble(socketData->receive_buffer_);

#ifdef CONNECT_ENABLE_HANDLER_TRACKING
		BOOST_LOG_TRIVIAL(CONNECT_LOG_SEVERITY)
			<< lg::build_src("receive command handler")
#ifdef CONNECT_DETAIL_LOG
			<< lg::build_arg("socket  : " + std::to_string(socketData->get_handle()))
			<< lg::build_arg("package : " + socketData->receive_buffer_)
			<< lg::build_arg("command : " + lg::command_to_str(splitedPack.get_command()))
			<< lg::build_arg("match   : " + lg::boolalpha_cast(splitedPack.isMatched()))
			<< lg::build_arg("status  : " + error_c.message())
#endif
			;
#endif
		if (error_c || !splitedPack.isMatched())
		{
			leave_user(socketData);
			return;
		}
		switch (splitedPack.get_command())
		{
		case command::REGISTER:
		{
			auto name = splitedPack.get_arg(protocol::USER_NAME_INDEX);
			auto pass = splitedPack.get_arg(protocol::USER_PASS_INDEX);
			if (!is_user_name(name))
			{
				auto newUser = std::make_shared<ordinary_chat_user>(name, pass);
				insert_user(newUser);
				insert_socket(socketData, newUser);
				send_command(socketData, command::SUCCESS_REGISTER);
			}
			else
			{
				send_command(socketData, command::ERROR_REGISTER);
			}
		}
		break;

		case command::LOG_IN:
		{
			if (auto userDataOpt = find_user_data(
					splitedPack.get_arg(protocol::USER_NAME_INDEX),
					splitedPack.get_arg(protocol::USER_PASS_INDEX));
				userDataOpt)
			{
				insert_socket(socketData, userDataOpt.value());
				send_command(socketData, command::SUCCESS_LOG_IN);
			}
			else
			{
				send_command(socketData, command::ERROR_LOG_IN);
			}
		}
		break;

		case command::CREATE_ROOM:
		{
			auto roomName = splitedPack.get_arg(protocol::ROOM_NAME_INDEX);
			if (!is_channel_name(roomName))
			{
				auto newRoom = std::make_shared<ordinary_channel>(shared_from_this());
				insert_channel(roomName, newRoom);
				send_command(socketData, command::SUCCESS_CREATE_ROOM);
			}
			else
			{
				send_command(socketData, command::ERROR_CREATE_ROOM);
			}
		}
		break;

		case command::JOIN_ROOM:
		{
			auto roomName = splitedPack.get_arg(protocol::ROOM_NAME_INDEX);
			if (auto channelOpt = find_channel(roomName); channelOpt)
			{
				channelOpt.value()->
						enter_user(socketData, get_active_user_data(socketData));
			}
			else
			{
				send_command(socketData, command::ERROR_JOIN_ROOM);
			}
		}
		break;

		default:
		{
#ifdef CONNECT_ENABLE_HANDLER_TRACKING
			BOOST_LOG_TRIVIAL(error)
				<< lg::build_src("unknown command accepted");
#endif
			leave_user(socketData);
		}
		}
	}
}

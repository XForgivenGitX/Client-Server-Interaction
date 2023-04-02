#pragma once
#include <boost/date_time/posix_time/posix_time.hpp>

#include "interface.hpp"
#include "send_receive.hpp"
namespace server
{
	struct ordinary_chat_user : 
		public IChat_user
	{
		ordinary_chat_user(const common::name_t& name, const common::pass_t& pass);
		virtual ~ordinary_chat_user() = default;
	
	public:
		common::name_t get_name() const override;
		common::pass_t get_pass() const override;
	};
	
	struct ordinary_channel_member : 
		public IChannel_member
	{
		ordinary_channel_member(const IChannel_ptr& myChannel, const IChat_user_ptr& myData);
		virtual ~ordinary_channel_member() = default;
		common::msg_type configure_message(const common::msg_type &usersMessage) const;
	
	public:
		void start_receiving_messages(const anet::socket_data_ptr& socketData) override;
		
		void send_command(const anet::socket_data_ptr& socketData, common::command cmd);
		void send_command_handler(anet::socket_data_ptr& socketData, const anet::err_c& error_c);
		void receive_command_handler(anet::socket_data_ptr& socketData, const anet::err_c& error_c);
	};
}
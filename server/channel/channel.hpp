#pragma once
#include "interface.hpp"
#include "database.hpp"

namespace server
{
	struct channel : 
		public IChannel, 
		private db::channel_database
	{
		void enter_user(anet::socket_data_ptr& socketData, IChat_member_ptr) override;
		void leave_user(channelMember& socketData) override;
		void leave_all() override;
		void send_all(const common::msg_type& msg) override;

	private:
		void send_message_handler(anet::socket_data_ptr& socketData, const anet::err_c& error_c);
		
	};
}
#pragma once
#include <unordered_set>
#include "i_channels.hpp"
#include "database.hpp"

namespace server
{
	struct channel : 
		public IChannel, 
		private db::channel_database
	{
		void enter_user(anet::socket_data_ptr& socketData) override;
		void leave_user(anet::socket_data_ptr& socketData) override;
		void leave_all() override;
		void send_all(const common::msg_type& msg) override;

	private:
		void send_message_handler(anet::socket_data_ptr& socketData, const anet::err_c& error_c);
		
	};
}
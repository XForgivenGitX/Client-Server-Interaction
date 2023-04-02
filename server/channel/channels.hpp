#pragma once
#include "send_receive.hpp"
#include "database.hpp"
#include "users.hpp"

namespace server
{
	struct ordinary_channel : 
		public IChannel,
		private db::channel_database
	{
		ordinary_channel(const IConnections_manager_ptr& connectionManager);	
		virtual ~ordinary_channel() = default;
		
	public:
		void enter_user(const anet::socket_data_ptr& socketData, const IChat_user_ptr& user) override;
		void normal_leave_user(const anet::socket_data_ptr& socketData) override;
		void forced_leave_user(const anet::socket_data_ptr& socketData) override;
		void send_history(const anet::socket_data_ptr& socketData, IChannel_member* recipient) override;
		void leave_everyone() override;
		void send_everyone(const common::msg_type& msg) override;
	};
}
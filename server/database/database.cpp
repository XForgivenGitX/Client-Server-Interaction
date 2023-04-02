#include "database.hpp"

namespace db
{
	users_database::find_user_data_ret_v users_database::find_user_data
		(const common::name_t &name, const common::pass_t &pass) const
	{
		std::shared_lock lock(userDataMut);
		auto it = userData_.find(name);
		auto& [name_, user_] = *it;
		return ((it != userData_.end() && (user_->get_pass() == pass)) 
			? find_user_data_ret_v(user_) : std::nullopt);
	}

	bool users_database::is_user_name(const common::name_t &name) const
	{
		return userData_.find(name) != userData_.end();
	}

	void users_database::insert_user(server::IChat_user_ptr user)
	{
		std::unique_lock lock(userDataMut);
		auto [user_, status] = userData_.emplace(std::make_pair(user->get_name(), user));
#ifdef DATABASE_ENABLE_HANDLER_TRACKING
		lock.unlock();
		BOOST_LOG_TRIVIAL(DATABASE_LOG_SEVERITY)
			<< lg::build_src("user database: insert user")
#ifdef DATABASE_DETAIL_LOG
			<< lg::build_arg("name: " + user->get_name())
			<< lg::build_arg("pass: " + user->get_pass())
			<< lg::build_arg("status: " + lg::boolalpha_cast(status))
#endif
		;
#endif
		if (!status)
			throw std::runtime_error("error adding a new user to the chat");
	}

	void users_database::erase_user(const common::name_t &userName)
	{
#ifdef DATABASE_ENABLE_HANDLER_TRACKING
		BOOST_LOG_TRIVIAL(DATABASE_LOG_SEVERITY)
			<< lg::build_src("user database: erase user")
#ifdef DATABASE_DETAIL_LOG
			<< lg::build_arg("name: " + userName)
#endif
		;
#endif
		std::lock_guard lock(userDataMut);
		userData_.erase(userName);
	}

	void users_database::insert_socket(const anet::socket_data_ptr &socketData, server::IChat_user_ptr user)
	{
		std::lock_guard lock(activeSocketsMut);
		activeSockets_[socketData] = user;
	}
	
	void users_database::erase_socket(const anet::socket_data_ptr &socketData)
	{
		std::lock_guard lock(activeSocketsMut);
		activeSockets_.erase(socketData);
	}

	void users_database::for_each_socket(user_function_t func)
	{
		std::shared_lock lock(activeSocketsMut);
		for (auto &socket : activeSockets_)
			func(socket);
	}
	
	server::IChat_user_ptr users_database::get_active_user_data(const anet::socket_data_ptr& socketData)
    {
        std::shared_lock lock(activeSocketsMut);
        return activeSockets_[socketData];
    }   
	
	channels_database::find_channel_ret_v channels_database::find_channel(const common::name_t &name)
	{
		std::shared_lock lock(channelsMut);
		auto it = channels_.find(name);
		auto& [name_, channel] = *it;
		if (it != channels_.end())
		{
			return channel;
		}
		else
			return std::nullopt;
	}

	bool channels_database::is_channel_name(const common::name_t& name)
	{
		return channels_.find(name) != channels_.end();
	}

	void channels_database::insert_channel(const common::name_t &name, server::IChannel_ptr new_channel)
	{
		std::unique_lock lock(channelsMut);
		auto [name_, status] = channels_.emplace(std::make_pair(name, new_channel));

#ifdef DATABASE_ENABLE_HANDLER_TRACKING
		lock.unlock();
		BOOST_LOG_TRIVIAL(DATABASE_LOG_SEVERITY)
			<< lg::build_src("channels-database: insert channel")
#ifdef DATABASE_DETAIL_LOG
			<< lg::build_arg("name: " + name)
			<< lg::build_arg("status: " + lg::boolalpha_cast(status))
#endif
		;
#endif
		if (!status)
			throw std::runtime_error("error adding a new chat room");
	}

	void channels_database::erase_channel(const common::name_t &name)
	{
#ifdef DATABASE_ENABLE_HANDLER_TRACKING
		BOOST_LOG_TRIVIAL(DATABASE_LOG_SEVERITY)
			<< lg::build_src("channels-database: erase channel")
#ifdef DATABASE_DETAIL_LOG
			<< lg::build_arg("name: " + name)
#endif
		;
#endif
		std::lock_guard lock(channelsMut);
		channels_.erase(name);
	}

	void channel_database::insert_chat_member(const anet::socket_data_ptr &socketData, server::IChannel_member_ptr&& channelMember)
	{
		std::lock_guard lock(activeSocketsMut);
		auto [insteadedMembIt, status] = activeSockets_.emplace
				(std::make_pair(socketData, std::move(channelMember)));
		auto& [socketData_, channelMember_] = *insteadedMembIt;
		channelMember_->start_receiving_messages(socketData_);
		if(!status)
			throw std::runtime_error("error adding a new member in room");
	}

	void channel_database::erase_chat_member(const anet::socket_data_ptr &socketData)
	{
		std::lock_guard lock(activeSocketsMut);
		activeSockets_.erase(socketData);
	}

	void channel_database::for_each_member(active_members_func func) 
    {
        std::lock_guard lock(activeSocketsMut);
		for (auto &socketMember : activeSockets_)
			func(socketMember);
    }

	void channel_database::add_message_to_history(const common::msg_type& msg)
    {
        std::lock_guard lock(channelHistoryMut);
        channelHistory_.push_back(msg);
    }

	void channel_database::for_each_history(channel_history_func func)
    {
        std::shared_lock lock(channelHistoryMut);
            for(auto& msg : channelHistory_)
                func(msg);
    }

}
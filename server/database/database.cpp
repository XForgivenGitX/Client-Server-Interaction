#include "database.hpp"

namespace db
{
	std::optional<server::IChat_member_ptr> users_database::check_user_data(const common::name_t &name, const common::pass_t &pass) const
	{
		std::shared_lock lock(userDataMut);
		auto it = userData_.find(name);
		return ((it != userData_.end() && (it->second->pass_ == pass)) ? std::optional<server::IChat_member_ptr>(it->second) : std::nullopt);
	}

	bool users_database::find_name(const common::name_t &name) const
	{
		return userData_.find(name) != userData_.end();
	}

	void users_database::insert_user(server::IChat_member_ptr user) //
	{
		std::unique_lock lock(userDataMut);
		auto ret_val = userData_.insert(std::make_pair(user->name_, user));
#ifdef SERVER_ENABLE_HANDLER_TRACKING
		lock.unlock();
		BOOST_LOG_TRIVIAL(DB_LOG_SEVERITY)
			<< lg::build_log("user_database: insert user",
							 "name: " + user->name_,
							 "pass: " + user->pass_,
							 "status: " + lg::boolalpha_cast(ret_val.second));
#endif
		if (!ret_val.second)
			throw std::runtime_error("error adding a new user to the chat");
	}

	void users_database::erase_user(const common::name_t &user)
	{
#ifdef SERVER_ENABLE_HANDLER_TRACKING
		BOOST_LOG_TRIVIAL(DB_LOG_SEVERITY)
			<< lg::build_log("user_database: erase user",
							 "name: " + user);
#endif
		std::lock_guard lock(userDataMut);
		userData_.erase(user);
	}

	void users_database::insert_socket(const anet::socket_data_ptr &socketData)
	{
		std::lock_guard lock(activeSocketsMut);
		activeSockets_.insert(socketData);
	}
	void users_database::erase_socket(const anet::socket_data_ptr &socketData)
	{
		std::lock_guard lock(activeSocketsMut);
		activeSockets_.erase(socketData);
	}

	void users_database::for_each_socket(std::function<void(const anet::socket_data_ptr &)> func)
	{
		std::shared_lock lock(activeSocketsMut);
		for (auto &socket : activeSockets_)
			func(socket);
	}

	std::optional<server::IChannel_ptr> channels_database::find_channel(const common::name_t &name)
	{
		std::shared_lock lock(channelsMut);
		auto it = channels_.find(name);
		if (it != channels_.end())
		{
			return it->second;
		}
		else
			return std::nullopt;
	}

	void channels_database::insert_channel(const common::name_t &name, server::IChannel_ptr new_channel)
	{
		std::unique_lock lock(channelsMut);
		auto ret_val = channels_.insert(std::make_pair(name, new_channel));
#ifdef SERVER_ENABLE_HANDLER_TRACKING
		lock.unlock();
		BOOST_LOG_TRIVIAL(DB_LOG_SEVERITY)
			<< lg::build_log("channels-database: insert channel",
							 "name: " + name,
							 "status: " + lg::boolalpha_cast(ret_val.second));
#endif
		if (!ret_val.second)
			throw std::runtime_error("error adding a new chat room");
	}

	void channels_database::erase_channel(const common::name_t &name)
	{
#ifdef SERVER_ENABLE_HANDLER_TRACKING
		BOOST_LOG_TRIVIAL(DB_LOG_SEVERITY)
			<< lg::build_log("database-lobby: erase channel",
							 "name: " + name);
#endif
		std::lock_guard lock(channelsMut);
		channels_.erase(name);
	}

	void channel_database::for_each_member(std::function<void(server::channelMember &)> func)
	{
		std::shared_lock lock(activeSocketsMut);
		for (auto &member : activeSockets_)
			func(member);
	}

	void channel_database::insert_socket(anet::socket_data_ptr &socketData, server::IChat_member_ptr userDesc)
	{
		std::lock_guard lock(activeSocketsMut);
		if (!activeSockets_.insert({socketData, userDesc}).second)
			throw std::runtime_error("error adding a new member in room");
	}

	void channel_database::erase_socket(const anet::socket_data_ptr &socketData)
	{
		std::lock_guard lock(activeSocketsMut);
		activeSockets_.erase(socketData);
	}

}
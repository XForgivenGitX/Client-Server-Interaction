#include "database.hpp"

namespace db
{
    user_data::user_data(const name_t &name, const pass_t &pass)
        : name_(name), pass_(pass) {}

    server_database::insert_user_return_v server_database::insert_user(const user_data &user)
    {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
        BOOST_LOG_TRIVIAL(DB_LOG_SEVERITY) 
            << lg::build_log("database-server: insert user", 
            "name: " + user.name_,
            "pass: " + user.pass_);
#endif
        std::lock_guard lock(userDataMut);
        return userData_.insert(std::make_pair(user.name_, user));
    }

    void server_database::erase_user(const name_t &user)
    {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
        BOOST_LOG_TRIVIAL(DB_LOG_SEVERITY) 
            << lg::build_log("database-server: erase user", "name: " + user);
#endif
        std::lock_guard lock(userDataMut);
        userData_.erase(user);
    }

    // bool db::lobby_database::is_contains_room_name(const name_t &name) const
    // {
    //     std::shared_lock lock(roomsMut);
    //     return rooms.contains(name);
    // }
    // server::chat_room_ptr db::lobby_database::get_chat_room_ptr(const name_t &name) const
    // {
    //     std::shared_lock lock(roomsMut);
    //     return rooms.find(name)->second;
    // }
    // ip_type db::lobby_database::get_active_ip(const anet::socket_data_ptr &socketData) const
    // {
    //     std::shared_lock lock(activeIpMut);
    //     return activeIp_.find(socketData)->second;
    // }

    // name_t db::lobby_database::get_active_name(const anet::socket_data_ptr &socketData) const
    // {
    //     std::shared_lock lock(activeNamesMut);
    //     return activeNames_.find(socketData)->second;
    // }

    // void db::lobby_database::shutdown_all_sockets()
    // {
    // #ifdef SERVER_ENABLE_HANDLER_TRACKING
    //     std::cout << "@database-lobby: shutdown all sockets";
    // #endif
    //     std::lock_guard lock(activeIpMut);
    //     for (auto &[socket, ip] : activeIp_)
    //         socket->shutdown();
    // }

    // void db::lobby_database::insert_active_ip(const anet::socket_data_ptr &socketData, const ip_type &ip)
    // {
    // #ifdef SERVER_ENABLE_HANDLER_TRACKING
    //     std::cout << "@database-lobby: insert active ip: " << ip << '\n';
    // #endif
    //     std::lock_guard lock(activeIpMut);
    //     activeIp_.insert(std::make_pair(socketData, ip));
    // }

    // void db::lobby_database::erase_active_ip(const anet::socket_data_ptr &socketData)
    // {
    // #ifdef SERVER_ENABLE_HANDLER_TRACKING
    //     std::cout << "@database-lobby: erase active ip: " << activeIp_[socketData] << '\n';
    // #endif
    //     std::lock_guard lock(activeIpMut);
    //     activeIp_.erase(socketData);
    // }

    // void db::lobby_database::insert_active_name(const anet::socket_data_ptr &socketData, const name_t &name)
    // {
    // #ifdef SERVER_ENABLE_HANDLER_TRACKING
    //     std::cout << "@database-lobby: insert active name: " << name << '\n';
    // #endif
    //     std::lock_guard lock(activeNamesMut);
    //     activeNames_.insert(std::make_pair(socketData, name));
    // }

    // void db::lobby_database::erase_active_name(const anet::socket_data_ptr &socketData)
    // {
    // #ifdef SERVER_ENABLE_HANDLER_TRACKING
    //     std::cout << "@database-lobby: erase active name: " << activeNames_[socketData] << '\n';
    // #endif
    //     std::lock_guard lock(activeNamesMut);
    //     activeNames_.erase(socketData);
    // }

    // void db::lobby_database::insert_room(const server::chat_room_ptr &room, const name_t &name)
    // {
    // #ifdef SERVER_ENABLE_HANDLER_TRACKING
    //     std::cout << "@database-lobby: insert room: " << name << '\n';
    // #endif
    //     std::lock_guard lock(roomsMut);
    //     rooms.insert(std::make_pair(name, room));
    // }

}
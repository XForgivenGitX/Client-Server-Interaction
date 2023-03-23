#pragma once
#include <server_module.hpp>


using namespace common;

namespace server
{
    struct lobby
    {
        virtual void leave_auth_user(anet::socket_data_ptr)                     = 0;
        virtual void leave_all_users()                                          = 0;
        virtual void authorization(anet::socket_data_ptr)                       = 0;
        virtual void main_menu_response(anet::socket_data_ptr, common::command) = 0;
        virtual ~lobby() = default;
    };
    typedef std::shared_ptr<lobby> lobby_ptr;
    
    struct group_chat_member
    {
        virtual void start_communication(anet::socket_data_ptr socketData) = 0;
        virtual void send_message_handler(anet::socket_data_ptr& socketData, 
                                    const boost::system::error_code &error) noexcept = 0;
        virtual ~group_chat_member() = default;
    };
    typedef std::unique_ptr<group_chat_member> group_chat_member_ptr;

    struct chat_room :  public boost::noncopyable
                        
    {
        using activeMembers = std::unordered_map<anet::socket_data_ptr, group_chat_member_ptr>;
        lobby_ptr myLobby_;
    
    private:
        activeMembers activeMembers_;
        
    private:
        mutable std::shared_mutex mutex;

    public:
        chat_room(const lobby_ptr& myLobby);
        void enter_member(const anet::socket_data_ptr& socketData, group_chat_member_ptr&& newMember);
        void send_all_members(const std::string& msg);
        void leave_member(anet::socket_data_ptr socketData, std::string&&);
    };
    typedef std::shared_ptr<chat_room> chat_room_ptr;

    
    struct simple_group_chat_member :   public group_chat_member, 
                                        public boost::noncopyable, public std::enable_shared_from_this<simple_group_chat_member>
    {                                   
    private:
        chat_room_ptr myRoom_;
        name_t name_;

    public:
        simple_group_chat_member(const chat_room_ptr& myRoom, const name_t& name) ;
        
        void start_communication(anet::socket_data_ptr socketData) override;
        void start_communication_handler(anet::socket_data_ptr& socketData, const boost::system::error_code &error) noexcept;
        void receive_message_handler(anet::socket_data_ptr& socketData, const boost::system::error_code &error) noexcept;
        std::string configure_message(const std::string& usersMessage) const;
        void send_message_handler(anet::socket_data_ptr& socketData, const boost::system::error_code &error) noexcept override;
        void forced_leave(anet::socket_data_ptr socketData);
        void leave_room(anet::socket_data_ptr socketData);
    };
    typedef std::unique_ptr<simple_group_chat_member> simple_group_chat_member_ptr;

}
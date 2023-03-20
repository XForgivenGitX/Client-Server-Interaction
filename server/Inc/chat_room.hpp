#pragma once
#include <server_module.hpp>


using namespace common;

namespace server
{
    struct lobby
    {
        virtual void leave_auth_user(const anet::socket_data_ptr&)              = 0;
        virtual void leave_all_users()                                          = 0;
        virtual void authorization(anet::socket_data_ptr)                       = 0;
        virtual void main_menu_response(anet::socket_data_ptr, common::command) = 0;
        virtual ~lobby() = default;
    };
    typedef std::shared_ptr<lobby> lobby_ptr;
    
    struct group_chat_member
    {
        virtual void start_communication(anet::socket_data_ptr socketData) = 0;
        virtual void send_message_handler(anet::socket_data_ptr socketData, 
                                    const boost::system::error_code &error) = 0;
        virtual ~group_chat_member() = default;
    };
    typedef std::unique_ptr<group_chat_member> group_chat_member_ptr;

    struct chat_room :  public boost::noncopyable, 
                        public std::enable_shared_from_this<chat_room>
    {
        using activeMembers = std::unordered_map<anet::socket_data_ptr, group_chat_member_ptr>;
    
    //private:
        lobby_ptr myLobby_;
        activeMembers activeMembers_;
        
    private:
        mutable std::shared_mutex mutex;

    public:
        chat_room(const lobby_ptr& myLobby) : myLobby_(myLobby) {}
        void enter_member(const anet::socket_data_ptr& socketData, group_chat_member_ptr&& newMember)
        {
            mutex.lock();
            activeMembers_.insert(std::make_pair(socketData, std::move(newMember)));
            mutex.unlock();
            newMember->start_communication(socketData);
        }
        
        void send_all_members(const std::string& msg)
        {
            std::shared_lock lock(mutex);
            auto assembledMsg = common::assemble_frame(common::command::SEND_MESSAGE, {msg});
            for(auto& [socketData, member] : activeMembers_)
            {
                socketData->send_buffer_ = assembledMsg;
                anet::send_receive::send(socketData, {&group_chat_member::send_message_handler, &*member});
            }
        }

        void leave_member(const anet::socket_data_ptr& socketData)
        {
            std::lock_guard lock(mutex);
            activeMembers_.erase(socketData);
        }
    };
    typedef std::shared_ptr<chat_room> chat_room_ptr;

    
    struct simple_group_chat_member :   public group_chat_member, 
                                        public boost::noncopyable
    {   
    private:
        chat_room_ptr myRoom_;
        name_t name_;

    public:
        simple_group_chat_member(const chat_room_ptr& myRoom, const name_t& name)  
            : myRoom_(myRoom), name_(name) {}
        
        void start_communication(anet::socket_data_ptr socketData) override
        {
            socketData->send_buffer_ = assemble_frame(command::SUCCESS_JOIN_ROOM_RESP);
            anet::send_receive::send(socketData, {&simple_group_chat_member::receive_message_handler, this});
            
            std::string message("new member: ");
            myRoom_->send_all_members(message + name_);
        }

        void start_communication_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {
            if(error) 
            { 
                forced_leave(socketData);
                return;
            }
            else
            {
                anet::send_receive::receive(socketData, {&simple_group_chat_member::receive_message_handler, this});
            }
        }

        void receive_message_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {
            auto splited_frame = parse_and_chek_errors(socketData->receive_buffer_, error);
            if(!splited_frame) 
            { 
                forced_leave(socketData);
                return;
            }
            
            auto& [cmd, args] = splited_frame.value();
            switch(cmd)
            {
                case command::SEND_MESSAGE:
                    socketData->receive_buffer_ = configure_message(args[0]);
                    myRoom_->send_all_members(socketData->receive_buffer_);
                    anet::send_receive::receive(socketData, {&simple_group_chat_member::receive_message_handler, this});
                    break;
                
                case command::DETACH_ROOM_REQ:
                    leave_room(socketData);
                    break;

            }
        }

        void forced_leave(const anet::socket_data_ptr& socketData) const
        {
            myRoom_->leave_member(socketData);
            myRoom_->myLobby_->leave_auth_user(socketData);
            myRoom_->send_all_members(name_ + std::string(" left the chat due to a bad connection."));
        }

        void leave_room(const anet::socket_data_ptr& socketData) const
        {
            myRoom_->leave_member(socketData);
            myRoom_->myLobby_->main_menu_response(socketData, common::command::DETACH_ROOM_RESP);
            myRoom_->send_all_members(name_ + std::string(" left the chat."));
        }

        std::string configure_message(const std::string& usersMessage) const
        {
            auto datetime = boost::posix_time::second_clock::universal_time();
            std::ostringstream configuredMessage;
            configuredMessage << "[" << datetime << "] " << name_ << ": " << usersMessage;
            return configuredMessage.str();
        }

        void send_message_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {
            if(error) 
            { 
                forced_leave(socketData);
                return;
            }
        }
    };
    typedef std::unique_ptr<simple_group_chat_member> simple_group_chat_member_ptr;

}
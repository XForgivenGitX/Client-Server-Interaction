#include "chat_lobby.hpp"

//find n insrt
namespace
{
    auto &myServer = server::server_control_block::get_mutable_instance();
}

namespace server
{

    void main_lobby::enter_user(anet::socket_data_ptr &socketData)
    {
        myServer.insert_socket(socketData);
        anet::send_receive::receive(socketData, {receive_command_handler, this});
    }

    void main_lobby::leave_user(anet::socket_data_ptr &socketData)
    {
    
    }

    void main_lobby::send_command(anet::socket_data_ptr &socketData, cmd_type cmd)
    {
        socketData->send_buffer_ = common::assemble_package(cmd);
        anet::send_receive::send(socketData, {send_command_handler, this});
    }

    void main_lobby::send_command_handler(anet::socket_data_ptr &socketData, const anet::err_c &error_c)
    {
#ifdef SERVER_ENABLE_HANDLER_TRACKING
        BOOST_LOG_TRIVIAL(info) 
            << lg::build_log(lg::address_cat("lobby=", this), 
                "called send command handler", 
                "sock: " + std::to_string(socketData->get_handle()), 
                "pack: " + socketData->send_buffer_,
                "status: " + error_c.message());
#endif
        
        if (error_c)
        {
            return;
        }
        else
        {
            anet::send_receive::receive(socketData, {receive_command_handler, this});
        }
    }

    void main_lobby::receive_command_handler(anet::socket_data_ptr &socketData, const anet::err_c &error_c)
    {   
        using namespace common;
        transf_package splitedPack;
        splitedPack.disassemble(socketData->receive_buffer_);
#ifdef SERVER_ENABLE_HANDLER_TRACKING
        BOOST_LOG_TRIVIAL(info) 
            << lg::build_log(lg::address_cat("lobby=", this), 
                "called receive command handler", 
                "sock: " + std::to_string(socketData->get_handle()), 
                "pack: " + socketData->receive_buffer_,
                "match: " + lg::boolalpha_cast(splitedPack.isMatched()),
                "status: " + error_c.message());
#endif
        if (error_c || !splitedPack.isMatched())
        {
           return;
        }
        switch (splitedPack.get_command())
        {
        case command::REGISTER:
        {
            auto name = splitedPack.get_argument(protocol::USER_NAME_INDEX);
            auto pass = splitedPack.get_argument(protocol::USER_PASS_INDEX);
            if (!myServer.find_and_insert_name(name))
            {
                auto newUser = myServer.insert_user
                (db::user_data{
                    name,
                    pass,
                    socketData->get_ip()
                });
                myServer.insert_socket(socketData, newUser.first);
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
            auto name = splitedPack.get_argument(protocol::USER_NAME_INDEX);
            auto pass = splitedPack.get_argument(protocol::USER_PASS_INDEX);
            if (auto userDataOpt = myServer.check_user_data(name, pass); userDataOpt)
            {
                myServer.insert_socket(socketData, userDataOpt.value());
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
            auto name = splitedPack.get_argument(protocol::ROOM_NAME_INDEX);
            
        }
        break;
        
        default:
#ifdef SERVER_ENABLE_HANDLER_TRACKING
        BOOST_LOG_TRIVIAL(error) 
            << lg::build_log(lg::address_cat("lobby=", this), 
                "unknown command accepted");
#endif
        break;
        }
    }
}
// void server::simple_lobby::main_menu_response(anet::socket_data_ptr socketData, common::command cmd)
// {
//     socketData->send_buffer_ = common::assemble_frame(cmd);
//     anet::send_receive::send(socketData, {main_menu_responce_handler, this});
// }

// void server::simple_lobby::auth_request_handler(
//     anet::socket_data_ptr& socketData, const boost::system::error_code &error) noexcept
// {
//     if(error)
//     {
//         leave_unauth_user(socketData);
//         return;
//     }
//     anet::send_receive::receive(socketData, {auth_responce_handler, this});
// }

// void server::simple_lobby::auth_responce_handler(
//     anet::socket_data_ptr& socketData, const boost::system::error_code &error) noexcept
// {
//     auto msg = parse_and_chek_errors(socketData->receive_buffer_, error);
//     if (!msg)
//     {
//         leave_unauth_user(socketData);
//         return;
//     }
//     auto &[cmd, args] = msg.value();
//     auto &name = args[NAME_INDEX];
//     auto &pass = args[PASS_INDEX];

//     switch (cmd)
//     {
//     case command::REGISTER_RESP:
//         if (!server_control_block::get_mutable_instance().serverDataBase_.is_contains_user_name(name))
//         {
//             server_control_block::get_mutable_instance().serverDataBase_.add_user(lobbyDataBase_.get_active_ip(socketData), {name, pass});
//             lobbyDataBase_.insert_active_name(socketData, name);
//             main_menu_response(socketData, command::SUCCESS_REG_RESP);
//         }
//         else
//         {
//             socketData->send_buffer_ = common::assemble_frame(command::ECHO_REGISTER_REQ);
//             anet::send_receive::send(socketData, {auth_request_handler, this});
//         }
//         break;

//     case command::LOG_IN_RESP:
//         if (server_control_block::get_mutable_instance().serverDataBase_.is_valid_user_data(lobbyDataBase_.get_active_ip(socketData),
//                                                           args[NAME_INDEX], args[PASS_INDEX]))
//         {
//             lobbyDataBase_.insert_active_name(socketData, name);
//             main_menu_response(socketData, command::SUCCESS_LOG_IN_RESP);
//         }
//         else
//         {
//             socketData->send_buffer_ = common::assemble_frame(command::ECHO_LOG_IN_REQ);
//             anet::send_receive::send(socketData, {auth_request_handler, this});
//         }
//         break;

//     default:
//     {
//         leave_unauth_user(socketData);
//         return;
//     }
//     }
// }
// void server::simple_lobby::main_menu_responce_handler(
//     anet::socket_data_ptr& socketData, const boost::system::error_code &error) noexcept
// {
//     if(error)
//     {
//         leave_auth_user(socketData);
//         return;
//     }
//     anet::send_receive::receive(socketData, {main_menu_request_handler, this});
// }

// void server::simple_lobby::main_menu_request_handler(
//     anet::socket_data_ptr& socketData, const boost::system::error_code &error) noexcept
// {
//     auto msg = parse_and_chek_errors(socketData->receive_buffer_, error);
//     if (!msg)
//     {
//         leave_auth_user(socketData);
//         return;
//     }
//     auto &[cmd, args] = msg.value();
//     auto& name = args[NAME_INDEX];
//     switch(cmd)
//     {
//         case command::CREATE_ROOM_REQ:
//             if(!lobbyDataBase_.is_contains_room_name(name) && !server_control_block::get_mutable_instance().serverDataBase_.is_contains_user_name(name))
//             {
//                 auto newRoom = std::make_shared<server::chat_room>(shared_from_this());
//                 lobbyDataBase_.insert_room(newRoom, name);
//                 main_menu_response(socketData, command::SUCCESS_CREATE_ROOM_RESP);
//             }
//             else
//             {
//                 main_menu_response(socketData, command::ERROR_CREATE_ROOM_RESP);
//             }
//             break;

//         case command::JOIN_ROOM_REQ:
//             if(lobbyDataBase_.is_contains_room_name(name))
//             {
//                 auto room = lobbyDataBase_.get_chat_room_ptr(name);
//                 auto userName =  lobbyDataBase_.get_active_name(socketData);
//                 auto roomMember = std::make_unique<server::simple_group_chat_member>(room, userName);
//                 room->enter_member(socketData, std::move(roomMember));
//             }
//             else
//             {
//                 main_menu_response(socketData, command::ERROR_JOIN_ROOM_RESP);
//             }
//             break;
//     }

// }

// void server::simple_lobby::leave_unauth_user(anet::socket_data_ptr socketData)
// {
//     lobbyDataBase_.erase_active_ip(socketData);
//     socketData->shutdown();
// }

// void server::simple_lobby::leave_auth_user(anet::socket_data_ptr socketData)
// {
//     lobbyDataBase_.erase_active_ip(socketData);
//     lobbyDataBase_.erase_active_name(socketData);
//     socketData->shutdown();
// }
// void server::simple_lobby::leave_all_users()
// {
//     lobbyDataBase_.shutdown_all_sockets();
// }

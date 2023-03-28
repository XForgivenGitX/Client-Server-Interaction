#pragma once
#include "listen_connect.hpp"
#include "send_receive.hpp"
#include "protocol.hpp"
#include <thread>
// TODO
// 1.Очищать буффер cin
using namespace common;

namespace client
{
    class client_session
    {
    public:
        client_session(io__::io_context &ios, const anet::end_point_wrapper &endPoint)
        {
            anet::connection::connection_request(anet::make_socket_data(ios.get_executor()),
                                                 endPoint, {&connection_handler, this});
        }

        void connection_handler(anet::socket_data_ptr &socketData, const boost::system::error_code &error)
        {
            if (error)
            {
                socketData->shutdown();
                return;
            }
            else
            {
                std::cout << "Welcome to the quickchat!\n";
                authorization(socketData);
            }
        }

        void authorization(anet::socket_data_ptr &socketData)
        {
            std::string buf;
            std::cout << "You need register/log in.[1/2]:";
            std::getline(std::cin, buf);
            if (buf == "1")
            {
                reading_and_sending_data(socketData, command::REGISTER);
            }
            else if (buf == "2")
            {
                reading_and_sending_data(socketData, command::LOG_IN);
            }
            else
            {
                std::cout << "Encorrect command. Enter 1 or 2:";
                authorization(socketData);
            }
        }

        void send_command(anet::socket_data_ptr &socketData, common::command cmd, const common::transf_package::args_t &args = {})
        {
            socketData->send_buffer_ = common::assemble_package(cmd, args);
            anet::send_receive::send(socketData, {send_command_handler, this});
        }

        void send_command_handler(anet::socket_data_ptr &socketData, const boost::system::error_code &error)
        {
            if (error)
            {
                socketData->shutdown();
                return;
            }
            else
            {
                anet::send_receive::receive(socketData, {receive_command_handler, this});
            }
        }
        
        void receive_command_handler(anet::socket_data_ptr &socketData, const boost::system::error_code &error)
        {
            using namespace common;
            transf_package splitedPack;
            splitedPack.disassemble(socketData->receive_buffer_);
            if (error || !splitedPack.isMatched())
            {
                socketData->shutdown();
                return;
            }
            switch (splitedPack.get_command())
            {
            case command::ERROR_LOG_IN:
                std::cout << "Invalid username or password. Try again:\n";
                authorization(socketData);
                break;

            case command::ERROR_REGISTER:
                std::cout << "A profile with that name already exists. Use other:\n";
                authorization(socketData);
                break;

            case command::SUCCESS_REGISTER:
            case command::SUCCESS_LOG_IN:
                enter_to_lobby(socketData);
                break;

            default:
                socketData->shutdown();
            }
        }

        void auth_response_handler(anet::socket_data_ptr &socketData, const boost::system::error_code &error)
        {
            // anet::send_receive::receive(socketData_, {authorization_handler, this});
        }

        void reading_and_sending_data(anet::socket_data_ptr &socketData, common::command cmd)
        {
            std::string nameBuff, readBuff;
            std::cout << "Name:";
            std::getline(std::cin, nameBuff);
            std::cout << "Pass:";
            std::getline(std::cin, readBuff);
            send_command(socketData, cmd, {nameBuff, readBuff});
        }

        void identify_and_send_command(anet::socket_data_ptr &socketData)
        {
            std::string cmdBuff;
            std::cout << "Enter command:";
            std::getline(std::cin, cmdBuff);
            if (cmdBuff == "join")
            {
                std::cout << "Room name:";
                std::getline(std::cin, cmdBuff);
                socketData->send_buffer_ = common::assemble_package(command::JOIN_ROOM, {cmdBuff});
                anet::send_receive::send(socketData, {lobby_request_handler, this});
            }
            else if (cmdBuff == "create")
            {
                std::cout << "Room name:";
                std::getline(std::cin, cmdBuff);
                socketData->send_buffer_ = common::assemble_package(command::CREATE_ROOM, {cmdBuff});
                anet::send_receive::send(socketData, {lobby_request_handler, this});
            }
            else
            {
                std::cout << "encorrect command.\n";
                identify_and_send_command(socketData);
            }
        }

        void enter_to_lobby(anet::socket_data_ptr &socketData)
        {
            std::cout << "Welcome to the lobby!\n";
            identify_and_send_command(socketData);
        }

        void lobby_request_handler(anet::socket_data_ptr &socketData, const boost::system::error_code &error)
        {
            anet::send_receive::receive(socketData, {lobby_responce_handler, this});
        }

        void lobby_responce_handler(anet::socket_data_ptr &socketData, const boost::system::error_code &error)
        {
            common::transf_package pack;
            pack.disassemble(socketData->receive_buffer_);
            if (error || !pack.isMatched())
            {
                socketData->shutdown();
                return;
            }
            switch (pack.get_command())
            {
            case common::command::SUCCESS_CREATE_ROOM:
                std::cout << "The room has been successfully created!\n";
                identify_and_send_command(socketData);
                break;

            case common::command::ERROR_CREATE_ROOM:
                std::cout << "A room with that name already exists.\n";
                identify_and_send_command(socketData);
                break;

            case common::command::SUCCESS_JOIN_ROOM:
                std::cout << "Successfully joined, have a nice chat!\n";
                start_communication(socketData);
                break;

            case common::command::ERROR_JOIN_ROOM:
                std::cout << "There is no room with that name.\n";
                identify_and_send_command(socketData);
                break;

            default:
                break;
            }
        }

        void start_communication(anet::socket_data_ptr &socketData)
        {
            io__::post(socketData->socket_.get_executor(), [&]{receive_message(socketData);});
            std::string msgBuff;
            bool leave = false;
            while (!leave)
            {
                std::getline(std::cin, msgBuff);
                if(msgBuff.empty()) continue;
                if(msgBuff == "-q")
                {
                    socketData->send_buffer_ = common::assemble_package(command::DETACH_ROOM, {msgBuff});
                    leave = true;
                }
                else
                    socketData->send_buffer_ = common::assemble_package(command::SEND_MESSAGE, {msgBuff});
                anet::send_receive::send(socketData, {send_message_handler, this});
            }
        }

        void receive_message(anet::socket_data_ptr &socketData)
        {
            anet::send_receive::receive(socketData, {receive_message_handler, this});
        }
        void receive_message_handler(anet::socket_data_ptr& socketData, const boost::system::error_code &error)
        {
            common::transf_package pack;
            pack.disassemble(socketData->receive_buffer_);
            // if (error || !pack.isMatched())
            // {
            //     socketData->shutdown();
            //     return;
            // }

            switch (pack.get_command())
            {
            case common::command::SEND_MESSAGE:
                std::cout << pack.get_argument(0) << '\n';
                receive_message(socketData);
                break;

            case common::command::DETACH_ROOM:
                identify_and_send_command(socketData);
                break;

            default:
                break;
            }
        }

        void send_message_handler(anet::socket_data_ptr& socketData, const boost::system::error_code &error)
        {
            // if (error)
            // {
            //     socketData->shutdown();
            // }

        }
    };
}
#pragma once
#include <client_module.hpp>
#include <protocol.hpp>
#include <thread>
// TODO
// 1.Очищать буффер cin
using namespace common;
namespace client
{
    class client_session
    {
        anet::socket_data_ptr socketData_;

    public:
        client_session(io__::io_context &ios, anet::end_point_wrapper endPoint)
            : socketData_(anet::make_socket_data(ios.get_executor()))
        {
            anet::connection::connection_request(anet::make_socket_data(socketData_, endPoint), {&connection_handler, this});
        }

        void connection_handler(anet::socket_data_endpoint_ptr &&socketDataEndPoint, const boost::system::error_code &error)
        {
            if (error)
            {
                socketData_->shutdown();
                return;
            }
            else
            {
                anet::send_receive::receive(socketData_, {authorization_handler, this});
            }
        }

        void authorization_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {
            auto resp = common::disassemble_frame(socketData->receive_buffer_);
            if (error || !resp)
            {
                socketData->shutdown();
                return;
            }
            auto &[cmd, args] = resp.value();

            switch (cmd)
            {
            case command::ECHO_LOG_IN_REQ:
                std::cout << "Invalid username or password. Try again:\n";
                reading_and_sending_data(socketData, common::command::LOG_IN_RESP);
                break;

            case command::LOG_IN_REQ:
                std::cout << "We are glad to see you again! Please log in:\n";
                reading_and_sending_data(socketData, common::command::LOG_IN_RESP);
                break;

            case command::ECHO_REGISTER_REQ:
                std::cout << "A profile with that name already exists. Use other:\n";
                reading_and_sending_data(socketData, common::command::REGISTER_RESP);
                break;

            case command::REGISTER_REQ:
                std::cout << "Welcome to the chat! You need to register:\n";
                reading_and_sending_data(socketData, common::command::REGISTER_RESP);
                break;

            case command::SUCCESS_REG_RESP:
            case command::SUCCESS_LOG_IN_RESP:
                enter_to_lobby(socketData);
                break;

            default:
                socketData->shutdown();
            }
        }

        void auth_response_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {
            anet::send_receive::receive(socketData_, {authorization_handler, this});
        }

        void reading_and_sending_data(const anet::socket_data_ptr &socketData, common::command cmd)
        {
            std::string nameBuff, readBuff;
            std::cout << "Name:";
            std::getline(std::cin, nameBuff);
            std::cout << "Pass:";
            std::getline(std::cin, readBuff);
            socketData->send_buffer_ = common::assemble_frame(cmd, {nameBuff, readBuff});
            anet::send_receive::send(socketData, {auth_response_handler, this});
        }

        void identify_and_send_command(const anet::socket_data_ptr &socketData)
        {
            std::string cmdBuff;
            std::cout << "Enter command:";
            std::getline(std::cin, cmdBuff);
            if (cmdBuff == "join")
            {
                std::cout << "Room name:";
                std::getline(std::cin, cmdBuff);
                socketData->send_buffer_ = common::assemble_frame(command::JOIN_ROOM_REQ, {cmdBuff});
                anet::send_receive::send(socketData, {lobby_request_handler, this});
            }
            else if (cmdBuff == "create")
            {
                std::cout << "Room name:";
                std::getline(std::cin, cmdBuff);
                socketData->send_buffer_ = common::assemble_frame(command::CREATE_ROOM_REQ, {cmdBuff});
                anet::send_receive::send(socketData, {lobby_request_handler, this});
            }
            else
            {
                std::cout << "encorrect command.\n";
                identify_and_send_command(socketData);
            }
        }

        void enter_to_lobby(const anet::socket_data_ptr &socketData)
        {
            std::cout << "Welcome to the lobby!\n";
            identify_and_send_command(socketData);
        }

        void lobby_request_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {
            anet::send_receive::receive(socketData_, {lobby_responce_handler, this});
        }

        void lobby_responce_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {
            auto resp = common::disassemble_frame(socketData->receive_buffer_);
            if (error || !resp)
            {
                socketData->shutdown();
                return;
            }
            auto &[cmd, args] = resp.value();
            switch (cmd)
            {
            case common::command::SUCCESS_CREATE_ROOM_RESP:
                std::cout << "The room has been successfully created!\n";
                identify_and_send_command(socketData);
                break;

            case common::command::ERROR_CREATE_ROOM_RESP:
                std::cout << "A room with that name already exists.\n";
                identify_and_send_command(socketData);
                break;

            case common::command::SUCCESS_JOIN_ROOM_RESP:
                std::cout << "Successfully joined, have a nice chat!\n";
                start_communication(socketData);
                break;

            case common::command::ERROR_JOIN_ROOM_RESP:
                std::cout << "There is no room with that name.\n";
                identify_and_send_command(socketData);
                break;

            default:
                break;
            }
        }

        void start_communication(const anet::socket_data_ptr &socketData)
        {
            io__::post(socketData->socket_.get_executor(), [&]{receive_message(socketData);});
            std::string msgBuff;
            while (true)
            {
                std::getline(std::cin, msgBuff);
                if(msgBuff.empty()) continue;
                socketData->send_buffer_ = common::assemble_frame(command::SEND_MESSAGE, {msgBuff});
                anet::send_receive::send(socketData, {send_message_handler, this});
            }
        }

        void receive_message(const anet::socket_data_ptr &socketData)
        {
            anet::send_receive::receive(socketData_, {receive_message_handler, this});
        }
        void receive_message_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {
            auto resp = common::disassemble_frame(socketData->receive_buffer_);
            if (error || !resp)
            {
                socketData->shutdown();
                return;
            }
            auto &[cmd, args] = resp.value();
            switch (cmd)
            {
            case common::command::SEND_MESSAGE:
                std::cout << args[0] << '\n';
                receive_message(socketData);
                break;
            default:
                break;
            }
        }

        void send_message_handler(anet::socket_data_ptr socketData, const boost::system::error_code &error)
        {
            if (error)
            {
                socketData->shutdown();
            }
        }
    };
}
# 1.очередь сообщений у каждой комнаты
# 2.порт-имя комнаты
# 3.запрос на количество участников всех комнат
# 4.создавать комнату
# 5.удалять комнату
# 6.передавать историю начиная с индекса
# 7.регистрация на сервере - имя не занято - у каждой комнаты свой индекс начала сообщений
# 8.запрос на изменение имени
# 9.

############################################

cmake_minimum_required(VERSION 3.0)
# set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")
# set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/g++.exe")
project(QuickChat)

set (CMAKE_CXX_STANDARD 20) 
set (RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

enable_testing()
add_subdirectory(modules/asio_network)
add_subdirectory(modules/common/shared)
add_subdirectory(modules/common/tests)
#add_subdirectory(server)
#add_subdirectory(client)

############################################

# https://habr.com/ru/post/197392/
# https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
# https://habr.com/ru/post/649363/
# https://docs.github.com/ru/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax
# https://github.com/botaojia/chat/blob/master/chat_client.cpp
# https://github.com/k-morozov/AppChat
# https://ru.wikipedia.org/wiki/%D0%9F%D0%B5%D1%80%D0%B5%D0%BD%D0%B0%D0%BF%D1%80%D0%B0%D0%B2%D0%BB%D0%B5%D0%BD%D0%B8%D0%B5_%D0%B2%D0%B2%D0%BE%D0%B4%D0%B0-%D0%B2%D1%8B%D0%B2%D0%BE%D0%B4%D0%B0
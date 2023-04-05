# Задачи:
# 1.очередь сообщений у каждой комнаты
# 2.возможность создавать приватные комнаты(вход с паролем)
# 3.запрос на количество участников всех комнат
# 4.создавать комнату
# 5.удалять комнату
# 6.передавать историю начиная с индекса
# 7.регистрация на сервере - имя не занято - у каждого игрока в комнате свой индекс начала сообщений
# 8.запрос на изменение имени
# 9.

############################################

cmake_minimum_required(VERSION 3.0)
project(QuickChat)

# set (CMAKE_CXX_STANDARD 20) 
# set (RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(modules/common)
add_subdirectory(modules/asio_network)
add_subdirectory(server)
add_subdirectory(client)

############################################

# https://habr.com/ru/post/197392/
# https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
# https://habr.com/ru/post/649363/
# https://www.sqlitetutorial.net/sqlite-foreign-key/
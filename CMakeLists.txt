cmake_minimum_required(VERSION 3.21)
project(Programming)

set(CMAKE_CXX_STANDARD 14)

add_executable(Programming main.cpp User.cpp User.h Question.cpp Question.h Comment.cpp Comment.h VoteType.cpp VoteType.h)

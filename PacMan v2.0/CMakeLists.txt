cmake_minimum_required(VERSION 3.15)
project(PacMan)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(PacMan
        BaseController.cpp
        BaseController.h
        Blinky.cpp
        Blinky.h
        Booster.cpp
        Booster.h
        Border.cpp
        Border.h
        Clyde.cpp
        Clyde.h
        Coins.cpp
        Coins.h
        Controller.cpp
        Controller.h
        CreateMap.cpp
        CreateMap.h
        Field.cpp
        Field.h
        Fruits.cpp
        Fruits.h
        Ghosts.cpp
        Ghosts.h
        Info.cpp
        Info.h
        Inky.cpp
        Inky.h
        main.cpp
        MotionlessObjects.cpp
        MotionlessObjects.h
        MovingObjects.cpp
        MovingObjects.h
        Objects.cpp
        Objects.h
        Pacman.cpp
        Pacman.h
        Pinky.cpp
        Pinky.h)

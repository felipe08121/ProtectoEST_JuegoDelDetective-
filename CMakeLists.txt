cmake_minimum_required(VERSION 4.0)
project(ProtectoEST_JuegoDelDetective_)

set(CMAKE_CXX_STANDARD 20)

add_executable(ProtectoEST_JuegoDelDetective_ src/main.cpp
        src/Ubicacion/Ubicacion.cpp
        src/Ubicacion/Ubicacion.h
        src/Pista/Pista.cpp
        src/Pista/Pista.h
        src/Testigo/Testigo.cpp
        src/Testigo/Testigo.h)

cmake_minimum_required(VERSION 4.0)
project(ProyectoEstructuraDatos)

set(CMAKE_CXX_STANDARD 20)

add_executable(ProyectoEstructuraDatos src/main.cpp
        src/Ubicacion/Ubicacion.cpp
        src/Ubicacion/Ubicacion.h
        src/Pista/Pista.cpp
        src/Pista/Pista.h
        src/Testigo/Testigo.cpp
        src/Testigo/Testigo.h
        src/Detective/Detective.cpp
        src/Detective/Detective.h
        src/Mapa/Mapa.cpp
        src/Mapa/Mapa.h
        src/Sospechoso/Sospechoso.cpp
        src/Sospechoso/Sospechoso.h
        src/TablaSospechosos/TablaSospechosos.cpp
        src/TablaSospechosos/TablaSospechosos.h
        src/Juego/Juego.cpp
        src/Juego/Juego.h)
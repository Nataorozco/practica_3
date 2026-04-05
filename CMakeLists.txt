cmake_minimum_required(VERSION 3.16)

project(practica_3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_custom_target(Documentacion SOURCES
    README.md
)

add_executable(practica_3
  src/main.cpp
  include/enlace.h
  src/ejercicio_1.cpp
  src/ejercicio_2.cpp
  src/ejercicio_3.cpp
  src/ejercicio_4.cpp
  src/ejercicio_5.cpp

)

include(GNUInstallDirs)
install(TARGETS practica_3
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_include_directories(practica_3 PRIVATE include)

install(FILES
  README.md
  DESTINATION ${CMAKE_INSTALL_DOCDIR}
)

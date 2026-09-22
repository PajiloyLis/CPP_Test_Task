cmake_minimum_required(VERSION 4.1)
project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(server main.cpp
        ui/MainWindow.cpp
        ui/MainWindow.h
        ui/MainWindow.ui
        controller/IServerController.h
        domainModels/ClientId.h
        domainModels/ClientStatus.h
        domainModels/ClientInfo.h
        domainModels/Thresholds.h
        domainModels/Measurements.h
        domainModels/Severity.h)
target_link_libraries(server
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_include_directories(server PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)


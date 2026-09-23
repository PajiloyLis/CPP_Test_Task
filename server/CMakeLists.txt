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
        Network
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
        domainModels/Severity.h
        controller/ServerController/ServerController.cpp
        controller/ServerController/ServerController.h
        protocolModels/Incoming.h
        protocolModels/Outcoming.h
        protocolModels/packetCodec/PacketCodec.cpp
        protocolModels/packetCodec/PacketCodec.h
        protocolModels/lineBuffer/LineBuffer.cpp
        protocolModels/lineBuffer/LineBuffer.h
        network/clientSession/ClientSession.cpp
        network/clientSession/ClientSession.h
        network/serverWorker/ServerWorker.cpp
        network/serverWorker/ServerWorker.h
        ui/settingsDialog/SettingsDialog.cpp
        ui/settingsDialog/SettingsDialog.h
        configParsers/thresholdsConfigParser/ThresholdsParser.cpp
        configParsers/thresholdsConfigParser/ThresholdsParser.h
        configModels/ServerSettings.h
        configParsers/serverConfigParser/ServerSettingsParser.h
        services/SettingsService.cpp
        services/SettingsService.h)
target_link_libraries(server
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
)

target_include_directories(server PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)


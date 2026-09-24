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

add_executable(server server/main.cpp
        server/ui/MainWindow.cpp
        server/ui/MainWindow.h
        server/ui/MainWindow.ui
        server/controller/IServerController.h
        server/domainModels/ClientStatus.h
        server/domainModels/ClientInfo.h
        server/domainModels/Thresholds.h
        server/controller/ServerController/ServerController.cpp
        server/controller/ServerController/ServerController.h
        protocolModels/Incoming.h
        protocolModels/Outcoming.h
        protocolModels/packetCodec/PacketCodec.cpp
        protocolModels/packetCodec/PacketCodec.h
        protocolModels/lineBuffer/LineBuffer.cpp
        protocolModels/lineBuffer/LineBuffer.h
        server/network/clientSession/ClientSession.cpp
        server/network/clientSession/ClientSession.h
        server/network/serverWorker/ServerWorker.cpp
        server/network/serverWorker/ServerWorker.h
        server/ui/settingsDialog/SettingsDialog.cpp
        server/ui/settingsDialog/SettingsDialog.h
        server/configParsers/thresholdsConfigParser/ThresholdsParser.cpp
        server/configParsers/thresholdsConfigParser/ThresholdsParser.h
        server/configModels/ServerSettings.h
        server/configParsers/serverConfigParser/ServerSettingsParser.h
        server/services/SettingsService.cpp
        server/services/SettingsService.h
        server/configParsers/serverConfigParser/ServerSettingsParser.h.cpp)
target_link_libraries(server
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
)

target_include_directories(server PRIVATE
        server
)


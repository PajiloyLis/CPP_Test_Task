cmake_minimum_required(VERSION 4.1)
project(TestTask)

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

add_library(communication_protocol STATIC
        commonProtocolModels/ClientId.h
        commonProtocolModels/Severity.h
        commonProtocolModels/Measurements.h
        commonProtocolModels/Incoming.h
        commonProtocolModels/Outcoming.h
        commonProtocolModels/lineBuffer/LineBuffer.h
        commonProtocolModels/lineBuffer/LineBuffer.cpp
        commonProtocolModels/packetCodec/PacketCodec.h
        commonProtocolModels/packetCodec/PacketCodec.cpp
)

target_include_directories(communication_protocol PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(communication_protocol PUBLIC
        Qt6::Core
)

add_executable(server
        server/main.cpp

        server/ui/MainWindow.cpp
        server/ui/MainWindow.h
        server/ui/MainWindow.ui

        server/ui/settingsDialog/SettingsDialog.cpp
        server/ui/settingsDialog/SettingsDialog.h
        server/ui/settingsDialog/SettingsDialog.ui

        server/controller/IServerController.h
        server/controller/ServerController/ServerController.cpp
        server/controller/ServerController/ServerController.h

        server/domainModels/ClientStatus.h
        server/domainModels/ClientInfo.h
        server/domainModels/Thresholds.h

#        protocolModels/Incoming.h
#        protocolModels/Outcoming.h
#        protocolModels/packetCodec/PacketCodec.cpp
#        protocolModels/packetCodec/PacketCodec.h
#        protocolModels/lineBuffer/LineBuffer.cpp
#        protocolModels/lineBuffer/LineBuffer.h

        server/network/clientSession/ClientSession.cpp
        server/network/clientSession/ClientSession.h

        server/network/serverWorker/ServerWorker.cpp
        server/network/serverWorker/ServerWorker.h

        server/services/SettingsService.cpp
        server/services/SettingsService.h

        server/configParsers/thresholdsConfigParser/ThresholdsParser.cpp
        server/configParsers/thresholdsConfigParser/ThresholdsParser.h

        server/configParsers/serverConfigParser/ServerSettingsParser.h
        server/configParsers/serverConfigParser/ServerSettingsParser.cpp

        server/configModels/ServerSettings.h
)

target_include_directories(server PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/server
)

target_link_libraries(server PRIVATE
        communication_protocol
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
)

add_custom_command(TARGET server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/server/ServerConfig.json
        $<TARGET_FILE_DIR:server>/server.json
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/server/ThresholdsConfig.json
        $<TARGET_FILE_DIR:server>/thresholds.json
)

add_executable(client
        client/configModels/ClientSettings.h
        client/configParsers/ClientConfigParser/ClientSettingsParser.cpp
        client/configParsers/ClientConfigParser/ClientSettingsParser.h
        client/services/ClientService/ClientService.cpp
        client/services/ClientService/ClientService.h
        client/main.cpp
)

target_include_directories(client PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/client
)


target_link_libraries(client PRIVATE
        communication_protocol
        Qt6::Core
        Qt6::Network
)

add_custom_command(TARGET client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/client/ClientConfig.json
        $<TARGET_FILE_DIR:client>/client.json
)
cmake_minimum_required(VERSION 3.20)
project(RecoilRCS VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Поддержка vcpkg
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Находим зависимости
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(websocketpp CONFIG REQUIRED)

# Определяем ресурсы
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/config.json ${CMAKE_CURRENT_BINARY_DIR}/config.json COPYONLY)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/patterns/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/patterns/)

# Исполняемый файл
add_executable(RecoilRCS WIN32
    src/main.cpp
    src/config_loader.cpp
    src/recoil_engine.cpp
    src/input_service.cpp
    src/timing_service.cpp
    src/hotkey_service.cpp
    src/gsi_server.cpp
    src/overlay_window.cpp
    src/logger.cpp
)

target_include_directories(RecoilRCS PRIVATE include)

target_link_libraries(RecoilRCS PRIVATE
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    websocketpp::websocketpp
    winmm      # timeBeginPeriod
    user32     # SendInput, GetAsyncKeyState
    gdi32      # оверлей
    ws2_32     # сокеты
)

set_target_properties(RecoilRCS PROPERTIES
    WIN32_EXECUTABLE TRUE
    LINK_FLAGS "/SUBSYSTEM:WINDOWS"
)

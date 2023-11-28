cmake_minimum_required(VERSION 3.26)
project(CasoFinalIntegrador)

include(FetchContent)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(json)
set(CMAKE_CXX_STANDARD 17)

add_executable(CasoFinalIntegrador CasoFinal.cpp)
target_link_libraries(CasoFinalIntegrador PRIVATE nlohmann_json::nlohmann_json)

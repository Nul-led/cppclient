cmake_minimum_required(VERSION 3.31.6)
project(cppclient)

set(CMAKE_CXX_STANDARD 23)

set(SOURCES
        main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

set(SOURCES_MODULES
        src/core/Viewport.ixx
        src/util/Color.ixx
        src/util/Interpolation.ixx
        src/net/NetProp.ixx
        src/net/Reader.ixx
        src/core/World.ixx
        src/game/ArenaData.ixx
        src/game/CameraData.ixx
        src/game/TankData.ixx
        src/game/EntityData.ixx
        src/core/GameServer.ixx
        src/net/Clientbound.ixx
        src/game/ScoreboardData.ixx
        src/game/AttributeData.ixx
        src/game/DeathData.ixx
        src/game/IndicatorData.ixx
        src/net/Serverbound.ixx
        src/net/Writer.ixx
        src/rendering/Canvas.ixx
        src/ui/UI.ixx
        src/rendering/Image.ixx
        src/rendering/GameRendering.ixx
        src/core/Inputs.ixx
        src/ui/widgets/Progressbar.ixx
        src/ui/widgets/Button.ixx
        src/ui/components/Attributes.ixx
        src/ui/UIColor.ixx
        src/ui/UIString.ixx
        src/ui/components/Upgrades.ixx
        src/rendering/Absolutes.ixx
        src/ui/components/Statistics.ixx
        src/game/UIData.ixx
        src/ui/widgets/Tooltip.ixx
        src/ui/components/Changelog.ixx
        src/ui/components/NameInput.ixx
        src/ui/views/Menu.ixx
        src/ui/components/GameModes.ixx
        src/net/Servers.ixx
        src/ui/components/Scoreboard.ixx
        src/rendering/Extraction.ixx
)

### todo remove
if(0)
    add_executable(${PROJECT_NAME} ${SOURCES_MODULES})
endif()

target_sources(${PROJECT_NAME} PRIVATE
        FILE_SET modules TYPE CXX_MODULES FILES
        ${SOURCES_MODULES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
# todo fix compiler / linker opts, this is a mess D:

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--bind")

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/external
)

# todo fix compiler / linker opts, this is a mess D:

set(COMMON_COMPILER_OPTIONS
        -std=c++23
        -lembind
        -sOFFSCREENCANVAS_SUPPORT=1
        -sENVIRONMENT=web
        -sFILESYSTEM=0
        -sDYNAMIC_EXECUTION=1
        -sALLOW_MEMORY_GROWTH=1
        -sWASM=1
        -sMODULARIZE=1
        -sEXPORT_ES6=1
        -sEXPORTED_FUNCTIONS=_main
        -sEXPORTED_RUNTIME_FUNCTIONS=cwrap
        -Wno-deprecated
        -Wno-reorder-init-list
)

set(COMMON_LINKER_OPTIONS
    ${COMMON_COMPILER_OPTIONS}
)

message(NOTICE "Building in mode " ${CMAKE_BUILD_TYPE})

if(CMAKE_BUILD_TYPE EQUAL "Debug")
    #set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--bind")
elseif(CMAKE_BUILD_TYPE EQUAL "Release")
    target_compile_options(${PROJECT_NAME}
            PRIVATE
            ${COMMON_COMPILER_OPTIONS}
            -O3
    )
    target_link_options(${PROJECT_NAME}
            PRIVATE
            ${COMMON_LINKER_OPTIONS}
            -flto
    )

    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=1)
    target_link_options(${PROJECT_NAME}
            PRIVATE
            -std=c++23
            -lembind
            -sOFFSCREENCANVAS_SUPPORT=1
            -sENVIRONMENT=web
            -sFILESYSTEM=0
            -sDYNAMIC_EXECUTION=1
            -sALLOW_MEMORY_GROWTH=1
            -sWASM=1
            -sMODULARIZE=1
            -sEXPORT_ES6=1
            -sEXPORTED_FUNCTIONS=_main
            -sEXPORTED_RUNTIME_FUNCTIONS=cwrap
            -DCLAY_WASM
            -Wno-deprecated
    )
endif()
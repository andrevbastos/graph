# Versão mínima do CMake e nome do projeto
cmake_minimum_required(VERSION 3.15)
project(graph_project LANGUAGES CXX)

# Configurações do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =-=-=-=-=-=-= Configurações de Build (Boas Práticas) =-=-=-=-=-=-=

# Flags de Compilação Comuns 
set(COMMON_CXX_FLAGS "-Wall")
if (CMAKE_COMPILER_IS_GNUXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -Wextra -Wpedantic")
endif ()

set(CMAKE_CXX_FLAGS_DEBUG_INIT "${COMMON_CXX_FLAGS} -g3")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "${COMMON_CXX_FLAGS} -O3 -DNDEBUG")

# Opção para sanitizers (ótimo para debug)
option(ENABLE_SANITIZERS "Enable Address and Undefined Behavior Sanitizers" OFF)
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS_DEBUG_INIT "${CMAKE_CXX_FLAGS_DEBUG_INIT} -fsanitize=address,undefined")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG_INIT "${CMAKE_EXE_LINKER_FLAGS_DEBUG_INIT} -fsanitize=address,undefined")
endif()

# Define CMAKE_BUILD_TYPE se não estiver definido
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

message(STATUS "Tipo de Build: ${CMAKE_BUILD_TYPE}")

# Copia o compile_commands.json para a raiz (útil para IDEs)
if (UNIX)
    add_custom_target(
        copy-compile-commands ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
        COMMENT "Copiando compile_commands.json para a raiz do projeto..."
    )
endif (UNIX)

# =-=-=-=-=-=-= BIBLIOTECA DE GRAFOS (graph_lib) =-=-=-=-=-=-=

# Encontra recursivamente todos os arquivos .cpp dentro de src/graph
file(GLOB_RECURSE GRAPH_LIB_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/graph/*.cpp"
)

# Criamos a biblioteca de grafos (STATIC é mais simples para linkar com um app)
add_library(graph_lib STATIC ${GRAPH_LIB_SOURCES})

# Define os diretórios de include da biblioteca
# 'PUBLIC' faz com que qualquer alvo que linkar graph_lib
# automaticamente herde este diretório de include.
target_include_directories(graph_lib PUBLIC
    "${CMAKE_SOURCE_DIR}/include"
)

message(STATUS "Biblioteca 'graph_lib' configurada.")

# =-=-=-=-=-=-= APLICAÇÃO DE TESTE (graph_app) =-=-=-=-=-=-=

# Define o diretório de output para executáveis
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Adiciona o executável
add_executable(graph_app tests/main.cpp)

# Linka a aplicação com a biblioteca de grafos
# Como graph_lib expôs seu include como PUBLIC,
# graph_app já vai encontrar "graph/graph.hpp" e "json/json.hpp".
target_link_libraries(
    graph_app
    PRIVATE
        graph_lib
)

# Comando Pós-Build para copiar a pasta 'resources'
# para o diretório do executável 
add_custom_command(TARGET graph_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:graph_app>/resources
    COMMENT "Copiando 'resources' para o diretório do executável..."
)

# Mensagem final para o usuário [cite: 11]
message(STATUS "Configuração do CMake concluída.")
message(STATUS "Para construir, use 'make' (ou seu build system) no diretório de build.")
message(STATUS "Executável estará em: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/graph_app")
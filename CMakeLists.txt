# Versão mínima do CMake e nome do projeto
cmake_minimum_required(VERSION 3.15)
project(ifcg_engine LANGUAGES C CXX)

# Configurações do CMake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (UNIX)
    # Copia o compile_commands.json para a raiz do projeto para facilitar o uso por IDEs/linters
    add_custom_target(
        copy-compile-commands ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
        COMMENT "Copiando compile_commands.json para a raiz do projeto..."
    )
endif (UNIX)

# Flags de Compilação Comuns
set(COMMON_CXX_FLAGS "-Wall")
if (CMAKE_COMPILER_IS_GNUXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -Wextra -Wpedantic")
endif ()

set(CMAKE_CXX_FLAGS_DEBUG_INIT "${COMMON_CXX_FLAGS} -g3")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "${COMMON_CXX_FLAGS} -O3 -DNDEBUG")

# Opção para sanitizers (útil para debug)
option(ENABLE_SANITIZERS "Enable Address and Undefined Behavior Sanitizers" OFF)
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS_DEBUG_INIT "${CMAKE_CXX_FLAGS_DEBUG_INIT} -fsanitize=address,undefined")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG_INIT "${CMAKE_EXE_LINKER_FLAGS_DEBUG_INIT} -fsanitize=address,undefined")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG_INIT "${CMAKE_SHARED_LINKER_FLAGS_DEBUG_INIT} -fsanitize=address,undefined")
endif()

# Define CMAKE_BUILD_TYPE se não estiver definido (ex: por presets ou linha de comando)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "Tipo de Build: ${CMAKE_BUILD_TYPE}")

# Encontrar Pacotes
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 CONFIG REQUIRED)
find_package(glm 0.9.9 REQUIRED)

# =-=-=-=-=-=-= BIBLIOTECA ifcg =-=-=-=-=-=-=
# Define onde as bibliotecas (.so, .dll, .dylib) serão colocadas
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Diretório base dos arquivos fonte da biblioteca ifcg
set(IFCG_LIB_BASE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

# Encontra recursivamente todos os arquivos .cpp dentro de IFCG_LIB_BASE_SOURCE_DIR e suas subpastas
# CONFIGURE_DEPENDS tenta fazer o CMake re-rodar o glob se arquivos forem adicionados/removidos
file(GLOB_RECURSE IFCG_LIBRARY_CPP_FILES CONFIGURE_DEPENDS
    "${IFCG_LIB_BASE_SOURCE_DIR}/*.cpp"
)

# O file(GLOB_RECURSE) já pega os arquivos em subdiretórios.
# Se você tiver ifcg.cpp na raiz de src/ e outros .cpp em src/2D/, src/3D/, etc.
# o seguinte padrão é suficiente:
file(GLOB_RECURSE IFCG_LIBRARY_CPP_FILES_REFINED CONFIGURE_DEPENDS
    "${IFCG_LIB_BASE_SOURCE_DIR}/*.cpp"
)

# Lista final de arquivos fonte para a biblioteca ifcg
set(IFCG_LIBRARY_SOURCE_FILES
    ${IFCG_LIB_BASE_SOURCE_DIR}/glad/glad.c
    ${IFCG_LIBRARY_CPP_FILES_REFINED}
)

# Adicionando a biblioteca ifcg como SHARED
add_library(ifcg SHARED ${IFCG_LIBRARY_SOURCE_FILES})


# Definições de compilação para a biblioteca ifcg
target_compile_definitions(ifcg PRIVATE IFCG_EXPORTS)
target_compile_definitions(ifcg PUBLIC GLAD_GLAPI_EXPORT PRIVATE GLAD_GLAPI_EXPORT_BUILD) #

# Diretórios de Include para ifcg
target_include_directories(
    ifcg
    PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
    PRIVATE
        "${IFCG_LIB_BASE_SOURCE_DIR}/glad"
)

# Bibliotecas para linkar com ifcg
target_link_libraries(
    ifcg
    PUBLIC
        OpenGL::GL
        glfw
        glm::glm
)

# Define o diretório de output para executáveis
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# =-=-=-=-=-=-= APLICAÇÃO PRINCIPAL =-=-=-=-=-=-=
set(APP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/tests)
set(APP_MAIN_SOURCE_FILES "${APP_SOURCE_DIR}/main.cpp")

add_executable(ifcg_app ${APP_MAIN_SOURCE_FILES})

# Diretórios de Include para a Aplicação (para encontrar os headers da ifcg)
target_include_directories(
    ifcg_app
    PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
)

# Bibliotecas para linkar com a Aplicação
target_link_libraries(
    ifcg_app
    PRIVATE
        ifcg 
)

# Comando Pós-Build para copiar DLLs no Windows (se ifcg for SHARED)
if (WIN32) #
    add_custom_command(TARGET ifcg_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:ifcg_app>
        $<TARGET_FILE_DIR:ifcg_app>
        COMMAND_EXPAND_LISTS
        COMMENT "Copiando DLLs necessárias para o diretório do executável..."
    )
endif ()

add_custom_command(TARGET ifcg_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:ifcg_app>/resources
)

# Mensagem final para o usuário
message(STATUS "Configuração do CMake concluída. Para construir, use 'make' no diretório de build.")
message(STATUS "Executável estará em: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ifcg_app")
message(STATUS "Biblioteca ifcg estará em: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
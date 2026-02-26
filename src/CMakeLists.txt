# Coleta a lógica do grafo, EXCETO o src/main.cpp
file(GLOB_RECURSE GRAPH_LIB_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/*.cpp"
)

# Cria a biblioteca core do grafo
add_library(graph_core STATIC ${GRAPH_LIB_SOURCES})

target_include_directories(graph_core PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Linka com a lib de JSON que baixamos via Dependências
target_link_libraries(graph_core PUBLIC
    nlohmann_json::nlohmann_json
)

# Executável final apenas linka com o core e adiciona o main.cpp
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_executable(graph main.cpp)
    target_link_libraries(graph PRIVATE graph_core)
endif()

# Copia a pasta resources para a raiz do binário para ser acessível
add_custom_command(TARGET graph POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:graph>/resources
    COMMENT "Copiando 'resources' para o diretório do executável..."
)
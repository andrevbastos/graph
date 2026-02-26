include(GoogleTest)

# Pega todos os arquivos de teste na pasta atual (ex: bfs_test.cpp, dfs_test.cpp)
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Cria o executável de testes
add_executable(graph_tests ${TEST_SOURCES})

# Linka com o graph_core e o gtest_main (que já provê uma função main() para os testes)
target_link_libraries(graph_tests PRIVATE
    graph_core
    GTest::gtest_main
)

# Se os seus testes precisarem ler os arquivos JSON, copiamos a pasta resources para eles também
add_custom_command(TARGET graph_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:graph_tests>/resources
)

# Registra no CTest
gtest_discover_tests(graph_tests)
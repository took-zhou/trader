include_directories(
    "${workspace_dir}/"
)
file(GLOB_RECURSE  trader_source
    "${CMAKE_CURRENT_SOURCE_DIR}/domain/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/entry/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/infra/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/interface/*.cc"
)

add_library(tradelib  ${trader_source})
target_link_libraries(tradelib 
    thosttraderapi_se_6.5.1
    xtptraderapi
    btptraderapi
    ftptraderapi
    gtptraderapi
    mtptraderapi
    oes_api
    pthread
    commonlib
    zmq
    protobuf
    python3.8
    sqlite3
    quickfix
    z
    curl
    boost_system
    dl
 )

file(GLOB_RECURSE test_source
    "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cc"
)

set(shared_libs pthread gmock gtest gtest_main gmock_main tradelib)

foreach(test_source ${test_source})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    target_link_libraries(${test_name} ${shared_libs})
endforeach()

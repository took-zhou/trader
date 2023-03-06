include_directories(
    "${workspace_dir}/"
)
file(GLOB_RECURSE  trader_source
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
)

add_library(tradelib  ${trader_source})
target_link_libraries(tradelib 
    thosttraderapi_se_6.5.1
    xtptraderapi
    btptraderapi
    pthread
    commonlib
    zmq
    protobuf
    python3.8
    sqlite3
 )

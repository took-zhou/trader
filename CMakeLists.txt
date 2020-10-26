
include_directories(
    "${workspace_dir}/"
)
file(GLOB_RECURSE  trader_source
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(tradelib  ${trader_source})
target_link_libraries(tradelib 
    LinuxDataCollect_6.3.15
    thosttraderapi_se_6.3.15
    thosttraderapi_se_6.3.15
    pthread
    commonlib
    zmq
    libgo
    protobuf
 )

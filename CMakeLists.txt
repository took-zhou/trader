aux_source_directory(. srcs_list)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/socket")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/trade_components")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include/log")

aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/socket" socket_list)
#add_library(socketlib SHARED ${socket_list})

aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/trade_components" trade_components)
#add_library(trade_components_libs SHARED ${trade_components})
list(APPEND  srcs_list ${socket_list} ${trade_components})
if(statement)
  
endif(statement)
add_library(tradelib  ${srcs_list})
target_link_libraries(tradelib 
    LinuxDataCollect_6.3.15

    thosttraderapi_se_6.3.15
    thosttraderapi_se_6.3.15
    convertlib
    libgo
 )

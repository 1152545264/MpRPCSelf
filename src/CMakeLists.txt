aux_source_directory(. SRC_LIST)
add_library(mprpc  ${SRC_LIST})

# 依赖关系不正确会导致链接时出问题
target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt)

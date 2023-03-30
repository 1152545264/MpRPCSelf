# set(SRC_LIST calluserservice.cc ../user.pb.cc)
# aux_source_directory(. SRC_LIST)
# aux_source_directory(.. SRC_LIST)

set(UserLists callUserService.cc ../user.pb.cc)
add_executable(UserConsumer ${UserLists})
target_link_libraries(UserConsumer mprpc protobuf)

set(FriendLists callFriendService.cc ../friend.pb.cc)
add_executable(FriendConsumer ${FriendLists})
target_link_libraries(FriendConsumer mprpc protobuf)
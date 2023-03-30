# set(UserProviderLists userservice.cc ../user.pb.cc)
# add_executable(UserProvider ${UserProviderLists})
# target_link_libraries(UserProvider mprpc protobuf)


# set(FriendList friendService.cc ../friend.pb.cc)
# add_executable(FriendProvider ${FriendList})
# target_link_libraries(FriendProvider mprpc protobuf)


aux_source_directory(. SRC_LIST)
aux_source_directory(.. SRC_LIST)
add_executable(provider ${SRC_LIST})
target_link_libraries(provider mprpc protobuf)
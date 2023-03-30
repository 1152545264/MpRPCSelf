// #include<iostream>
// #include<string>

// #include "mprpcapplication.h"
// #include"rpcprovider.h"

// #include "../friend.pb.h"
// #include<vector>

// class FriendService: public fixbug2::FriendService
// {
// public:
//     std::vector<std::string> GetFriendLists(uint32_t userid)
//     {
//         std::cout << "doing GetFriendLists service! userid=" << userid << std::endl;
//         std::vector<std::string> vec;
//         vec.push_back("高阳");
//         vec.push_back("刘勇");
//         vec.push_back("王硕");
//         return vec;
//     }

//     virtual void GetFriendLists(google::protobuf::RpcController* controller,
//         const ::fixbug2::GetFriendListsRequest* request,
//         ::fixbug2::GetFriendListsResponse* response,
//         ::google::protobuf::Closure* done)
//     {
//         uint32_t userid = request->userid();

//         auto friendList = GetFriendLists(userid);

//         response->mutable_result()->set_errcode(0);
//         response->mutable_result()->set_errmsg("");
//         for (const auto& name : friendList)
//         {
//             std::string* p = response->add_friends();
//             *p = name;
//         }
//         done->Run();
//     }
// };

// int main(int argc, char** argv)
// {
//     //调用框架的初始化操作
//     MprpcApplication::Init(argc, argv);

//     //provider是一个rpc网络服务对象，把rpcService对象发布到rpc节点上
//     MprpcProvider provider;
//     provider.NotifyService(new FriendService());

//     //启动一个rpc服务节点，run以后进入阻塞状态，等待远程的rpc调用请求
//     provider.Run();

//     return 0;
// }
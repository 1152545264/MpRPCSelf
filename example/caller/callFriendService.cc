#include<iostream>
#include "mprpcapplication.h"
#include "friend.pb.h"


int main(int argc, char** argv)
{
    //整个程序启动以后，想使用mprpc服务调用，一定需要先调用框架的初始化函数(只初始化一次)
    MprpcApplication::Init(argc, argv);

    fixbug2::FriendService_Stub stub(new MpRpcChannel());
    fixbug2::GetFriendListsRequest request;
    request.set_userid(2000);
    fixbug2::GetFriendListsResponse response;

    MpRpcController controller;
    stub.GetFriendLists(&controller, &request, &response, nullptr); //调用stub类的所有方法都会跳转到RpcChannel::CallMethod() 集中来做所有rpc方法调用的参数序列化和网络发送

    //一次rpc调用完成，读调用的结果
    if (controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        if (0 == response.result().errcode())
        {
            std::cout << "rpc GetFriendLists success!" << std::endl;
            int sz = response.friends_size();
            for (int i = 0; i < sz; ++i)
            {
                std::cout << "index:" << i + 1 << " name: " << response.friends(i) << std::endl;
            }
        }
        else
        {
            std::cout << "rpc GetFriendLists response error : " << response.result().errmsg() << std::endl;
        }
    }


    return 0;
}
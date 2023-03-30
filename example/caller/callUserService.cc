#include<iostream>
#include "mprpcapplication.h"
#include "user.pb.h"
#include "mprpcChannel.h"

int main(int argc, char** argv)
{
    //整个程序启动以后，想使用mprpc服务调用，一定需要先调用框架的初始化函数(只初始化一次)
    MprpcApplication::Init(argc, argv);

    //演示调用远程发布的rpc方法Login
    fixbug::UserServiceRpc_Stub stub(new MpRpcChannel());
    //rpc方法的请求参数
    fixbug::LoginRequest request;
    request.set_name("zhangsan");
    request.set_pwd("123456");
    //rpc方法的响应
    fixbug::LoginResponse response;

    //发起rpc方法的调用 同步的rpc调用过程 MpRPCChannel::CallMethod
    stub.Login(nullptr, &request, &response, nullptr); //调用stub类的所有方法都会跳转到RpcChannel::CallMethod() 集中来做所有rpc方法调用的参数序列化和网络发送

    //一次rpc调用完成，读调用的结果
    if (0 == response.result().errcode())
    {
        std::cout << "rpc login response:" << response.sunccess() << std::endl;
    }
    else
    {
        std::cout << "rpc login response error : " << response.result().errmsg() << std::endl;
    }

    //演示远程发布的rpc方法Register
    fixbug::RegisterRequest req;
    req.set_id(2000);
    req.set_name("Lisi");
    req.set_pwd("66666");
    fixbug::RegisterResponse rsp;
    stub.Register(nullptr, &req, &rsp, nullptr);

    //一次rpc调用完成，读调用的结果
    if (0 == rsp.result().errcode())
    {
        std::cout << "rpc register response:" << response.sunccess() << std::endl;
    }
    else
    {
        std::cout << "rpc register response error : " << response.result().errmsg() << std::endl;
    }

    return 0;
}
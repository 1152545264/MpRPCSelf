#include<iostream>
#include<string>
#include "../user.pb.h"
#include "mprpcapplication.h"
#include"mprpcprovider.h"
#include "../friend.pb.h"
#include<vector>
#include "logger.h"

/**
 * UserService原本是一个本地服务，提供了两个进程内的本地方法 Login和GetFriendLists
 **/

 //RPC服务器 ，提供者
class Userservice:public fixbug::UserServiceRpc //使用rpc服务发布端（rpc服务提供者）
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "doing local service::Login" << std::endl;
        std::cout << "name: " << name << " pwd: " << pwd << std::endl;
        return true;
    }

    bool Register(uint32_t id, std::string name, std::string pwd)
    {
        std::cout << "doing local service Register" << std::endl;
        std::cout << "id: " << id << " name: " << name << " pwd: " << pwd << std::endl;
        return true;
    }

    /********************************************************
     * 1 caller ===> Login(LoginRequest) ====> muduo等网络框架 ===> callee
     * 2 callee根据接受到的Login(LoginRequest)参数 ====> 转移到下面这个重写的Login方法
     ********************************************************/

     //重写基类UserServiceRpc的虚函数 下面这些方法都是框架直接调用的
    virtual void Login(google::protobuf::RpcController* controller,
        const ::fixbug::LoginRequest* request,
        ::fixbug::LoginResponse* response,
        ::google::protobuf::Closure* done)
    {
        //1 从LoginRequest获取参数的值
        auto name = request->name();
        auto pwd = request->pwd();

        //2 执行本地服务并获取返回值
        bool login_result = Login(name, pwd);

        //3 用上面的返回值填写LoginResponse
        //把响应写入，包括错误码 错误消息和返回值
        auto code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("login success!");
        response->set_sunccess(login_result);

        //4一个回调，把LoginResponse发送给rpcClient（或者叫RPC客户端）
        //执行回调操作  执行响应对象数据的序列化和网络发送，由框架完成
        done->Run();
    }

    //注册用的远程方法
    virtual void Register(google::protobuf::RpcController* controller,
        const ::fixbug::RegisterRequest* request,
        ::fixbug::RegisterResponse* response,
        ::google::protobuf::Closure* done)
    {
        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool ret = Register(id, name, pwd);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("sucess");
        response->set_success(ret);

        done->Run();

    }

};


class FriendService: public fixbug2::FriendService
{
public:
    std::vector<std::string> GetFriendLists(uint32_t userid)
    {
        std::cout << "doing GetFriendLists service! userid=" << userid << std::endl;
        std::vector<std::string> vec;
        vec.push_back("高阳");
        vec.push_back("刘勇");
        vec.push_back("王硕");
        return vec;
    }

    virtual void GetFriendLists(google::protobuf::RpcController* controller,
        const ::fixbug2::GetFriendListsRequest* request,
        ::fixbug2::GetFriendListsResponse* response,
        ::google::protobuf::Closure* done)
    {
        uint32_t userid = request->userid();

        auto friendList = GetFriendLists(userid);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        for (const auto& name : friendList)
        {
            std::string* p = response->add_friends();
            *p = name;
        }
        done->Run();
    }
};

int main(int argc, char** argv)
{
    LOG_INFO("first log message!");
    LOG_ERROR("%s:%s:%d", __FILE__,__FUNCTION__,__LINE__)
    //调用框架的初始化操作
    MprpcApplication::Init(argc, argv);

    //provider是一个rpc网络服务对象，把rpcService对象发布到rpc节点上
    MprpcProvider provider;
    provider.NotifyService(new Userservice());
    provider.NotifyService(new FriendService());


    //启动一个rpc服务节点，run以后进入阻塞状态，等待远程的rpc调用请求
    provider.Run();

    return 0;
}
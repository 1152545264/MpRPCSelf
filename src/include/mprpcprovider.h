#pragma once
#include <google/protobuf/service.h>
#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>
#include<muduo/net/InetAddress.h>
#include<memory>
#include<muduo/net/TcpConnection.h>
#include<muduo/net/Buffer.h>
#include<time.h>

#include<functional>
#include<google/protobuf/descriptor.h>
#include<unordered_map>

//框架提供的专门服务发布rpc服务的网络对象类
class MprpcProvider
{
public:
    //此处是框架提供个哦外部是用的，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service* service);

    //启动rpc服务节点，开始提供rpc远程服务
    void Run();
private:
    //新的socket连接回调函数
    void OnConnection(const muduo::net::TcpConnectionPtr&);
    //已建立连接的用户的读写事件回调函数
    void OnMessage(const muduo::net::TcpConnectionPtr&,
        muduo::net::Buffer*,
        muduo::Timestamp);
    
    //Closure的回调操作，用于序列化rpc的响应和网络发送
    void SendRpcResponse(const muduo::net::TcpConnectionPtr&, std::shared_ptr<google::protobuf::Message> );

    //组合了EventLoop
    muduo::net::EventLoop m_eventLoop;

    //service服务类型信息
    struct ServiceInfo
    {
        google::protobuf::Service* m_service; //保存服务对象
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> m_methodMap; //保存服务方法
    };

    //存储注册成功的服务对象和其服务方法的所有信息
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;
};
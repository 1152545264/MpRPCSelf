#pragma once
#include<google/protobuf/service.h>
#include<google/protobuf/descriptor.h>
#include<google/protobuf/message.h>

class MpRpcChannel: public google::protobuf::RpcChannel
{
public:
    /*所有通过stub代理对象调用的rpc方法，都会调用下面这个函数，因此：
    需要在该函数中统一做rpc方法调用的数据序列化、反序列化和网络发送与接收等功能
    */
    virtual void CallMethod(const google::protobuf::MethodDescriptor* method,
        google::protobuf::RpcController* controller,
        const google::protobuf::Message* request,
        google::protobuf::Message* response,
        google::protobuf::Closure* done);
private:

};
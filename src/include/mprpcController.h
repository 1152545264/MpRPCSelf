#pragma once
#include<google/protobuf/service.h>
#include <string>

class MpRpcController: public google::protobuf::RpcController
{
public:
    MpRpcController();
    virtual void Reset();
    virtual bool Failed() const;
    virtual std::string ErrorText() const;
    virtual void SetFailed(const std::string& reason);

    //目前未实现的功能
    virtual void StartCancel();
    virtual bool IsCanceled() const;
    virtual void NotifyOnCancel(google::protobuf::Closure* callback);

private:
    bool m_failed; //RPC方法执行过程中的状态
    std::string m_errtext; //RPC方法执行过程中的错误信息
};

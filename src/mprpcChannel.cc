#include "mprpcChannel.h"
#include<string>
#include<memory>
#include "rpcheader.pb.h"
#include"mprpcapplication.h"
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<error.h>
#include<unistd.h>
#include"mprpcController.h"
#include"zookeeperutil.h"

/***********
 * header_size + service_name method_name args_size + args
 *********/

 //所有通过stub代理对象调用的rpc方法，都会调用下面这个函数，因此：
 //需要在该函数中统一做rpc方法调用的数据序列化、反序列化和网络发送与接收等功能
void MpRpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
    google::protobuf::RpcController* controller,
    const google::protobuf::Message* request,
    google::protobuf::Message* response,
    google::protobuf::Closure* done)
{
    const google::protobuf::ServiceDescriptor* sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();

    //获取参数的序列化字符串长度 args_size
    uint32_t args_size = 0;
    std::string args_str;
    if (request->SerializeToString(&args_str))
    {
        args_size = args_str.size();
    }
    else
    {
        controller->SetFailed("serialize request failed!");
        return;
    }

    //定义rpc请求的header
    mprpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);

    uint32_t header_size = 0;
    std::string rpc_header_str;
    if (rpcHeader.SerializeToString(&rpc_header_str))
    {
        header_size = rpc_header_str.size();
    }
    else
    {
        controller->SetFailed("serialize rpc header error!");
        return;
    }

    //组织待发送的rpc请求的字符串
    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char*)&header_size, 4)); //header_size
    send_rpc_str += rpc_header_str; //rpcheader
    //TODO 是否需要args_size
    send_rpc_str += args_str; //args

    //打印调试信息
    std::cout << "==============================================" << std::endl;
    std::cout << "header_size: " << header_size << std::endl;
    std::cout << "rpc_header_str: " << rpc_header_str << std::endl;
    std::cout << "service_name: " << service_name << std::endl;
    std::cout << "method_name: " << method_name << std::endl;
    std::cout << "args_str: " << args_str << std::endl;
    std::cout << "==============================================" << std::endl;


    //使用TCP编程，完成RPC方法的远程调用
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == errno)
    {
        // char errtxt[512] = { 0 };
        // sprintf(errtxt, "create socket error! errno:%d", errno);
        // controller->SetFailed(errtxt);
        controller->SetFailed("create socket error! errno:" + std::to_string(errno));

        return;
    }

    // std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    // uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());

    //从zookeeper注册中心获取对应远程方法的ip和端口
    ZKClient zkcli;
    zkcli.Start();
    std::string method_path = "/" + service_name + "/" + method_name;
    std::string host_data = zkcli.GetData(method_path.c_str());
    if (host_data == "")
    {
        controller->SetFailed(method_path + " is not exist!");
        return;
    }
    int idx = host_data.find(":");
    if (idx == -1)
    {
        controller->SetFailed(method_path + " address is invalid");
        return;
    }
    auto ip = host_data.substr(0, idx);
    auto port = atoi(host_data.substr(idx + 1, host_data.size() - idx).c_str());
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    //连接rpc服务节点
    if (-1 == connect(clientfd, (struct sockaddr*)&server_addr, sizeof(server_addr)))
    {
        close(clientfd);
        controller->SetFailed("connect error! errno:" + std::to_string(errno));
        return;
    }
    if (-1 == send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0))
    {
        close(clientfd);
        controller->SetFailed("send error! errno:" + std::to_string(errno));
        return;
    }

    //接收RPC请求的响应值
    char recv_buff[1024] = { 0 };
    int recv_size = 0;
    if (-1 == (recv_size = recv(clientfd, recv_buff, 1024, 0)))
    {
        close(clientfd);
        controller->SetFailed("recv error! errno:" + std::to_string(errno));
        return;
    }

    //反序列化rpc调用的响应数据
    // std::string response_str(recv_buff, 0, recv_size); //存在bug recv_buf中遇到'\0'后面的额数据就无法存放
    // if (!response->ParseFromString(response_str))

    if (!response->ParseFromArray(recv_buff, recv_size))
    {
        close(clientfd);
        controller->SetFailed("parse error! response_str:" + std::to_string(errno));
        return;
    }

    close(clientfd);
}
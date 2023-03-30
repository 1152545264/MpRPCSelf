#include "test.pb.h"
#include <iostream>
#include<string>

using namespace std;
using namespace fixbug;


int main()
{
    // LoginResponse rsp;
    // ResultCode* rc = rsp.mutable_result();
    // rc->set_errcode(1);
    // rc->set_errmsg("登录处理失败");

    // auto str = rsp.SerializeAsString();
    // cout << str << endl;

    // LoginResponse rsp2;
    // if (rsp2.ParseFromString(str))
    // {
    //     cout << rsp2.mutable_result()->errcode() << "***" << rsp2.mutable_result()->errmsg();
    // }

    GetFriendListRequestResponse rsp;
    ResultCode* rc = rsp.mutable_result();
    rc->set_errcode(0);
    auto usr1 = rsp.add_friendlist();
    usr1->set_name("zhang san");
    usr1->set_age(20);
    usr1->set_sex(User::MAN);

    cout << rsp.friendlist_size() << endl;

    auto usr2 = rsp.add_friendlist();
    usr2->set_name("Li si");
    usr2->set_age(20);
    usr2->set_sex(User::WOMAN);
    cout << rsp.friendlist_size() << endl;

    return 0;
}


int main1()
{
    //封装了login请求对象的数据
    LoginRequest req;
    req.set_name("zhang san");
    req.set_pwd("123456");

    //对象数据序列化 ==> char*
    string sendStr;
    if (req.SerializeToString(&sendStr))
    {
        cout << sendStr << endl;
    }

    //从sendStr反序列化一个login对象
    LoginRequest req2;
    if (req2.ParseFromString(sendStr))
    {
        cout << req2.name() << "******" << req2.pwd() << endl;
    }


    return 0;
}
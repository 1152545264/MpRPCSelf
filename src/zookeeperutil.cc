#include "zookeeperutil.h"
#include "mprpcapplication.h"
#include "logger.h"

//全局挖槽他很观察器  zkserver给zkclient的通知
void global_watcher(zhandle_t* zh, int type,
    int state, const char* path, void* watcherCtx)
{
    if (type == ZOO_SESSION_EVENT)  //回调的消息类型是和会话相关的消息类型
    {
        if (state == ZOO_CONNECTED_STATE) //zkclient和zkserver连接成功
        {
            sem_t* sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}

ZKClient::ZKClient():m_zhandler(nullptr)
{

}
ZKClient::~ZKClient()
{
    if (m_zhandler != nullptr)
    {
        zookeeper_close(m_zhandler); //关闭句柄 释放资源
    }
}

//zkclient启动连接zkserver
void ZKClient::Start()
{
    auto host = MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    auto port = MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    auto connstr = host + ":" + port;

    /**************************************************************************************
     * zookeeper_mt :多线程版本
     * zookeeper的API客户端程序提供了三个线程：API 调用线程、网络IO线程 (使用的是Poll模型)、watcher回调线程
     *************************************************************************************/
    m_zhandler = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
    if (nullptr == m_zhandler)
    {
        LOG_ERROR("zookeeper_init error");
        exit(EXIT_FAILURE);
    }

    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(m_zhandler, &sem);

    sem_wait(&sem);
    LOG_INFO("zookeeper_init_success");
}

//在zkserver上根据指定的path创建znode节点
void ZKClient::Create(const char* path, const char* data, int datalen, int state)
{
    char path_buff[128] = { 0 };
    int buffer_len = sizeof(path_buff);
    int flag = 0;

    //先判断path表示的节点是否存在，如果存在就不再重复创建了
    flag = zoo_exists(m_zhandler, path, 0, nullptr);
    if (ZNONODE == flag) //表示path的znode节点不存在
    {
        //创建指定path的znode节点了
        flag = zoo_create(m_zhandler, path, data, datalen, &ZOO_OPEN_ACL_UNSAFE, state, path_buff, buffer_len);
        if (ZOK == flag)
        {
            LOG_INFO("znode create success...path:%s", path);
        }
        else
        {
            LOG_ERROR("znode create error...path:%s", path);
            exit(EXIT_FAILURE);
        }
    }
}

//根据参数指定的znode节点路径，获取znode节点的值
std::string ZKClient::GetData(const char* path)
{
    char buffer[64] = { 0 };
    int bufferlen = sizeof(buffer);
    int flag = zoo_get(m_zhandler, path, 0, buffer, &bufferlen, nullptr);
    if (flag != ZOK)
    {
        LOG_ERROR("get znode error...path:%s", path);
        return "";
    }
    else
    {
        return buffer;
    }
}

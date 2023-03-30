#pragma once
#include "mprpcconfig.h"
#include "mprpcChannel.h"
#include"mprpcController.h"

//mprpc是框架的基础类，负责框架的初始化工作
class MprpcApplication
{
public:
    static void Init(int argc, char** argv);
    static MprpcApplication& GetInstance();
    static MprpcConfig& GetConfig();
private:
    static MprpcConfig m_config;;
    MprpcApplication();
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;

};
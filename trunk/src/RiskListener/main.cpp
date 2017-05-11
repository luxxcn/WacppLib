
#include "Common.h"
#include "Log.h"
#include "Threading.h"
#include <signal.h>
#include "DatabaseEnv.h"
#include "Config.h"

#ifndef _RISK_LISTENER_CONFIG
#define _RISK_LISTENER_CONFIG "Config.conf"
#endif // !_RISK_LISTENER_CONFIG


volatile bool gRunning = true;

void _OnSignal(int s)
{
    /* ��Ӧ����̨�ź� */
    switch (s)
    {
    case SIGINT:
    case SIGTERM:
    case SIGABRT:
    case SIGBREAK:
        gRunning = false;
        break;
    }
} 

extern int main(int argc, char **argv)
{
    sLog.Init("log.txt", 1);
    sLog.outString("Start...");

    sConfig.SetSource(_RISK_LISTENER_CONFIG);
    sLog.outString("DatabaseInfo:%s", sConfig.GetStringDefault("DatabaseInfo", "DatabaseInfo").c_str());

    ThreadPool.Startup();

    signal(SIGINT, _OnSignal);
    signal(SIGTERM, _OnSignal);
    signal(SIGABRT, _OnSignal);
    signal(SIGBREAK, _OnSignal);

    while (gRunning)
    {

        Sleep(1000);
    }

    signal(SIGINT, 0);
    signal(SIGTERM, 0);
    signal(SIGABRT, 0);
    signal(SIGBREAK, 0);

    //Sleep(1000);// ��ֹ�̳߳ػ�û������Ϳ�ʼ�رա�
    ThreadPool.Shutdown();
    sLog.Close();

    return 0;
}
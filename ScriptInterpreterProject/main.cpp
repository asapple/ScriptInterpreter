#include"Parser.h"
#include"Interpreter.h"
#include"CMDInterface.h"
#include<set>
//#include<Windows.h>
using namespace std;

// 打印語法樹，方便調試
static void printStep(Step* step);
static void printScript(Script* script);
// 获取CPU的核心数
static DWORD GetNumOfProcess() {
    SYSTEM_INFO sysinfo;
    // 获取操作系统信息
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
}
// socket初始化
static void initServer(WSADATA& wsaData, SOCKET& servSock, struct sockaddr_in& sockAddr) {
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //创建套接字
    servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //绑定套接字
    memset(&sockAddr, 0, sizeof(sockAddr));		//每个字节用0填充
    sockAddr.sin_family = PF_INET;				//使用ipv4
    sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(PORT);			//端口
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //进入监听状态
    listen(servSock, 20);
}
ofstream output;
int main(int argc, char* argv[]) {
    cmdProc(argc, argv);
    Script* script;
    Parser parse;
    script = parse.ParseFile(filePath);
    // 打印生成的語法樹
    if (DebugLevel == 1) {
        output.open(outfilePath);
        printScript(script);
        exit(0);
    }


    // 创建一个线程池
    PTP_POOL tPool;
    tPool = CreateThreadpool(NULL);
    // 设置线程池参数（线程池中的线程数)
    if (dwMaxThread == 0)
        dwMaxThread = 3 * GetNumOfProcess() + 1;
    SetThreadpoolThreadMaximum(tPool, dwMaxThread);// 线程池中最多线程数
    SetThreadpoolThreadMinimum(tPool, 1);// 线程池中最少线程数
    // 给线程池分配回调环境
    TP_CALLBACK_ENVIRON tcEnv;
    InitializeThreadpoolEnvironment(&tcEnv);// 初始化线程池的回调环境
    SetThreadpoolCallbackPool(&tcEnv, tPool);

    WSADATA wsaData;
    SOCKET servSock;
    struct sockaddr_in sockAddr;
    initServer(wsaData, servSock, sockAddr);
    while (true) {
        //接收客户端消息
        SOCKADDR clntAddr;
        int nSize = sizeof(SOCKADDR);
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
        // 回调函数参数传递
        struct Parameter* para = (struct Parameter*)malloc(sizeof(struct Parameter));
        para->script = script;
        para->clntSock = clntSock;
        para->clntAddr = clntAddr;
        para->serSock = servSock;
        // 接受服务，执行回调函数
        TrySubmitThreadpoolCallback(interpreter, para, &tcEnv);
    }

    return 0;
}
// 打印语法树的辅助函数
static void printStep(Step* step) {
    output << "----Speak" << endl;
    if (step->speak != NULL)
        for (int i = 0; i < step->speak->size(); i++)
            output << "--------" << step->speak->at(i) << endl;

    if (step->listenTime.beginTimer != 0 && step->listenTime.endTimer != 0) {
        output << "----Listen:" << endl;
        output << "--------BeginTimer:" << step->listenTime.beginTimer << endl;
        output << "--------EndTimer:" << step->listenTime.endTimer << endl;
    }
    if (step->ansTable.size() > 0) {
        output << "----Branch" << endl;
        for (auto it : step->ansTable) {
            output << "--------" << it.first << "->" << it.second << endl;
        }
    }

    if (step->silense != "") {
        output << "----Silense" << endl;
        output << "--------" << step->silense << endl;
    }
    if (step->defaultStep != "") {
        output << "----DefaultStep" << endl;
        output << "--------" << step->defaultStep << endl;
    }

    output << "----isExit" << endl;
    output << "--------" << step->isExit << endl;
    output << endl;
}
static void printScript(Script* script) {
    output << "打印语法树：" << endl;
    output << "-Script" << endl;
    output << "--Entry:" << script->entry << endl;

    output << "--VarName:" << endl;
    for (int i = 0; i < script->vars.size(); i++)
        output << "----" << script->vars[i] << endl;

    for (auto it : script->stepTable) {
        output << "--" << it.first << endl;
        printStep(it.second);
    }

}
//1.	风格：
//满分10分，其中代码注释4分，命名4分，其它2分。
//2.	设计和实现：
//满分20分，其中数据结构5分，模块划分5分，功能5分，文档5分。
//3.	接口：
//满分10分，其中程序间接口5分，人机接口5分。
//4.	测试：
//满分20分，测试桩10分，自动测试脚本10分
//5.	记法：
//满分10分，文档中对此脚本语言的语法的准确描述。

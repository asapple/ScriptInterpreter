#include"Parser.h"
#include"Interpreter.h"
#include<set>
#include<Windows.h>
using namespace std;
// 打印語法樹，方便調試
void printStep(Step* step);
void printScript(Script* script);
// 获取CPU的核心数
DWORD GetNumOfProcess() {
    SYSTEM_INFO sysinfo;
    // 获取操作系统信息
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
}
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("命令行參數缺少文件路徑；");
        exit(1);
    }
    char* filePath = argv[1];
    Script* script;
    Parser parse;
    script = parse.ParseFile(filePath);
    // 打印生成的語法樹
    // printScript(script);

    // 创建一个线程池
    PTP_POOL tPool;
    tPool = CreateThreadpool(NULL);
    // 设置线程池参数（线程池中的线程数)
    DWORD dwMaxThread = GetNumOfProcess() * 2 + 1;
    SetThreadpoolThreadMaximum(tPool, dwMaxThread);// 线程池中最多线程数
    SetThreadpoolThreadMinimum(tPool, 1);// 线程池中最少线程数
    // 给线程池分配回调环境
    TP_CALLBACK_ENVIRON tcEnv;
    InitializeThreadpoolEnvironment(&tcEnv);// 初始化线程池的回调环境
    SetThreadpoolCallbackPool(&tcEnv, tPool);

    TrySubmitThreadpoolCallback(interpreter, (PVOID)script, &tcEnv);
    return 0;
}

void printStep(Step* step) {
    cout << "Expression: ";
    if (step->speak != NULL)
        for (int i = 0; i < step->speak->size(); i++)
            cout << ' ' << step->speak->at(i);
    cout << endl;
    if (step->listenTime.beginTimer != 0 && step->listenTime.endTimer != 0)
        cout << "Listen: " << step->listenTime.beginTimer << " " << step->listenTime.endTimer << endl;
    if (step->ansTable.size() > 0)
        for (auto it : step->ansTable) {
            cout << it.first << " " << it.second << endl;
        }
    if (step->silense != "")
        cout << "silense: " << step->silense << endl;
    if (step->defaultStep != "")
        cout << "defaultStep: " << step->defaultStep << endl;
    if (step->isExit != 0)
        cout << "isExit: " << step->isExit << endl;
}
void printScript(Script* script) {
    cout << "Entry Step: " << script->entry << endl;

    printf("List of VarName:");
    for (int i = 0; i < script->vars.size(); i++)
        cout << ' ' << script->vars[i];
    cout << endl;

    printf("HashTable:\n");
    for (auto it : script->stepTable) {
        cout << it.first << "{" << endl;
        printStep(it.second);
        cout << "}" << endl;
    }

}
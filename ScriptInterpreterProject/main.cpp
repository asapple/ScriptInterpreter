#include"Parser.h"
#include"Interpreter.h"
#include<set>
#include<Windows.h>
using namespace std;
// ��ӡ�Z���䣬�����{ԇ
void printStep(Step* step);
void printScript(Script* script);
// ��ȡCPU�ĺ�����
DWORD GetNumOfProcess() {
    SYSTEM_INFO sysinfo;
    // ��ȡ����ϵͳ��Ϣ
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
}
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("�����Ѕ���ȱ���ļ�·����");
        exit(1);
    }
    char* filePath = argv[1];
    Script* script;
    Parser parse;
    script = parse.ParseFile(filePath);
    // ��ӡ���ɵ��Z����
    // printScript(script);

    // ����һ���̳߳�
    PTP_POOL tPool;
    tPool = CreateThreadpool(NULL);
    // �����̳߳ز������̳߳��е��߳���)
    DWORD dwMaxThread = GetNumOfProcess() * 2 + 1;
    SetThreadpoolThreadMaximum(tPool, dwMaxThread);// �̳߳�������߳���
    SetThreadpoolThreadMinimum(tPool, 1);// �̳߳��������߳���
    // ���̳߳ط���ص�����
    TP_CALLBACK_ENVIRON tcEnv;
    InitializeThreadpoolEnvironment(&tcEnv);// ��ʼ���̳߳صĻص�����
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
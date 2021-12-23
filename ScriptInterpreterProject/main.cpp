#include"Parser.h"
#include"Interpreter.h"
#include"CMDInterface.h"
#include<set>
//#include<Windows.h>
using namespace std;

// ��ӡ�Z���䣬�����{ԇ
static void printStep(Step* step);
static void printScript(Script* script);
// ��ȡCPU�ĺ�����
static DWORD GetNumOfProcess() {
    SYSTEM_INFO sysinfo;
    // ��ȡ����ϵͳ��Ϣ
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
}
// socket��ʼ��
static void initServer(WSADATA& wsaData, SOCKET& servSock, struct sockaddr_in& sockAddr) {
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //�����׽���
    servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //���׽���
    memset(&sockAddr, 0, sizeof(sockAddr));		//ÿ���ֽ���0���
    sockAddr.sin_family = PF_INET;				//ʹ��ipv4
    sockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(PORT);			//�˿�
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //�������״̬
    listen(servSock, 20);
}
ofstream output;
int main(int argc, char* argv[]) {
    cmdProc(argc, argv);
    Script* script;
    Parser parse;
    script = parse.ParseFile(filePath);
    // ��ӡ���ɵ��Z����
    if (DebugLevel == 1) {
        output.open(outfilePath);
        printScript(script);
        exit(0);
    }


    // ����һ���̳߳�
    PTP_POOL tPool;
    tPool = CreateThreadpool(NULL);
    // �����̳߳ز������̳߳��е��߳���)
    if (dwMaxThread == 0)
        dwMaxThread = 3 * GetNumOfProcess() + 1;
    SetThreadpoolThreadMaximum(tPool, dwMaxThread);// �̳߳�������߳���
    SetThreadpoolThreadMinimum(tPool, 1);// �̳߳��������߳���
    // ���̳߳ط���ص�����
    TP_CALLBACK_ENVIRON tcEnv;
    InitializeThreadpoolEnvironment(&tcEnv);// ��ʼ���̳߳صĻص�����
    SetThreadpoolCallbackPool(&tcEnv, tPool);

    WSADATA wsaData;
    SOCKET servSock;
    struct sockaddr_in sockAddr;
    initServer(wsaData, servSock, sockAddr);
    while (true) {
        //���տͻ�����Ϣ
        SOCKADDR clntAddr;
        int nSize = sizeof(SOCKADDR);
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
        // �ص�������������
        struct Parameter* para = (struct Parameter*)malloc(sizeof(struct Parameter));
        para->script = script;
        para->clntSock = clntSock;
        para->clntAddr = clntAddr;
        para->serSock = servSock;
        // ���ܷ���ִ�лص�����
        TrySubmitThreadpoolCallback(interpreter, para, &tcEnv);
    }

    return 0;
}
// ��ӡ�﷨���ĸ�������
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
    output << "��ӡ�﷨����" << endl;
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
//1.	���
//����10�֣����д���ע��4�֣�����4�֣�����2�֡�
//2.	��ƺ�ʵ�֣�
//����20�֣��������ݽṹ5�֣�ģ�黮��5�֣�����5�֣��ĵ�5�֡�
//3.	�ӿڣ�
//����10�֣����г����ӿ�5�֣��˻��ӿ�5�֡�
//4.	���ԣ�
//����20�֣�����׮10�֣��Զ����Խű�10��
//5.	�Ƿ���
//����10�֣��ĵ��жԴ˽ű����Ե��﷨��׼ȷ������

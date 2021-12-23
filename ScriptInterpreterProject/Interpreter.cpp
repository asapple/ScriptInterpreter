#include "Interpreter.h"
#include"CMDInterface.h"
#include"SQL.h"
#define MAXERRTIMES 3
VOID WINAPI interpreter(PTP_CALLBACK_INSTANCE instance, void* p) {
    cout << "�����µ�����" << endl;
    struct Parameter* param = (struct Parameter*)p;
    Script* script = param->script;
    StepId curStepId = script->entry;
    Step* curStep = script->stepTable[curStepId];
    int errAskNum = 0;
    while (true) {
        // ִ��Speak���������׼�����
        string sendMess;
        if (curStep->speak != nullptr) {
            for (int i = 0; i < curStep->speak->size(); i++) {
                // ����
                if (curStep->speak->at(i)[0] == '$') {
                    string item;
                    SQL::finditem(param->clntSock, curStep->speak->at(i), item);
                    sendMess += item;
                } else {
                    sendMess += curStep->speak->at(i);
                }
            }
        }
        send(param->clntSock, sendMess.c_str(), sendMess.size() + sizeof(char), NULL);
        // ������������սᲽ�裬�����ѭ�����Ͽ�ͨ��
        if (curStep->isExit == 1)
            break;
        // ִ��Listen��ֱ�Ӵӱ�׼��������û���Ը��
        string ask = "";
        listenToUser(param, curStep->listenTime.beginTimer, curStep->listenTime.endTimer, ask);
        // �����һ��StepId
        // ����ǰStep��Ϊ�ղŻ�õ�StepId��Ӧ��Step
        if (ask == "")
            errAskNum++;
        else
            errAskNum = 0;
        getNextStep(script, ask, curStepId, curStep);
        if (errAskNum > MAXERRTIMES) {
            string brkWord = "��ʱ��δ��Ӧ����������Ͽ����ӣ�";
            send(param->clntSock, brkWord.c_str(), brkWord.size() + sizeof(char), NULL);
            break;
        }
    }
    free(param);
    return;
}
static void listenToUser(struct Parameter* param, int startTimer, int stopTimer, string& ask) {
    char szBuffer[MAXBYTE] = "";
    int nNetTimeout = 1000 * stopTimer;
    // recv���ó�ʱ
    setsockopt(param->clntSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&nNetTimeout, sizeof(int));
    recv(param->clntSock, szBuffer, MAXBYTE, NULL);
    ask = szBuffer;
    cout << "recv from " << param->clntSock << ":" << ask << "(size=" << ask.size() << ")" << endl;
    return;
}
static void getNextStep(Script* script, const string ask, StepId& curStepId, Step*& curStep) {
    // ������ܳ�ʱ��Ӧ��askΪ��ʱ
    if (ask == "")
        curStepId = curStep->silense;
    else {
        if (curStep->ansTable.find(ask) != curStep->ansTable.end())
            curStepId = curStep->ansTable[ask];
        else
            curStepId = curStep->defaultStep;

    }
    curStep = script->stepTable[curStepId];
}
#include "Interpreter.h"
#include"CMDInterface.h"
#include"SQL.h"
#define MAXERRTIMES 3
VOID WINAPI interpreter(PTP_CALLBACK_INSTANCE instance, void* p) {
    cout << "创建新的连接" << endl;
    struct Parameter* param = (struct Parameter*)p;
    Script* script = param->script;
    StepId curStepId = script->entry;
    Step* curStep = script->stepTable[curStepId];
    int errAskNum = 0;
    while (true) {
        // 执行Speak（输出到标准输出）
        string sendMess;
        if (curStep->speak != nullptr) {
            for (int i = 0; i < curStep->speak->size(); i++) {
                // 变量
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
        // 如果本步骤是终结步骤，则结束循环，断开通话
        if (curStep->isExit == 1)
            break;
        // 执行Listen（直接从标准输入读入用户意愿）
        string ask = "";
        listenToUser(param, curStep->listenTime.beginTimer, curStep->listenTime.endTimer, ask);
        // 获得下一个StepId
        // 将当前Step置为刚才获得的StepId对应的Step
        if (ask == "")
            errAskNum++;
        else
            errAskNum = 0;
        getNextStep(script, ask, curStepId, curStep);
        if (errAskNum > MAXERRTIMES) {
            string brkWord = "长时间未响应，与服务器断开连接！";
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
    // recv设置超时
    setsockopt(param->clntSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&nNetTimeout, sizeof(int));
    recv(param->clntSock, szBuffer, MAXBYTE, NULL);
    ask = szBuffer;
    cout << "recv from " << param->clntSock << ":" << ask << "(size=" << ask.size() << ")" << endl;
    return;
}
static void getNextStep(Script* script, const string ask, StepId& curStepId, Step*& curStep) {
    // 如果接受超时，应答ask为空时
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
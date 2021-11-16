#include "Interpreter.h"
VOID WINAPI interpreter(PTP_CALLBACK_INSTANCE instance, PVOID param) {
    cout << "Thread id = " << GetCurrentThreadId() << endl;
    Script* script = (Script*)param;
    StepId curStepId = script->entry;
    Step* curStep = script->stepTable[curStepId];
    while (true) {
        // 执行Speak（输出到标准输出）
        if (curStep->speak != NULL)
            for (int i = 0; i < curStep->speak->size(); i++) {
                // 变量
                if (curStep->speak->at(i)[0] == '$')
                    cout << GetCurrentThreadId();
                else {
                    cout << curStep->speak->at(i);
                }
            }
        cout << endl;
        // 如果本步骤是终结步骤，则结束循环，断开通话
        if (curStep->isExit == 1)
            break;
        // 执行Listen（直接从标准输入读入用户意愿）
        string ask = "";
        listenToUser(curStep->listenTime.beginTimer, curStep->listenTime.endTimer, ask);
        // 获得下一个StepId
        // 将当前Step置为刚才获得的StepId对应的Step
        getNextStep(script, ask, curStepId, curStep);
    }
    return;
}
void listenToUser(int startTimer, int stopTimer, string& ask) {
    ask += "\"";
    // ask += "投诉";
    ask += "\",";
    return;
}
void getNextStep(Script* script, const string ask, StepId& curStepId, Step*& curStep) {
    if (ask == "\"\",")
        curStepId = curStep->silense;
    else {
        if (curStep->ansTable.find(ask) != curStep->ansTable.end())
            curStepId = curStep->ansTable[ask];
        else
            curStepId = curStep->defaultStep;

    }
    curStep = script->stepTable[curStepId];
}
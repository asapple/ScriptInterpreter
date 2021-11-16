#include "Interpreter.h"
VOID WINAPI interpreter(PTP_CALLBACK_INSTANCE instance, PVOID param) {
    cout << "Thread id = " << GetCurrentThreadId() << endl;
    Script* script = (Script*)param;
    StepId curStepId = script->entry;
    Step* curStep = script->stepTable[curStepId];
    while (true) {
        // ִ��Speak���������׼�����
        if (curStep->speak != NULL)
            for (int i = 0; i < curStep->speak->size(); i++) {
                // ����
                if (curStep->speak->at(i)[0] == '$')
                    cout << GetCurrentThreadId();
                else {
                    cout << curStep->speak->at(i);
                }
            }
        cout << endl;
        // ������������սᲽ�裬�����ѭ�����Ͽ�ͨ��
        if (curStep->isExit == 1)
            break;
        // ִ��Listen��ֱ�Ӵӱ�׼��������û���Ը��
        string ask = "";
        listenToUser(curStep->listenTime.beginTimer, curStep->listenTime.endTimer, ask);
        // �����һ��StepId
        // ����ǰStep��Ϊ�ղŻ�õ�StepId��Ӧ��Step
        getNextStep(script, ask, curStepId, curStep);
    }
    return;
}
void listenToUser(int startTimer, int stopTimer, string& ask) {
    ask += "\"";
    // ask += "Ͷ��";
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
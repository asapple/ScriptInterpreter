#include "Parser.h"
Script* Parser::ParseFile(char* filePath) {
    script = new Script;
    script->entry = "";
    ifstream f;
    f.open(filePath, ios::in);
    if (f.fail()) {
        // �ļ����_ʧ��������-1
        printf("can not open file %s\n", filePath);
        exit(1);
    }
    string curLineStr;
    curLineNum = 0;
    while (!f.eof()) {
        getline(f, curLineStr);
        curLineNum++;
        trim(curLineStr);
        // ���Կ��С�'#'��ͷ��ע����
        if (curLineStr.empty() || curLineStr[0] == '#') {
            continue;
        } else {
            ParseLine(curLineStr);
        }
    }
    f.close();
    return script;
}
void Parser::trim(string& s) {
    s.erase(0, s.find_first_not_of(BLANK_CHARACTERS));
    s.erase(s.find_last_not_of(BLANK_CHARACTERS) + 1);

}
void Parser::ParseLine(const string line) {
    stringstream s;
    s.str(line);
    vector<string> tokenList;
    string token;
    while (!s.eof()) {
        s >> token;
        // ����'#'��ͷ��token���������������βע�ͣ�
        if (token[0] == '#') {
            break;
        } else {
            tokenList.push_back(token);
        }
    }
    ProcessTokens(tokenList);
}
void Parser::ProcessTokens(vector<string> tokenList) {
    string act = tokenList[0];
    // ����token[0]���������
    if (act == "Step") {
        ProcessStep(tokenList[1]);
    } else if (act == "Speak") {
        ProcessSpeak(tokenList);
    } else if (act == "Listen") {
        ProcessListen(tokenList[1], tokenList[2]);
    } else if (act == "Branch") {
        ProcessBranch(tokenList[1], tokenList[2]);
    } else if (act == "Silence") {
        ProcessSilence(tokenList[1]);
    } else if (act == "Default") {
        ProcessDefault(tokenList[1]);
    } else if (act == "Exit") {
        ProcessExit();
    } else {
        // �����������token�򱨴�
        ParseError(act);
    }
}
void initStep(Step* step) {
    step->speak = NULL;
    step->listenTime.beginTimer = 0;
    step->listenTime.endTimer = 0;
    step->silense = "";
    step->defaultStep = "";
    step->isExit = 0;
}
void Parser::ProcessStep(StepId s) {
    // Script����һ���µ�Step����ʶΪstepId; ���õ�ǰStepΪ�´�����Step
    if (script->stepTable.count(s) == 0) {
        step = new Step;
        initStep(step);
        script->stepTable[s] = step;
    } else {
        // ����StepId�ظ������StepId��ǰ���Ѿ������ˣ�������ǰ��Ĵ���
        step = script->stepTable[s];
    }

    // ������ǵ�һ��Step�������õ�ǰStepΪScript��mainStep
    if (script->stepTable.size() == 1)
        script->entry = s;
}
void Parser::ProcessSpeak(vector<string> tokenList) {
    // token[]��һ�����ʽ��ÿ��token�������ַ�������������'+'
    // ProcessExpression(token[])�õ�Expression����Speak�Լ���Ӧ�ı��ʽ���뵱ǰ��Step
    step->speak = ProcessExpression(tokenList);
}
Expression* Parser::ProcessExpression(vector<string> tokenList) {
    Expression* exp = new Expression;
    int len = tokenList.size();
    for (int i = 1; i < len; i++) {
        // ���Ե��Ӻţ�����token׷�ӵ�Expression�е�List<Item>
        if (tokenList[i] == "+")
            continue;
        exp->push_back(tokenList[i]);
        // ������������Script��List<VarName>��
        if (tokenList[i][0] == '$')
            script->vars.push_back(tokenList[i]);
    }
    return exp;
}
void Parser::ProcessListen(string startTimer, string stopTimer) {
    // 
    startTimer.erase(startTimer.find_last_of(NUMBER) + 1);
    stopTimer.erase(stopTimer.find_last_of(NUMBER) + 1);
    step->listenTime.beginTimer = atoi(startTimer.c_str());
    step->listenTime.endTimer = atoi(stopTimer.c_str());
    if (step->listenTime.beginTimer <= 0)
        ParseError(startTimer);
    if (step->listenTime.endTimer <= 0)
        ParseError(stopTimer);
}
void Parser::ProcessBranch(string answer, string  nextStepId) {
    step->ansTable[answer] = nextStepId;
}
void Parser::ProcessSilence(string nextStepId) {
    step->silense = nextStepId;
}
void Parser::ProcessDefault(string nextStepId) {
    step->defaultStep = nextStepId;
}
void Parser::ProcessExit() {
    step->isExit = 1;
}
void Parser::ParseError(const string errorToken) {
    cout << "�~�������e̎;" << endl;
    if (curLineNum > 0)
        cout << "��ǰ�����Д��飺�� " << curLineNum << " ��;" << endl;
    if (errorToken != "")
        cout << "��ǰ������ Token�飺 " << errorToken << " Ո�z���_�����a�Ƿ���e;" << endl;
    exit(1);
}
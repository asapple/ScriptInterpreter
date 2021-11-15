#include "Parser.h"
Script* Parser::ParseFile(char* filePath) {
    script = new Script;
    script->entry = "";
    ifstream f;
    f.open(filePath, ios::in);
    if (f.fail()) {
        // 文件打開失敗，返回-1
        printf("can not open file %s\n", filePath);
        exit(1);
    }
    string curLineStr;
    curLineNum = 0;
    while (!f.eof()) {
        getline(f, curLineStr);
        curLineNum++;
        trim(curLineStr);
        // 忽略空行、'#'开头的注释行
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
        // 遇到'#'开头的token则处理结束（忽略行尾注释）
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
    // 根据token[0]分情况处理
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
        // 如果不是上述token则报错
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
    // Script创建一个新的Step，标识为stepId; 设置当前Step为新创建的Step
    if (script->stepTable.count(s) == 0) {
        step = new Step;
        initStep(step);
        script->stepTable[s] = step;
    } else {
        // 允许StepId重复，如果StepId在前面已经出现了，不覆盖前面的处理
        step = script->stepTable[s];
    }

    // 如果这是第一个Step，则设置当前Step为Script的mainStep
    if (script->stepTable.size() == 1)
        script->entry = s;
}
void Parser::ProcessSpeak(vector<string> tokenList) {
    // token[]是一个表达式，每个token可能是字符串，变量或者'+'
    // ProcessExpression(token[])得到Expression，将Speak以及对应的表达式存入当前的Step
    step->speak = ProcessExpression(tokenList);
}
Expression* Parser::ProcessExpression(vector<string> tokenList) {
    Expression* exp = new Expression;
    int len = tokenList.size();
    for (int i = 1; i < len; i++) {
        // 忽略掉加号，其它token追加到Expression中的List<Item>
        if (tokenList[i] == "+")
            continue;
        exp->push_back(tokenList[i]);
        // 将变量名存入Script的List<VarName>中
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
    cout << "詞法分析錯處;" << endl;
    if (curLineNum > 0)
        cout << "當前所在行數為：第 " << curLineNum << " 行;" << endl;
    if (errorToken != "")
        cout << "當前分析的 Token為： " << errorToken << " 請檢查腳本代碼是否出錯;" << endl;
    exit(1);
}
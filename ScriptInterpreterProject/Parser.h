#pragma once
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<map>
#include<string>
#include<vector>
#include<algorithm>
#include<sstream>
#define BLANK_CHARACTERS " \r\n\t\v\f"
#define NUMBER "0123456789"
using namespace std;
// 存储语法树的数据结构
typedef string StepId;
typedef string Item;
typedef string Answer;
typedef string VarName;
typedef vector<Item> Expression;
typedef struct {
    int beginTimer;
    int endTimer;
}Listen;
typedef struct {
    Expression* speak;
    Listen listenTime;
    map<Answer, StepId> ansTable;
    StepId silense;
    StepId defaultStep;
    bool isExit;
}Step;
typedef struct {
    StepId entry;
    map<StepId, Step*> stepTable;
    vector<VarName> vars;
}Script;

class Parser {
public:
    Script* ParseFile(char* filePath);
private:
    // 記錄當前行數，腳本文件出現BUG時，將當前行數（即BUG所在行數）返回給用戶，方便用戶調試
    int curLineNum;
    void trim(string& s);
    void ParseLine(const string line);
    void ProcessTokens(vector<string> tokenList);
    void ProcessStep(StepId s);
    void ProcessSpeak(vector<string> tokenList);
    Expression* ProcessExpression(vector<string> tokenList);
    void ProcessListen(string startTimer, string stopTimer);
    void ProcessBranch(string answer, StepId  nextStepId);
    void ProcessSilence(StepId nextStepId);
    void ProcessDefault(StepId nextStepId);
    void ProcessExit();
    void ParseError(const string errorToken);
    Script* script;
    Step* step;
};
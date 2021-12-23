#pragma once
#include"Parser.h"
#include<iostream>

#include<stdio.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <string.h>
#include <stdlib.h>
using namespace std;
struct Parameter {
    Script* script;
    SOCKET clntSock;
    SOCKADDR clntAddr;
    SOCKET serSock;
};
VOID WINAPI interpreter(PTP_CALLBACK_INSTANCE instance, void* param);
void listenToUser(struct Parameter* param, int startTimer, int stopTimer, string& ask);
void getNextStep(Script* script, const string ask, StepId& curStepId, Step*& curStep);
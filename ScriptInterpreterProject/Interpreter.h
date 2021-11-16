#pragma once
#include"Parser.h"
#include<iostream>
#include<Windows.h>
using namespace std;
VOID WINAPI interpreter(PTP_CALLBACK_INSTANCE instance, PVOID param);
void listenToUser(int startTimer, int stopTimer, string& ask);
void getNextStep(Script* script, const string ask, StepId& curStepId, Step*& curStep);
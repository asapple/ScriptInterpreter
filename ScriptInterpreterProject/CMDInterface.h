#pragma once
#include<iostream>
#include<fstream>
#include <stdlib.h> 
using namespace std;
extern int DebugLevel;// ���Եȼ�
extern char* filePath;// �����ļ�·��
extern char* outfilePath;// ����ļ�·��
extern unsigned long dwMaxThread;
extern int PORT;// �˿�
void cmdProc(int argc, char* argv[]);
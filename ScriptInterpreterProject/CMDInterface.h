#pragma once
#include<iostream>
#include<fstream>
#include <stdlib.h> 
using namespace std;
extern int DebugLevel;// 调试等级
extern char* filePath;// 输入文件路径
extern char* outfilePath;// 输出文件路径
extern unsigned long dwMaxThread;
extern int PORT;// 端口
void cmdProc(int argc, char* argv[]);
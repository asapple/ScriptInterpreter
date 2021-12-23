#include "CMDInterface.h"
int DebugLevel = 0;// 调试等级
char* filePath;// 输入文件路径
char* outfilePath;// 输出文件路径
unsigned long dwMaxThread = 0;// 最大线程数量
int PORT = 8080;// 端口
void cmdProc(int argc, char* argv[]) {
    for (int i = 1; i < argc - 1; i++) {
        if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--inputfile")) {
            filePath = argv[i + 1];
            i++;
        } else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--outputfile")) {
            outfilePath = argv[i + 1];
            i++;
        } else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debuglevel")) {
            DebugLevel = std::atol(argv[i + 1]);
            i++;
            if (DebugLevel < 0 || DebugLevel>1) {
                std::cerr << "调试等级设置错误" << std::endl;
            }
        } else if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--threadnumber")) {
            dwMaxThread = std::atol(argv[i + 1]);
            if (dwMaxThread < 1 || dwMaxThread>128) {
                std::cerr << "线程数量设置错误" << std::endl;
            }
        } else if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port")) {
            PORT = std::atol(argv[i + 1]);
            if (dwMaxThread <= 1024) {
                std::cerr << "端口设置错误，请设置为非常用端口（ port > 1024），以免引发端口冲突" << std::endl;
            }
        } else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            ifstream input;
            input.open("help.txt");
        } else {
            std::cerr << "未定义" << argv[i] << "命令" << std::endl;
            std::cerr << "输入 -h/--help 参数，查看命令行接口参数定义" << std::endl;
            exit(0);
        }
    }
}
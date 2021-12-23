#include "CMDInterface.h"
int DebugLevel = 0;// ���Եȼ�
char* filePath;// �����ļ�·��
char* outfilePath;// ����ļ�·��
unsigned long dwMaxThread = 0;// ����߳�����
int PORT = 8080;// �˿�
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
                std::cerr << "���Եȼ����ô���" << std::endl;
            }
        } else if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--threadnumber")) {
            dwMaxThread = std::atol(argv[i + 1]);
            if (dwMaxThread < 1 || dwMaxThread>128) {
                std::cerr << "�߳��������ô���" << std::endl;
            }
        } else if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port")) {
            PORT = std::atol(argv[i + 1]);
            if (dwMaxThread <= 1024) {
                std::cerr << "�˿����ô���������Ϊ�ǳ��ö˿ڣ� port > 1024�������������˿ڳ�ͻ" << std::endl;
            }
        } else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            ifstream input;
            input.open("help.txt");
        } else {
            std::cerr << "δ����" << argv[i] << "����" << std::endl;
            std::cerr << "���� -h/--help �������鿴�����нӿڲ�������" << std::endl;
            exit(0);
        }
    }
}
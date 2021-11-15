#include"Parser.h"
#include<set>
using namespace std;
// ��ӡ�Z���䣬�����{ԇ
void printStep(Step* step) {
    cout << "Expression: ";
    if (step->speak != NULL)
        for (int i = 0; i < step->speak->size(); i++)
            cout << ' ' << step->speak->at(i);
    cout << endl;
    cout << "Listen: " << step->listenTime.beginTimer << " " << step->listenTime.endTimer << endl;
    for (auto it : step->ansTable) {
        cout << it.first << " " << it.second << endl;
    }

    cout << "silense: " << step->silense << endl;
    cout << "defaultStep: " << step->defaultStep << endl;
    cout << "isExit: " << step->isExit << endl;
}
void printScript(Script* script) {
    cout << "Entry Step: " << script->entry << endl;

    printf("List of VarName:");
    for (int i = 0; i < script->vars.size(); i++)
        cout << ' ' << script->vars[i];
    cout << endl;

    printf("HashTable:\n");
    for (auto it : script->stepTable) {
        cout << it.first << "{" << endl;
        printStep(it.second);
        cout << "}" << endl;
    }

}
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("�����Ѕ���ȱ���ļ�·����");
        exit(1);
    }
    char* filePath = argv[1];
    Script* script;
    Parser parse;
    script = parse.ParseFile(filePath);
    printScript(script);
    return 0;
}
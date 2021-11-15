#include"Parser.h"
#include<set>
using namespace std;
// 打印語法樹，方便調試
void printStep(Step* step) {
    cout << "Expression: ";
    if (step->speak != NULL)
        for (int i = 0; i < step->speak->size(); i++)
            cout << ' ' << step->speak->at(i);
    cout << endl;
    if (step->listenTime.beginTimer != 0 && step->listenTime.endTimer != 0)
        cout << "Listen: " << step->listenTime.beginTimer << " " << step->listenTime.endTimer << endl;
    if (step->ansTable.size() > 0)
        for (auto it : step->ansTable) {
            cout << it.first << " " << it.second << endl;
        }
    if (step->silense != "")
        cout << "silense: " << step->silense << endl;
    if (step->defaultStep != "")
        cout << "defaultStep: " << step->defaultStep << endl;
    if (step->isExit != 0)
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
        printf("命令行參數缺少文件路徑；");
        exit(1);
    }
    char* filePath = argv[1];
    Script* script;
    Parser parse;
    script = parse.ParseFile(filePath);
    printScript(script);
    return 0;
}
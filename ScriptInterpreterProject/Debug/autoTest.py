import subprocess
for i in range(1,4):
    command = "E:\\ScriptInterpreter\\ScriptInterpreterProject\\Debug\\ScriptInterpreterProject.exe -d 1 -i" + " E:\\ScriptInterpreter\\ScriptInterpreterProject\\test%d.txt"%i + " -o E:\\ScriptInterpreter\\ScriptInterpreterProject\\Debug\\output\\output%d.txt"%i
    print(command)
    res = subprocess.call(command)# 非堵塞cmd
    print(res)# 成功执行返回1
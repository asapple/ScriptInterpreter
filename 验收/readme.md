//1.	风格：
//满分10分，其中代码注释4分，命名4分，其它2分。
//2.	设计和实现：
//满分20分，其中数据结构5分，模块划分5分，功能5分，文档5分。
//3.	接口：
//满分10分，其中程序间接口5分，人机接口5分。
//4.	测试：
//满分20分，测试桩10分，自动测试脚本10分
//5.	记法：
//满分10分，文档中对此脚本语言的语法的准确描述。

# 风格
## 代码注释：
- 在变量、函数定义、类型定义、重要代码区都有详细的注释
## 命名
依照阿里巴巴集团技术团队《Java 开发手册——嵩山版》编程规约的命名方法，例如：
- 常量：全部大写，e.g. MAXERRTIMES
- 函数：动词 + 名词 e.g.ProcessListen，getNextStep
- 局部变量：首个组成单词的首字母小写，其他组成单词的首字母大写， e.g. errAskNum
## 其他
使用VS编程，在项目属性中提前设置较为优美的代码格式
- 运算符号、赋值符号两端空格
- 左大括号不换行
# 设计和实现
## 数据结构
- 语法树
## 模块划分
分为五个模块
- Parser
  - 语法分析模块
  - 功能：构造语法树
  - 输出：脚本文件
  - 输出：语法树
- Interpreter
  - 客户服务模块
  - 功能：作为线程池的回调函数，响应客户的请求
  - 基于socket的send()、recv()实现交互
- main
  - 主函数模块
  - 功能：创建socket服务器、创建线程池
- CMDInterface
  - 人机接口模块
  - 功能：处理命令行参数
- SQL
  - 数据库查询模块
  - 功能：根据用户信息，查询数据库获取该用户name、amout等数据
## 功能
功能演示
- 单客户端演示
- 多客户端演示
# 测试
## 测试桩
- 语法树生成测试
## 测试脚本
- Python subProcess()
# 记法
- 词法描述
  - 注释：忽略一行中 # 及其之后字符
  - Token:
  - 关键词Token，Token.second存储关键词类别
    - (keyword,Step)->Step
    - (keyword,Speak)->Speak
    - (keyword,Listen)->Listen
    - (keyword,Branch)->Branch
    - (keyword,Silence)->Silence
    - (keyword,Default)->Default
    - (keyword,Exit)->Exit
  - 整数Token，Token.second存储整数大小
    - (number,n)->0..9|0..9(number,n) 
  - 字符串Token，Token.second存储字符串内容
    - (word,str)->0..9|a..z|A..Z|0..9(word,str)|a..z(word,str)|A..Z(word,str)
  - 布尔Token，Token.second存储布尔值
  - (bool,bo)->0|1
- 语义描述
  - 一行一个语句，有如下语句：
  - Step:完整表示一个步骤的所有行为
  - Speak:计算表达式合成一段文字，调用媒体服务器进行语音合成并播放
  - Listen:调用媒体服务器对客户说的话录音，并进行语音识别，语音识别的结果调用“自然语言分析服务”分析客户的意愿
  - Branch:对客户的意愿进行分支处理，不同的意愿，跳转到不同的Step
  - Silence:如果用户不说话，应该跳转到哪个 Step
  - Default:如果客户意愿没有相应匹配，应该跳转到哪个 Step
  - Exit:结束对话
- 各语句生成式描述
  - Step->(keyword,Step)(word,str)
  - Speak->(keyword,Speak)|*words*
  - *words*->(word,str)|(word,str)*words*
  - Listen->(keyword,Listen)(number,n)(number,n)
  - Branch->(keyword,Branch)(word,str)(word,str)
  - Silence->(keyword,Silence)(word,str)
  - Default->(keyword,Default)(word,str)
  - Exit->(keyword,Default)(bool,bo)
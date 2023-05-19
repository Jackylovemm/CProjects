#ifndef FILE_H
#define FILE_H
#include "Struct.h"

void CopyData(char* filename,int UserNum); //文件->结构体
void addInfo(char* filename,char* money,char* io,char* year,char* month,char* day,char* usage); //往对应的文件中添加个人Finance信息
void Synchronize(char* filename,Users user); //结构体->文件
bool CheckDeleteQuery(char* ReadFilename,char* WriteFilename,char* Amount,char* IO,char* Year,char* Month,char* Day,char* Usage);//校验删除查询结果并存入对应文件中,若查询到,则写入对应文件,若未查询到,不进行文件操作,直接关闭读入流 
void DeleteInfo(char* RFName,char* WFName,Users user,int UserNum);//删除信息的操作函数 
void ChangeInfo(char* filename,Users user,int UserNum,char* BAmount,char* BIO,char* BYear,char* BMonth,char* BDay,char* BUsage,char* AAmount,char* AIO,char* AYear,char* AMonth,char* ADay,char* AUsage);
void BudgetOP(char *WFName,char* tag,char* flex,char* fixed,char* free,char* purein);//生成对应预算,并将信息同步 
int count(char* filename);//文件行数计数器 
void Statistics(char* filename,Users user,int UserNum,int year,int month,int day);//相应开支统计,若相应year,month,day未输入,则自动设置为0;
bool Register(char* RId,char* RPass);//校验注册功能 
bool readIn(char* InID,char* InPass); //校验登陆功能 
void showData(char* filename,int x,int y,int UserNum); //多行展示文件内容,x,y为初始坐标值,UserNum为展示的用户编号 
bool change(char* OriID,char* OriPass,char* NewPass);//修改密码函数 步骤:先进行修改->TestCode覆盖NewTestCode->NewTestCode覆盖TestCode 

#endif

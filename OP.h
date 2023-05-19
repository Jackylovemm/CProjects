#ifndef OP_H
#define OP_H

#include <stdio.h>
#include <graphics.h>
#include <ege/sys_edit.h>
#include <sys/types.h>
#include <string>
#include "Struct.h"
using namespace std;

int InitStruct(Users* user); //结构体初始化
void DateSort(PFRec RecArray[],int length);//根据日期由前至后排序 早的在前面,晚的在后面 
void SwapRec(PFRec* Pri1,PFRec* Pri2);//交换结构体的信息
bool RegexMatch(string Regex,char* MatchString); //前面是正则表达式,后面是待匹配的字符串
void TotalInOut(Users user,int UserNum); //统计纯收入,输出,目前剩余 并赋值
void InitField(sys_edit* Field,bool ismultline,int x,int y,int Height,COLORS bkcolor,COLORS txcolor,int maxlen,bool isView,int textNum);//文本框生成函数
void iMage(PIMAGE pimg, const char* filename, int width, int height);//插入图片专用函数
void showData(char* filename,int x,int y,int UserNum);//信息展示函数 

#endif

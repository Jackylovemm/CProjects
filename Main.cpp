#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include "View.h"
#include "OP.h"
#include "File.h"
#include "Struct.h"
using namespace std;

//数组中的每一个元素绑定对应编号每一个用户
   extern float Total[Umax]; //现存总金额 
   extern float Income[Umax]; //纯收入 
   extern float Outcome[Umax]; //纯支出  
   extern Users UserInfo[Umax]; //用户的所有信息
   extern float StatisticIncome[Umax]; //统计界面收入
   extern float StatisticOutcome[Umax]; //统计界面支出
   extern float StatisticTotal[Umax]; //统计界面现存金额 
   extern bool is_Admin; //判断是否为管理员 
//	
//正则表达式规定输入格式 
	//金额,1~10位任意数字,首位不为0; 
	extern	string  MoneyRS;
	//IO ,指定2个字母
	extern	string  IORS;
	//年份,指定1-4位数字,首位不能为0 
	extern	string  YearRS;//左闭右开
	//月份,指定1-2位数字,首位不为0 
	extern	string  MonthRS;//左闭右开
	 //日期,指定1-2位数字,首位不为0 
	extern	string  DayRS;
	 //备注,指定1-20位字母or数字or空格 
	extern	string  UsageRS;
	 //匹配U/A开头,后边数字位数4-9的字符串;即ID输入规范 
	extern	string  rs;   
	 //预算输入规范,10位任意数字,首位不能为0 
	extern	string  BTS;
//	
int main(){
	loginMain();
	closegraph();
	return 0;
}


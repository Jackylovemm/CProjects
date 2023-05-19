#include "Struct.h"
#include<string>
using namespace std;
//数组中的每一个元素绑定对应编号每一个用户
    float Total[Umax]; //现存总金额 
    float Income[Umax]; //纯收入 
    float Outcome[Umax]; //纯支出  
    Users UserInfo[Umax]; //用户的所有信息
    float StatisticIncome[Umax]; //统计界面收入
	 float StatisticOutcome[Umax]; //统计界面支出
     float StatisticTotal[Umax]; //统计界面现存金额 
     bool is_Admin=false; //判断是否为管理员 
	
//正则表达式规定输入格式 
	//金额,1~10位任意数字,首位不为0; 
	string  MoneyRS = "([1-9])(\\d{0,10})";
	//IO ,指定2个字母
	string  IORS = "([IO])";
	//年份,指定1-4位数字,首位不能为0 
	string  YearRS = "([1-9])(\\d{0,4})";//左闭右开
	//月份,指定1-2位数字,首位不为0 
	string  MonthRS = "([1-9])(\\d{0,2})";//左闭右开
	 //日期,指定1-2位数字,首位不为0 
	string  DayRS = "([1-9])(\\d{0,2})";
	 //备注,指定1-20位字母or数字or空格 
	string  UsageRS = "([a-z0-9A-Z ]{1,21})";
	 //匹配U/A开头,后边数字位数4-9的字符串;即ID输入规范 
	string  rs = "([UA])(\\d{4,10})";   
	 //预算输入规范,10位任意数字,首位不能为0 
	string  BTS = "([1-9])(\\d{0,10})";

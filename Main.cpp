#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include "View.h"
#include "OP.h"
#include "File.h"
#include "Struct.h"
using namespace std;

//�����е�ÿһ��Ԫ�ذ󶨶�Ӧ���ÿһ���û�
   extern float Total[Umax]; //�ִ��ܽ�� 
   extern float Income[Umax]; //������ 
   extern float Outcome[Umax]; //��֧��  
   extern Users UserInfo[Umax]; //�û���������Ϣ
   extern float StatisticIncome[Umax]; //ͳ�ƽ�������
   extern float StatisticOutcome[Umax]; //ͳ�ƽ���֧��
   extern float StatisticTotal[Umax]; //ͳ�ƽ����ִ��� 
   extern bool is_Admin; //�ж��Ƿ�Ϊ����Ա 
//	
//������ʽ�涨�����ʽ 
	//���,1~10λ��������,��λ��Ϊ0; 
	extern	string  MoneyRS;
	//IO ,ָ��2����ĸ
	extern	string  IORS;
	//���,ָ��1-4λ����,��λ����Ϊ0 
	extern	string  YearRS;//����ҿ�
	//�·�,ָ��1-2λ����,��λ��Ϊ0 
	extern	string  MonthRS;//����ҿ�
	 //����,ָ��1-2λ����,��λ��Ϊ0 
	extern	string  DayRS;
	 //��ע,ָ��1-20λ��ĸor����or�ո� 
	extern	string  UsageRS;
	 //ƥ��U/A��ͷ,�������λ��4-9���ַ���;��ID����淶 
	extern	string  rs;   
	 //Ԥ������淶,10λ��������,��λ����Ϊ0 
	extern	string  BTS;
//	
int main(){
	loginMain();
	closegraph();
	return 0;
}


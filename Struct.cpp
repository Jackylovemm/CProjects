#include "Struct.h"
#include<string>
using namespace std;
//�����е�ÿһ��Ԫ�ذ󶨶�Ӧ���ÿһ���û�
    float Total[Umax]; //�ִ��ܽ�� 
    float Income[Umax]; //������ 
    float Outcome[Umax]; //��֧��  
    Users UserInfo[Umax]; //�û���������Ϣ
    float StatisticIncome[Umax]; //ͳ�ƽ�������
	 float StatisticOutcome[Umax]; //ͳ�ƽ���֧��
     float StatisticTotal[Umax]; //ͳ�ƽ����ִ��� 
     bool is_Admin=false; //�ж��Ƿ�Ϊ����Ա 
	
//������ʽ�涨�����ʽ 
	//���,1~10λ��������,��λ��Ϊ0; 
	string  MoneyRS = "([1-9])(\\d{0,10})";
	//IO ,ָ��2����ĸ
	string  IORS = "([IO])";
	//���,ָ��1-4λ����,��λ����Ϊ0 
	string  YearRS = "([1-9])(\\d{0,4})";//����ҿ�
	//�·�,ָ��1-2λ����,��λ��Ϊ0 
	string  MonthRS = "([1-9])(\\d{0,2})";//����ҿ�
	 //����,ָ��1-2λ����,��λ��Ϊ0 
	string  DayRS = "([1-9])(\\d{0,2})";
	 //��ע,ָ��1-20λ��ĸor����or�ո� 
	string  UsageRS = "([a-z0-9A-Z ]{1,21})";
	 //ƥ��U/A��ͷ,�������λ��4-9���ַ���;��ID����淶 
	string  rs = "([UA])(\\d{4,10})";   
	 //Ԥ������淶,10λ��������,��λ����Ϊ0 
	string  BTS = "([1-9])(\\d{0,10})";

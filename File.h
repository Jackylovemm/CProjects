#ifndef FILE_H
#define FILE_H
#include "Struct.h"

void CopyData(char* filename,int UserNum); //�ļ�->�ṹ��
void addInfo(char* filename,char* money,char* io,char* year,char* month,char* day,char* usage); //����Ӧ���ļ�����Ӹ���Finance��Ϣ
void Synchronize(char* filename,Users user); //�ṹ��->�ļ�
bool CheckDeleteQuery(char* ReadFilename,char* WriteFilename,char* Amount,char* IO,char* Year,char* Month,char* Day,char* Usage);//У��ɾ����ѯ����������Ӧ�ļ���,����ѯ��,��д���Ӧ�ļ�,��δ��ѯ��,�������ļ�����,ֱ�ӹرն����� 
void DeleteInfo(char* RFName,char* WFName,Users user,int UserNum);//ɾ����Ϣ�Ĳ������� 
void ChangeInfo(char* filename,Users user,int UserNum,char* BAmount,char* BIO,char* BYear,char* BMonth,char* BDay,char* BUsage,char* AAmount,char* AIO,char* AYear,char* AMonth,char* ADay,char* AUsage);
void BudgetOP(char *WFName,char* tag,char* flex,char* fixed,char* free,char* purein);//���ɶ�ӦԤ��,������Ϣͬ�� 
int count(char* filename);//�ļ����������� 
void Statistics(char* filename,Users user,int UserNum,int year,int month,int day);//��Ӧ��֧ͳ��,����Ӧyear,month,dayδ����,���Զ�����Ϊ0;
bool Register(char* RId,char* RPass);//У��ע�Ṧ�� 
bool readIn(char* InID,char* InPass); //У���½���� 
void showData(char* filename,int x,int y,int UserNum); //����չʾ�ļ�����,x,yΪ��ʼ����ֵ,UserNumΪչʾ���û���� 
bool change(char* OriID,char* OriPass,char* NewPass);//�޸����뺯�� ����:�Ƚ����޸�->TestCode����NewTestCode->NewTestCode����TestCode 

#endif

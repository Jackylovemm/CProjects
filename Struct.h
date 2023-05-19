#ifndef STRUCT_H
#define STRUCT_H

#define Umax 40 //�û���� 
#define PFmax 75 //PrivateFinance��Ϣһ����� 
#define FBmax 5  //FBudget ��Ϣһ����� 
#define FNmax 50 //�ļ���� 
    
//���˲�����Ϣ 
typedef struct PriFinance{
	
    float amount; //����orȡ���Ľ��
    char InOrOut[5]; //����orȡ��  
	int year; //������� 
    int month; //�����·� 
    int day; //�������� 
    char Usage[50]; //��ע(��;or��Դ)

}PFRec;

typedef enum BudgetKind{
	Saving=1,  //����Ŀ�� ----1
	Flex,    //��֧  ----2
	Fixed,   //�̶���֧  ----3
 	Free,    //���ɿ�֧   ----4
 	PureIn   //������  ----5 
}Kind;

//����Ԥ����Ϣ
typedef struct FinanceBudget{
 
   Kind kind; //��֧���� 
   float money; //��֧�������
   	
}FBudget; 
 
//�û���Ϣ 
typedef struct UserRecord{
	
	//���������� UserInfo�ļ��� 
	int UserNum;  //�û����,��0��ʼ 
	char name[10]; //�û��� 
	char code[20]; //����
	
	//������ݲ��洢���ļ���,��Ϊ�ṹ������ 
	int Rec_len; //�ִ��¼����   
	int Is_administrators; //�����û��Ƿ�Ϊ����Ա,�����,Ϊ1,������,Ϊ0
	
	
	//�ֱ�����Ӧ��PrivateFinance.txt��FinancialBudget.txt��  
    struct PriFinance List_Finance[PFmax]; //�û���Ӧ�ĸ��˲�����Ϣ
	struct FinanceBudget List_Budget[FBmax]; //�û���Ӧ�Ĳ���Ԥ����Ϣ 
	
}Users; 

#endif 

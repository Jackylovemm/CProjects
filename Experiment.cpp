#include <graphics.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ege/sys_edit.h>
#include <sys/types.h>
#include <regex> 
#include <string>
#include <iostream>
using namespace std;

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
	
	char FileLocate[FNmax];   //��Ӧ���ļ�λ�� 
	
	//�ֱ�����Ӧ��PrivateFinance.txt��FinancialBudget.txt��  
    struct PriFinance List_Finance[PFmax]; //�û���Ӧ�ĸ��˲�����Ϣ
	struct FinanceBudget List_Budget[FBmax]; //�û���Ӧ�Ĳ���Ԥ����Ϣ 
	
}Users; 

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
	 string MoneyRS = "([1-9])(\\d{0,10})";
	//IO ,ָ��2����ĸ
	 string IORS = "([IO])";
	//���,ָ��1-4λ����,��λ����Ϊ0 
	 string YearRS = "([1-9])(\\d{0,4})";//����ҿ�
	//�·�,ָ��1-2λ����,��λ��Ϊ0 
	 string MonthRS = "([1-9])(\\d{0,2})";//����ҿ�
	 //����,ָ��1-2λ����,��λ��Ϊ0 
	 string DayRS = "([1-9])(\\d{0,2})";
	 //��ע,ָ��1-20λ��ĸor����or�ո� 
	 string UsageRS = "([a-z0-9A-Z ]{1,21})";
	 //ƥ��U/A��ͷ,�������λ��4-9���ַ���;��ID����淶 
	 string rs = "([UA])(\\d{4,10})";   
	 //Ԥ������淶,10λ��������,��λ����Ϊ0 
	 string BTS = "([1-9])(\\d{0,10})"; 
	 
void loginMain(); //��¼������
void SignIn(); //ע���˺�������
void ChangePassWord(); //�޸�����������
void MainMenu(); //��¼֮��Ĺ���������,����������Ϣ����ɾ�Ĳ�ģ��,��Ϣͳ��ģ��,����Ԥ��ģ�� 
void Add_Message(); //�����Ӧ���˿�֧��Ϣ 
void Delete_Message(); //ɾ����Ӧ��֧��Ϣ(����ԱȨ��) 
void Change_Message(); //�޸���Ӧ��֧��Ϣ(����ԱȨ��)
void Query_Message(); //��ѯ��Ӧ���˿�֧��Ϣ 
void FinancialBudget(); //����Ԥ����湦����� 
/*����Ϊ���溯��*/
void Statistics(char* filename,Users user,int UserNum,int year,int month,int day); //��Ӧ��֧ͳ��,����Ӧyear,month,dayδ����,���Զ�����Ϊ0; 
void iMage(PIMAGE pimg, const char* filename, int width, int height);//����ͼƬר�ú���
bool change(char* OriID,char* OriPass,char* NewPass);//�޸����빦�ܺ��� 
bool readIn(char* InID,char* InPass); //�˶��˺����뺯�� 
bool Register(char* RId,char* RPass); //У��ע�Ṧ�� 
void InitField(sys_edit* Field,bool ismultline,int x,int y,int Height,COLORS bkcolor,COLORS txcolor,int maxlen,bool isView,int textNum); //����ӵ���ض����Ե������ 
void showData(char* filename,int x,int y,int UserNum); //����չʾ�ļ�����,x,yΪ��ʼ����ֵ,UserNumΪչʾ���û���� 
int count(char* filename); //������Ӧ�ļ������� 
int InitStruct(Users* user); //�ṹ���ʼ��
void CopyData(char* filename,int UserNum); //�ļ�->�ṹ��
void TotalInOut(Users user,int UserNum); //ͳ�ƴ�����,���,Ŀǰʣ�� ����ֵ
bool RegexMatch(string Regex,char* MatchString);//ǰ����������ʽ,�����Ǵ�ƥ����ַ��� 
void addInfo(char* filename,char* money,char* io,char* year,char* month,char* day,char* usage); //����Ӧ���ļ�����Ӹ���Finance��Ϣ 
void Synchronize(char* filename,Users user); //�ṹ��->�ļ�
void DateSort(PFRec RecArray[],int length); //����������ǰ�������� �����ǰ��,����ں��� 
void SwapRec(PFRec* Pri1,PFRec* Pri2);//�����ṹ�����Ϣ;
bool CheckDeleteQuery(char* ReadFilename,char* WriteFilename,char* Amount,char* IO,char* Year,char* Month,char* Day,char* Usage);//У��ɾ����ѯ����������Ӧ�ļ���,����ѯ��,��д���Ӧ�ļ�,��δ��ѯ��,�������ļ�����,ֱ�ӹرն�����  
void DeleteInfo(char* RFName,char* WFName,Users user,int UserNum);//ɾ����Ϣ�Ĳ������� 
void ChangeInfo(char* filename,Users user,int UserNum,char* BAmount,char* BIO,char* BYear,char* BMonth,char* BDay,char* BUsage,char* AAmount,char* AIO,char* AYear,char* AMonth,char* ADay,char* AUsage);
void BudgetOP(char *WFName,char* tag,char* flex,char* fixed,char* free,char* purein);//���ɶ�ӦԤ��,������Ϣͬ�� 

int main(){
	loginMain();
	closegraph();
	return 0;
}

int InitStruct(Users* user){
	*user = {};
}

//���ɶ�ӦԤ��,������Ϣͬ��
void BudgetOP(char *WFName,char* tag,char* flex,char* fixed,char* free,char* purein){
	//�����ʽ:SavingTag:Xxx \n Flex:Xxx \n Fixed:Xxx \n Free:Xxx \n PureIn:Xxx \n(�����,�����������) 
    FILE* fp = fopen(WFName,"w+");
    
    //���������ʽ 
    setfont(20,0,"����"); 
    
    char BRec[15]={""};
    //�Խṹ��Ĳ�ͬ���Ը�ֵ
	UserInfo[0].List_Budget[0].kind=Saving; //����Ŀ��
	UserInfo[0].List_Budget[1].kind=Flex; //��֧
	UserInfo[0].List_Budget[2].kind=Fixed; //�̶���֧
	UserInfo[0].List_Budget[3].kind=Free; //���ɿ�֧
	UserInfo[0].List_Budget[4].kind=PureIn; //������ 
	//�ٽ���Ӧ��ֵ��ֵ��Budget��,ͨ����Ӧ��������Ԥ��
	float FTag,FFlex,FFixed,FFree,FPure;
	FTag = atof(tag);
	FFlex = atof(flex);
	FFixed = atof(fixed);
	FFree = atof(free);
	FPure = atof(purein);
	
    UserInfo[0].List_Budget[0].money=FTag;
    UserInfo[0].List_Budget[1].money=FFlex;
    UserInfo[0].List_Budget[2].money=FFixed;
    UserInfo[0].List_Budget[3].money=FFree;
    UserInfo[0].List_Budget[4].money=FPure;
	
	//�ṹ��->�ļ�
	
	int sign,dec_pl;
	for(int i=0;i<5;i++){
		char Statistic[20]={""},Kind[2]={""};
		char* Money;
        Money = fcvt(UserInfo[0].List_Budget[i].money,0,&dec_pl,&sign);
		//int->string 
		sprintf(Kind,"%d",UserInfo[0].List_Budget[i].kind);
        //����ʽ��ֵ 
        strcat(Statistic,Money);
        strcat(Statistic," ");
        strcat(Statistic,Kind);
        if(i==4){
        	fprintf(fp,"%s",Statistic);
		}else{
			fprintf(fp,"%s\n",Statistic);
		}
	}
	
	float MonthlySave,ExtraSave;
	MonthlySave = FPure-FFlex-FFixed-FFree;
	ExtraSave = MonthlySave-FTag;
	//���PureIn-tag��ֵ>=flex+fixed+free,�ڵ������:
	//���¿ɴ���:Xxx,����SavingTag: Xxx,ExtraSave: Xxx; 
	//���մ�Ԥ��,һ���Լ�ɴ洢:Xxx,��������� 
	if((FPure-FTag)>=(FFlex+FFixed+FFree)){
		xyprintf(450,240,"���¿ɴ洢:%.0fԪ",MonthlySave);
		xyprintf(450,265,"����SavingTag:%.0fԪ",FTag);
		xyprintf(450,290,"ExtraSave:%.0fԪ",ExtraSave);
		xyprintf(450,315,"���մ�Ԥ��,һ���Լ�ɴ洢:%.0fԪ",(12*MonthlySave));
		xyprintf(450,340,"���������");
	}
	
	//���PureIn��ֵ<free+flex+fixed֮��,�ڵ������:
	//���½�͸֧:Xxx,����SavingTag�޷����;
	//���մ�Ԥ��,һ���Լ͸֧:Xxx,�뼰ʱ�������ѽṹ 
	if(FPure<FFree+FFlex+FFixed){
	     MonthlySave = -MonthlySave;
	     xyprintf(450,240,"���½�͸֧:%.0fԪ",MonthlySave);
	     xyprintf(450,265,"����SavingTag���޷����");
	     xyprintf(450,290,"���մ�Ԥ��,һ���Լ͸֧:%.0fԪ",(12*MonthlySave));
	     xyprintf(450,315,"�뼰ʱ�������ѽṹ");
	}else if(FPure-FTag<FFlex+FFixed+FFree){
		//���PureIn-tag��ֵ<flex+fixed+free,�ڵ������:
	    //�޷���ɴ˴洢ԤԤ��,����ɽ��洢Ԥ������� 0~MonthlySave;
		xyprintf(450,240,"�޷���ɴ˴洢Ԥ��:%.0f",FTag);
		xyprintf(450,265,"���齫�洢Ԥ�������0~%.0f",MonthlySave);
		xyprintf(450,290,"��ʱ�������ѽṹ");
	}
	
	//�ر��ļ��� 
	fclose(fp);
}

void ChangeInfo(char* filename,Users user,int UserNum,char* BAmount,char* BIO,char* BYear,char* BMonth,char* BDay,char* BUsage,char* AAmount,char* AIO,char* AYear,char* AMonth,char* ADay,char* AUsage){
	//��������ȡ��Ϣ,���޸�֮ǰ����Ϣ�����ļ��˶�(�˶�ǰ,�ļ�->�ṹ��ͬ����Ϣ),���˶Գɹ�,���ض�Ӧ�±�
	
	char FIO[2]={""},FUs[20]={""};//IO,��ע 
    int TransY,TransM,TransD; //������
	float TransA; //��� 
	bool FindCheck = false; //��ѯ��־
	int index; //��ѯ��Ӧ�±� 
    
    strcpy(FIO,BIO);
    strcpy(FUs,BUsage);
    TransY = atoi(BYear);
    TransM = atoi(BMonth);
    TransD = atoi(BDay);
    TransA = atof(BAmount);
    
    //testcode
    printf("\n%.0f->%s->%d->%d->%d->%s\n",TransA,FIO,TransY,TransM,TransD,FUs);
    	//�ļ�->�ṹ��ͬ�� 
	CopyData(filename,UserNum);
    
	for(int i=0;i<user.Rec_len;i++){
		//testcode
	   if((user.List_Finance[i].amount==TransA)&&(strcmp(user.List_Finance[i].InOrOut,FIO)==0)
	   &&(user.List_Finance[i].year==TransY)&&(user.List_Finance[i].month==TransM)&&
	   (user.List_Finance[i].day==TransD)&&(strcmp(user.List_Finance[i].Usage,FUs)==0)){
	   	index = i; //�±긳ֵ 
	   	FindCheck = true; //��ѯ�ɹ� 
	   	break;
	   }
	}
	// ֱ�ӶԶ�Ӧ�±�Ľṹ����Ϣ�����޸�,�޸�ǰ�ǵð�����ת��
	
    if(RegexMatch(MoneyRS,AAmount)&&RegexMatch(DayRS,ADay)
	&&RegexMatch(IORS,AIO)&&RegexMatch(MonthRS,AMonth)
	&&RegexMatch(UsageRS,AUsage)&&RegexMatch(YearRS,AYear)){
	    if(FindCheck){
		//����ҵ��������ʽ����ȷ
			user.List_Finance[index].amount=atof(AAmount);
			user.List_Finance[index].day=atoi(ADay);
			strcpy(user.List_Finance[index].InOrOut,AIO);
			user.List_Finance[index].month=atoi(AMonth);
			strcpy(user.List_Finance[index].Usage,AUsage);
			user.List_Finance[index].year=atoi(AYear);
	    }else{
	     xyprintf(700,280,"��ѯʧ��");
      	}	
	}else{
		xyprintf(700,280,"�����ʽ����");
	}
	
	// �ٶԽṹ��->�ļ���Ϣͬ��,�ر��ļ��� 
	Synchronize(filename,user);
}

//ɾ����Ϣ�Ĳ������� ���ִ���,ɾ��ʧ�� 
void DeleteInfo(char* RFName,char* WFName,Users user,int UserNum){
	
	FILE *fp = fopen(RFName,"r");
	
	int index; //��¼��Ӧ�±�
	bool FindCheck=false; //��־λ 
	char money[10],IO[5],Year[5],Month[2],Day[2],Usage[20];
	float amount;
	int year,month,day;
	char io[2],usage[20];
	//ֻ��һ����Ϣ,����ֱ�Ӷ�ȡ
	    
		 
	    while(!feof(fp)){
	    	fscanf(fp,"%s%s%s%s%s%s",money,IO,Year,Month,Day,Usage);
		}
		//����Ƚ�,ֱ�Ӹ�ֵ 
		amount=atof(money);
        strcpy(io,IO);
		year=atoi(Year);
		month=atoi(Month);
	    day=atoi(Day);
	    strcpy(usage,Usage);
	    
	    //�ļ�->�ṹ��ͬ��
		CopyData(WFName,0);
		
		//�ṹ��������� 
		for(int i=0;i<user.Rec_len;i++){
		
			if((user.List_Finance[i].amount==amount)&&(user.List_Finance[i].day==day)
			&&(user.List_Finance[i].year==year)&&(user.List_Finance[i].month==month)
			&&(strcmp(user.List_Finance[i].InOrOut,io)==0)&&(strcmp(user.List_Finance[i].Usage,usage)==0)){
				//���ҵ���,��Ӧ�±긳ֵ�Լ���־λ��Ϊ��ֵ 
				index=i; 
				FindCheck=true;
				break;
			} 
			
		}
		
		if(FindCheck){
			for(int j=index;j<user.Rec_len-1;j++){
			     user.List_Finance[j]=user.List_Finance[j+1];
			}
			user.List_Finance[user.Rec_len-1]={};//���һ���ÿ� 
			user.Rec_len--;//ɾ����¼,��¼���ȼ�һ 
			//TestCode
		}
		
		//�ṹ��ͬ���ļ�
		Synchronize(WFName,user);
		//�ر��ļ���
		fclose(fp); 
}

//��Ӧ��֧ͳ��,����Ӧyear,month,dayδ����,���Զ�����Ϊ0;
void Statistics(char* filename,Users user,int UserNum,int year,int month,int day){
	
	int count=0; 
	
	//ͳ��ǰ���ļ�����Ϣ�������ṹ����
	CopyData(filename,UserNum);
	
	int TempY,TempM,TempD;
	float TempAmount=0;
    char TempIO[5];
    
	//��������������ǵ�
    if((year!=0)&&(month!=0)&&(day!=0)){
    	//1.�����վ��������ҺϷ� 
        for(;count<user.Rec_len;count++){
        	TempY=user.List_Finance[count].year;
        	TempM=user.List_Finance[count].month;
        	TempD=user.List_Finance[count].day;
        	TempAmount=user.List_Finance[count].amount;
        	strcpy(TempIO,user.List_Finance[count].InOrOut);
        	
        	//����ҵ��˶�Ӧ�������� 
        	if((year==TempY)&&(month==TempM)&&(day==TempD)){
        		if(strcmp(TempIO,"I")==0){
        		   StatisticIncome[UserNum]+=TempAmount;
			    }else if(strcmp(TempIO,"O")==0){
				   StatisticOutcome[UserNum]+=TempAmount;
			    }
			}
        	
		}
    	
	}else if((year!=0)&&(month!=0)&&(day==0)){
		//2.���������,��������
		for(;count<user.Rec_len;count++){
        	TempY=user.List_Finance[count].year;
        	TempM=user.List_Finance[count].month;
        	TempAmount=user.List_Finance[count].amount;
        	strcpy(TempIO,user.List_Finance[count].InOrOut);
        	
        	//����ҵ��˶�Ӧ������ 
        	if((year==TempY)&&(month==TempM)){
        		if(strcmp(TempIO,"I")==0){
        		   StatisticIncome[UserNum]+=TempAmount;
			    }else if(strcmp(TempIO,"O")==0){
				   StatisticOutcome[UserNum]+=TempAmount;
			    }
			}
		} 
		 
	}else if((year!=0)&&(month==0)&&(day!=0)){
		//3.���������,��������
		  for(;count<user.Rec_len;count++){
        	TempY=user.List_Finance[count].year;
        	TempD=user.List_Finance[count].day;
        	TempAmount=user.List_Finance[count].amount;
        	strcpy(TempIO,user.List_Finance[count].InOrOut);
        	
        	//����ҵ��˶�Ӧ������� 
        	if((year==TempY)&&(day==TempD)){
        		if(strcmp(TempIO,"I")==0){
        		   StatisticIncome[UserNum]+=TempAmount;
			    }else if(strcmp(TempIO,"O")==0){
				   StatisticOutcome[UserNum]+=TempAmount;
			    }
			}
        	
		}
		 
	}else if((year==0)&&(month!=0)&&(day!=0)){
		//4.�����º���,���������
		  for(;count<user.Rec_len;count++){
        	TempM=user.List_Finance[count].month;
        	TempD=user.List_Finance[count].day;
        	TempAmount=user.List_Finance[count].amount;
        	strcpy(TempIO,user.List_Finance[count].InOrOut);
        	
        	//����ҵ��˶�Ӧ���º��� 
        	if((month==TempM)&&(day==TempD)){
        		if(strcmp(TempIO,"I")==0){
        		   StatisticIncome[UserNum]+=TempAmount;
			    }else if(strcmp(TempIO,"O")==0){
				   StatisticOutcome[UserNum]+=TempAmount;
			    }
			}
        	
		}
	}else if((year!=0)&&(month==0)&&(day==0)){
		//5.ֻ������,�������º��� 
		 for(;count<user.Rec_len;count++){
        	TempY=user.List_Finance[count].year;
        	TempAmount=user.List_Finance[count].amount;
        	strcpy(TempIO,user.List_Finance[count].InOrOut);
        	
        	//����ҵ��˶�Ӧ����� 
        	if((year==TempY)){
        		if(strcmp(TempIO,"I")==0){
        		   StatisticIncome[UserNum]+=TempAmount;
			    }else if(strcmp(TempIO,"O")==0){
				   StatisticOutcome[UserNum]+=TempAmount;
			    }
			}
        	
		}
	}else if((year==0)&&(month!=0)&&(day==0)){
		//6.ֻ������,����������� 
		 for(;count<user.Rec_len;count++){
        	TempM=user.List_Finance[count].month;
        	TempAmount=user.List_Finance[count].amount;
        	strcpy(TempIO,user.List_Finance[count].InOrOut);
        	
        	//����ҵ��˶�Ӧ���� 
        	if((month==TempM)){
        		if(strcmp(TempIO,"I")==0){
        		   StatisticIncome[UserNum]+=TempAmount;
			    }else if(strcmp(TempIO,"O")==0){
				   StatisticOutcome[UserNum]+=TempAmount;
			    }
			}
        	
		}
		
	}else if((year==0)&&(month==0)&&(day!=0)){
		//7.ֻ������,����������� 
		for(;count<user.Rec_len;count++){
        	TempD=user.List_Finance[count].day;
        	TempAmount=user.List_Finance[count].amount;
        	strcpy(TempIO,user.List_Finance[count].InOrOut);
        	
        	//����ҵ��˶�Ӧ���� 
        	if((day==TempD)){
        		if(strcmp(TempIO,"I")==0){
        		   StatisticIncome[UserNum]+=TempAmount;
			    }else if(strcmp(TempIO,"O")==0){
				   StatisticOutcome[UserNum]+=TempAmount;
			    }
			}
        	
		}
	}
	//ͳ����� 
	StatisticTotal[UserNum]=StatisticIncome[UserNum]-StatisticOutcome[UserNum];
}

//����������ǰ�������� �����ǰ��,����ں��� 
void DateSort(PFRec RecArray[],int length){
	
	for(int i=0;i<length-1;i++){
		for(int j=i+1;j<length;j++){
		     if(RecArray[i].year<RecArray[j].year){
		     	continue;
			 }else if(RecArray[i].year==RecArray[j].year){
			 	 if(RecArray[i].month<RecArray[j].month){
			 	 	continue;
				 }else if(RecArray[i].month==RecArray[j].month){
				 	if(RecArray[i].day<RecArray[j].day){
				 		continue;
					 }else if(RecArray[i].day==RecArray[j].day){
					 	 continue;
					 }else{
					 	//RecArray[i].day>RecArray[j].day
					 	SwapRec(&RecArray[i],&RecArray[j]);
					 }
				 }else{
				 	//RecArray[i].month>RecArray[j].month
				 	SwapRec(&RecArray[i],&RecArray[j]);
				 }
			 }else{
			 	//RecArray[i].year>RecArray[j].year
			  	  SwapRec(&RecArray[i],&RecArray[j]);
			 }
		}
	}
	
} 
//�����ṹ�����Ϣ; 
void SwapRec(PFRec* Pri1,PFRec* Pri2){
	
	PFRec temp;
	temp = *Pri1;
	*Pri1 = *Pri2;
	*Pri2 = temp;
	
} 
  
//�ṹ��->�ļ�
void Synchronize(char* filename,Users user){
		
    FILE *fp = fopen(filename,"w+");
    
    int line=0;
    //���ļ��ж�������Ϣ�����ַ��������͵� 
    //itoa(num, str, 10); ���԰Ѷ�Ӧ������ת��Ϊ�ַ���,�����������ǻ���(n����);
	//���� atof() ���ڽ��ַ���ת��Ϊ˫���ȸ�����(double)��
	//��ԭ��Ϊ��
    //double atof (const char* str);
	//��ֵ���� 
	
	while(line<user.Rec_len){
		char FileInfo[100]="",Year[4]="",Month[2]="",Day[2]="";
		char *amount;
		
		//ÿ�����Ա��char����,Ȼ��ֵ
		//int->string 
		sprintf(Year,"%d",user.List_Finance[line].year);
		sprintf(Month,"%d",user.List_Finance[line].month);
		sprintf(Day,"%d",user.List_Finance[line].day);
		//float->string
		 int sign,dec_pl;
         amount = fcvt(user.List_Finance[line].amount,0,&dec_pl,&sign);
		//��һ�����ṹ�����Ϣƴ��һ����ʽ�ַ��� 
		strcat(FileInfo,amount);
		strcat(FileInfo," ");
		strcat(FileInfo,user.List_Finance[line].InOrOut);
		strcat(FileInfo," ");
		strcat(FileInfo,Year);
		strcat(FileInfo," ");
		strcat(FileInfo,Month);
		strcat(FileInfo," ");
		strcat(FileInfo,Day);
		strcat(FileInfo," ");
		strcat(FileInfo,user.List_Finance[line].Usage);
		
		if(line==(user.Rec_len-1)){
			fprintf(fp,"%s",FileInfo);
		}else{
			fprintf(fp,"%s\n",FileInfo);
		}
		
			line++; //�Լ�������1 
	
	} 
	fclose(fp);	
} 


//����Ӧ���ļ�����Ӹ���Finance��Ϣ 
void addInfo(char* filename,char* money,char* io,char* year,char* month,char* day,char* usage){
		//a+,��ԭ���ļ�������������� 
	FILE *fp = fopen(filename,"a+");
	
	if(fp==NULL){
		return ;
	}
    fprintf(fp,"\n%s %s %s %s %s %s",money,io,year,month,day,usage);
    
	fclose(fp);
} 

//ǰ����������ʽ,�����Ǵ�ƥ����ַ��� 
bool RegexMatch(string Regex,char* MatchString){
		
		regex expression(Regex); // �ַ������ݸ����캯��������������ʽ
	   	bool ret = regex_match((string)MatchString,expression); //��Ӧ����ƥ��,ƥ����ȷΪtrue,����Ϊfalse 
	   	
	   	return ret;
}

//ͳ�ƴ�����,���,Ŀǰʣ�� ����ֵ 
void TotalInOut(Users user,int UserNum){
	float TotalIn=0,TotalOut=0,total=0;
	for(int i=0;i<user.Rec_len;i++){
		if(strcmp(user.List_Finance[i].InOrOut,"I")==0){
			TotalIn+=user.List_Finance[i].amount;
		}else if(strcmp(user.List_Finance[i].InOrOut,"O")==0){
			TotalOut+=user.List_Finance[i].amount;
		}
	}
	Income[UserNum]=TotalIn;
	Outcome[UserNum]=TotalOut;
	Total[UserNum]=(TotalIn-TotalOut);
}

//�ļ����������� 
int count(char* filename){
	FILE *fp=fopen(filename,"r");
	
	if(fp==NULL){
		return -1;
	}
	
	int sum=0;
	char buff[100];
	while(!feof(fp)&&fgets(buff,100,fp))
	{
		sum++;
	}
	fclose(fp);
	return sum;
}

//�޸����뺯�� 
//����:�Ƚ����޸�->TestCode����NewTestCode->NewTestCode����TestCode 
bool change(char* OriID,char* OriPass,char* NewPass){
	
	if(strcmp(OriPass,NewPass)==0){
	   return false;
	}
	
	//1.ԭ�����IDȷ��
	FILE *fp,*fp1,*fp2,*fp3,*fp4;
	fp = fopen("TestCode.txt","r");
	fp1 = fopen("NewTestCode.txt","w+");
	fp2 = fopen("TestCode.txt","r+");
    //�ҵ���Ҫ�޸��������һ�� ʵ�� 
	int line=0;
	//ȷ���Ƿ��ҵ� 
	bool isFound = false;
	char ID[10],Code[20];
	while(!feof(fp))
	{
		line++;
		fscanf(fp,"%s%s",ID,Code);

		if(strcmp(OriID,ID)==0&&strcmp(OriPass,Code)==0){
		    isFound = true;
			break;
		}
		
	}
	
	//�����Ҫ����������� 
    if(isFound){
      	char str[100]="",str1[100]="";
	int i=0;
	while(!feof(fp2)){
		i++;
		//�Ƚ������Ƿ����
		if(i==line){

            fscanf(fp2,"%s%s",ID,Code);
		    strcat(ID," ");
		    strcat(ID,NewPass);
			strcat(str,ID); 
		
		}else{
			fgets(str,50,(FILE*)fp2);
		}
		
		fprintf(fp1,"%s",str); 
	}
	
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	
	fp3 = fopen("NewTestCode.txt","r+");
	fp4 = fopen("TestCode.txt","w+");
	char str3[100];
	while(fgets(str3,50,(FILE*)fp3)!=NULL){
		fputs(str3,fp4);
	}
	fclose(fp3);
	fclose(fp4);
	 }else{
	  fclose(fp);
	  fclose(fp1);
	  fclose(fp2);
	}
	return isFound;
}
/*
       ���� sys_edit xxx;Ȼ��������Խ��и�ֵ 
	   bool ismultline //�Ƿ����
	   int x,int y; //��Ӧ��x��y����
	   int Height; //���ָ߶�
	   COLORS bkcolor; //������ɫ 
	   COLORS txcolor; //������ɫ 
	   int maxlen; //����������󳤶�
	   bool isView; //�����Ƿ�ɼ� 
	   int textNum; //��������򳤶� 
	   Ĭ������focus() �� ����Ϊ���� 
*/

//�ı������ɺ��� 
void InitField(sys_edit* Field,bool ismultline,int x,int y,int Height,COLORS bkcolor,COLORS txcolor,int maxlen,bool isView,int textNum){
	
	Field->create(ismultline);     //����:false����,true���� 
    Field->move(x,y);       //����λ�� 
	Field->size(Height * textNum, Height + 5);	//���óߴ�
	Field->setbgcolor(bkcolor);					//���ñ�����ɫ
	Field->setcolor(txcolor);					//����������ɫ
	Field->setfont(Height, 0, "����");	//��������

	Field->setmaxlen(maxlen);					//����������󳤶�
	Field->visable(isView);					//ʹ�ɼ�
    Field->setfocus();                       //���ý��� 
        
}
//У��ע�Ṧ�� 
bool Register(char* RId,char* RPass){
	//a+,��ԭ���ļ�������������� 
	FILE *fp = fopen("TestCode.txt","a+");
	char ID[10],Code[20];
	
	bool CheckR = true;
	while(!feof(fp)){
		fscanf(fp,"%s%s",ID,Code);
		if(strcmp(ID,RId)==0){
			CheckR = false;
			break;
		}
	}
	if(CheckR){
	    fprintf(fp,"\n%s %s",RId,RPass);
	}
	fclose(fp);
	return CheckR;
}

//У���½ 
bool readIn(char* InID,char* InPass){
	
		FILE *fp = fopen("TestCode.txt","r");
	        char ID[10],Code[20];
	        bool Check=false;
	        
	while(!feof(fp)){
		fscanf(fp,"%s%s",ID,Code);
		if(strcmp(ID,InID)==0&&strcmp(Code,InPass)==0){
			Check=true;
			break;
		}
	}
	
    //�ر��ļ��� 
	fclose(fp);
	return Check; 
} 

//У��ɾ����ѯ����������Ӧ�ļ���,����ѯ��,��д���Ӧ�ļ�,��δ��ѯ��,�������ļ�����,ֱ�ӹرն����� 
bool CheckDeleteQuery(char* ReadFilename,char* WriteFilename,char* Amount,char* IO,char* Year,char* Month,char* Day,char* Usage){
	//��ȡ��Ӧ�ļ��е�����,�򿪷�ʽr 
	FILE *fp = fopen(ReadFilename,"r");
	FILE *fp1 = fopen(WriteFilename,"w+");
	char FileA[10]={""},FileI[2]={""},FileY[5]={""},FileM[3]={""},FileD[3]={""},FileU[20]={""},Rec[100]={""};
	
	 bool DQuery=false;
	while(!feof(fp)){
		fscanf(fp,"%s%s%s%s%s%s",FileA,FileI,FileY,FileM,FileD,FileU);
		//�ļ���Ϣ��ƥ��,�����ַ����洢,��д���Ӧ�ļ� 
		if(strcmp(Amount,FileA)==0&&strcmp(IO,FileI)==0&&strcmp(Year,FileY)==0&&strcmp(Month,FileM)==0&&strcmp(Day,FileD)==0&&strcmp(Usage,FileU)==0){
			//������Ϣ����Rec�ַ�����
			strcat(Rec,FileA);
			strcat(Rec," ");
			strcat(Rec,FileI);
			strcat(Rec," ");
			strcat(Rec,FileY);
			strcat(Rec," ");
			strcat(Rec,FileM);
			strcat(Rec," ");
			strcat(Rec,FileD);
			strcat(Rec," ");
			strcat(Rec,FileU);
			//��Rec�ַ���д�� 
		    fprintf(fp1,"%s",Rec);
			
			DQuery=true;
			break;
		}
	}

    //�ر��ļ������� 
	fclose(fp);
	fclose(fp1);
	return DQuery; 
}

//����ͼƬר�ú��� 
void iMage(PIMAGE pimg, const char* filename, int width, int height){
		PIMAGE temp = newimage();
	    getimage(temp, filename);
	
	if(getwidth(pimg) != width || getheight(pimg) != height)
		resize(pimg, width, height);
		
	putimage(pimg, 0, 0, width, height, temp, 0, 0, getwidth(temp), getheight(temp));

	delimage(temp);
}

//�ļ�->�ṹ�� 
void CopyData(char* filename,int UserNum){
	
	//�ҵ�ָ���ļ�,���ļ��ڵ���Ϣչʾ�������洢����Ӧ�ṹ���� 
    FILE *fp = fopen(filename,"r");
    
    //��ʼ���ṹ��
	InitStruct(&UserInfo[UserNum]); 
    
    int line=0;
    //���ļ��ж�������Ϣ�����ַ��������͵� 
	//atoi(char *str)���԰Ѷ�Ӧ��strת��Ϊ���� ��int���� 
    //itoa(num, str, 10); ���԰Ѷ�Ӧ������ת��Ϊ�ַ���,�����������ǻ���(n����);
	//���� atof() ���ڽ��ַ���ת��Ϊ˫���ȸ�����(double)��
	//��ԭ��Ϊ��
    //double atof (const char* str);
	//��ֵ���� 
	char money[10],IO[5],Year[5],Month[2],Day[2],Usage[20];
	while(!feof(fp)&&line<count(filename)){
		
		fscanf(fp,"%s%s%s%s%s%s",money,IO,Year,Month,Day,Usage);
		UserInfo[UserNum].List_Finance[line].amount=atof(money);
        strcpy(UserInfo[UserNum].List_Finance[line].InOrOut,IO);
		UserInfo[UserNum].List_Finance[line].year=atoi(Year);
		UserInfo[UserNum].List_Finance[line].month=atoi(Month);
		UserInfo[UserNum].List_Finance[line].day=atoi(Day);
	    strcpy(UserInfo[UserNum].List_Finance[line].Usage,Usage);
		UserInfo[UserNum].Rec_len++;//����һ������,���ȼ�1 
		line++;//�����Լ� 
	} 
	fclose(fp);	
}

//��Ϣչʾ���� 
void showData(char* filename,int x,int y,int UserNum){
	
	//�ҵ�ָ���ļ�,���ļ��ڵ���Ϣչʾ�������洢����Ӧ�ṹ���� 
    FILE *fp = fopen(filename,"r");
    
    int fontHeight = 20;
    setfont(fontHeight,0,"����");

    //�ļ�->�ṹ��
    CopyData(filename,UserNum);
    if(UserInfo[UserNum].Rec_len==0){
    	outtextxy(x,y,"��δ��ѯ��������Ϣ");
    	fclose(fp);
    	return ;
	}
    //�·�ΪSaveFInfo���� 
    int line=0;
    //���ļ��ж�������Ϣ�����ַ��������͵� 
	//atoi(char *str)���԰Ѷ�Ӧ��strת��Ϊ���� ��int���� 
    //itoa(num, str, 10); ���԰Ѷ�Ӧ������ת��Ϊ�ַ���,�����������ǻ���(n����);
	//���� atof() ���ڽ��ַ���ת��Ϊ˫���ȸ�����(double)��
	//��ԭ��Ϊ��
    //double atof (const char* str);
	//��ֵ���� 
	
	//locate��¼�仯λ�� 
	int locatex[6],locatey,Orix,Oriy; 
	Orix=x; Oriy=y;  //��¼��ʼλ�� 
	for(int i=0;i<6;i++){
		locatex[i]=Orix+(i*260);
	}
	locatey = y+40;
	char money[10],IO[5],Year[5],Month[2],Day[2],Usage[20];
	while(!feof(fp)&&line<count(filename)){
		
		fscanf(fp,"%s%s%s%s%s%s",money,IO,Year,Month,Day,Usage);

        xyprintf(locatex[0],locatey,money);
        xyprintf(locatex[1],locatey,IO);
        xyprintf(locatex[2],locatey,Year);
        xyprintf(locatex[3],locatey,Month);
        xyprintf(locatex[4],locatey,Day);
        xyprintf(locatex[5],locatey,Usage);
        locatey += fontHeight;
		line++;//�����Լ� 
	}
	fclose(fp);
	
}

//��¼������
void loginMain(){
 
	initgraph(640,480,0);			
	setcaption("��½������");
	
	//��������� 
	sys_edit editBox,passBox;
    InitField(&editBox,false,190,100,25,WHITE,BLACK,10,true,10);
    InitField(&passBox,false,190,180,25,WHITE,BLACK,20,true,10);
    
	//���ñ���ͼƬ 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"��½.jpg",640,480);
	putimage(0,0,img);
	
    setbkcolor(WHITE);
	setbkmode(TRANSPARENT);
	
	setcolor(RED);
	setfont(70,15,"����");
	setlinewidth(2); 
    outtextxy(0,0,"���˲���������");
    
    setfont(40,10,"����");
    xyprintf(100,100,"�û���:");
    xyprintf(100,180,"����:");
    
    //�������屳��ɫΪ��ɫ 
	setbkcolor(EGERGB(0xFC,0xFC,0XFC));
	
		    setfont(30,15,"����");
		    setlinewidth(1);
		
		    rectangle(100,320,220,390);
			rectangle(260,320,380,390);
			rectangle(420,320,540,390);
			
			setcolor(EGERGB(44,0,15));
			
			outtextxy(130,337,"ע��");
			outtextxy(290,337,"��½");
			outtextxy(420,337,"�޸�����");
			
			//���������ж�������ʽ
			string Admin = "([A])(\\d{4,10})";
			string User = "([U])(\\d{4,10})"; 
			
			//���������Ϣ 
			mouse_msg msg;
	
	for(;is_run();delay_fps(60)){
		
	    bool ChangeClick = false;//ȷ���Ƿ����޸����� 
	    bool RegisterClick = false; //ȷ���Ƿ���ע��
	    bool SignClick = false; //ȷ���Ƿ�����¼ 
		
		
		while(mousemsg()){
			msg = getmouse();
			
		if((msg.x>=100&&msg.x<=220)&&(msg.y>=320&&msg.y<=390)){
					
			if(msg.is_left()&&msg.is_down()){
				RegisterClick = true; 	
			}
			
		}else if((msg.x>=260&&msg.x<=380)&&(msg.y>=320&&msg.y<=390)){
		
			if(msg.is_left()&&msg.is_down()){
				SignClick = true;
			}
			
		}else if((msg.x>=420&&msg.x<=540)&&(msg.y>=320&&msg.y<=390)){
					
			if(msg.is_left()&&msg.is_down()){
				ChangeClick = true; 
			}
	
		} 
		
		if(SignClick){
	    //�˶��˺������Ƿ���ȷ 
		//���뻺����
	     int buffSize = 100,passSize = 100;
	    char strBuff[10] = "",strPass[20] = ""; 
	    //��ȡ��������� 
		editBox.gettext(buffSize, strBuff);
		passBox.gettext(passSize, strPass);
	
	     if(readIn(strBuff,strPass)){
	     	//��Ϊ����Ա 
	     	if(RegexMatch(Admin,strBuff)){
	     		is_Admin = true;
			 }
			 //��Ϊ�û�
			 if(RegexMatch(User,strBuff)){
			 	is_Admin = false;
			 } 
	     	
	     	cleardevice();
	     	editBox.destory();
			passBox.destory();
			Sleep(100); 
		    MainMenu();
		   }else{
		   	setfont(30,10,"����");
		   	setcolor(RED); 
		   	outtextxy(130,450,"�������,�����¼�������!");
		   	editBox.settext("");
		   	passBox.settext("");
		   }
        }	 
 
	    if(ChangeClick){
	    	cleardevice();
	     	editBox.destory();
			passBox.destory();
			Sleep(100);
			ChangePassWord();	
		}
		if(RegisterClick){
			cleardevice();
	     	editBox.destory();
			passBox.destory();
			Sleep(100);
			SignIn();
		}
	}
}
 
	cleardevice(); 
} 

//ע���˺�������
void SignIn(){
	
	initgraph(720,480,0);
	setcaption("ע���˺�");
	
	//���ñ���ͼƬ 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"ע��.jpg",720,480);
	putimage(0,0,img);
	
	setbkmode(TRANSPARENT);
	
	setcolor(BLACK);
	setfont(70,15,"����");
	setlinewidth(2); 
    outtextxy(0,0,"ע���˺�");
    
    setfont(40,10,"����");
    xyprintf(100,100,"�û���:");
    xyprintf(100,170,"����:");

    //�������屳��ɫΪ��ɫ 
	setbkcolor(EGERGB(0xFC,0xFC,0XFC));
	
	        setfont(30,0,"����");
		    setlinewidth(1);
		    
		
		    rectangle(100,320,220,390);
			rectangle(260,320,380,390);
			
			setcolor(EGERGB(44,0,15));
			
			outtextxy(130,337,"ȷ��");
			outtextxy(290,337,"����");
			
			//���������Ϣ 
			mouse_msg msg1;
			
            //���������
			sys_edit RID,RCode; //Register ID&&Register PassCode
			InitField(&RID,false,210,100,25,WHITE,BLACK,10,true,10);
			InitField(&RCode,false,210,170,25,WHITE,BLACK,20,true,10);
			//����������ʾ
			 setfont(20,0,"����");
			 setcolor(RED);
			xyprintf(140,10,"�û�����A/U��ͷ,�󷽽�4~9������(ע���ʽ�淶)");
			xyprintf(140,40,"����A��ͷΪ����Ա,��ʹ���޸���ɾ����¼,U��ͷ���޷�ʹ��");
		    
						
		for(;is_run();delay_fps(60)){
		
	    bool CheckClick = false;//�Ƿ���ȷ�� 
	    bool ReturnClick = false; //�Ƿ������� 

		
		while(mousemsg()){
			msg1 = getmouse();
			
		if((msg1.x>=100&&msg1.x<=220)&&(msg1.y>=320&&msg1.y<=390)){
					
			if(msg1.is_left()&&msg1.is_down()){
				CheckClick = true; 		
			}
			
		}else if((msg1.x>=260&&msg1.x<=380)&&(msg1.y>=320&&msg1.y<=390)){
		
			if(msg1.is_left()&&msg1.is_down()){
				ReturnClick = true;
			}
			
		}
	} 
	    if(CheckClick){
        	
		regex expression(rs); // �ַ������ݸ����캯��������������ʽ
		   	
	   	//����ע����û���Ϣ���뵽��Ӧ�ļ���
	   	 int IDSize=10,CodeSize=20;
	   	char ID[10],Code[20];
	   	RID.gettext(IDSize,ID);
	   	RCode.gettext(CodeSize,Code);
	   	
	   	bool ret = regex_match((string)ID,expression); //ID��Ӧ����ƥ��,ƥ����ȷΪtrue,����Ϊfalse 
	   	
	   	//����Register��������,����һ��bool���͵�ֵ 
		if(ret){
			outtextxy(130,400,"ע��ɹ�!");
		     Register(ID,Code);
		    
		}else{
	       outtextxy(130,400,"ע��ʧ��:�����ʽ/�Ѵ��ڸ��û�");
		}
		
	    RID.settext("");
	    RCode.settext("");
			
		}
		if(ReturnClick){
			
	    cleardevice();
		RID.destory();
		RCode.destory();
        Sleep(100); 
		loginMain(); 
			
		}	
   }
   cleardevice();
 	
}

//�޸�����������
void ChangePassWord(){
	
	initgraph(640,480,0);
	setcaption("�޸�����");
	
	//ͬ��,���ñ���ͼƬ 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"�޸�����.jpg",640,480);
	putimage(0,0,img);
	
	setcolor(RED);
	setfont(70,15,"����");
	setlinewidth(2); 
    outtextxy(0,0,"�޸�����");
    
    setfont(40,10,"����");
    xyprintf(100,100,"�û���:");
    xyprintf(100,150,"ԭ����:");
    xyprintf(100,200,"������:");
//    xyprintf(100,250,"ȷ������:");
    
    //�������屳��ɫΪ��ɫ 
	setbkcolor(EGERGB(0xFC,0xFC,0XFC));
	
	        setfont(30,0,"����");
		    setlinewidth(1);
		    setbkmode(TRANSPARENT);
		
		    rectangle(100,320,220,390);
			rectangle(260,320,380,390);
			
			setcolor(EGERGB(44,0,15));
			
			outtextxy(130,337,"ȷ��");
			outtextxy(290,337,"����");
			
			//�����������Ϣ
			sys_edit CID,COriCode,CNewCode; //ID,ԭ����,������
			InitField(&CID,false,190,100,25,WHITE,BLACK,10,true,10);
			InitField(&COriCode,false,190,150,25,WHITE,BLACK,20,true,10);
			InitField(&CNewCode,false,190,200,25,WHITE,BLACK,20,true,10); 
			
			//���������Ϣ 
			mouse_msg msg2;
			
		for(;is_run();delay_fps(60)){
		
	    bool CheckClick1 = false;//�Ƿ���ȷ�� 
	    bool ReturnClick1 = false; //�Ƿ������� 

		
		while(mousemsg()){
			msg2 = getmouse();
			
		if((msg2.x>=100&&msg2.x<=220)&&(msg2.y>=320&&msg2.y<=390)){
					
			if(msg2.is_left()&&msg2.is_down()){
				CheckClick1 = true; 		
			}
			
		}else if((msg2.x>=260&&msg2.x<=380)&&(msg2.y>=320&&msg2.y<=390)){
		
			if(msg2.is_left()&&msg2.is_down()){
				ReturnClick1 = true;
			}
			
		}
	} 
	    if(CheckClick1){
	   	
	   	//һͨ�ļ�����.....���޸��������û���Ϣ������Ӧ�ļ���(CID,COriCode,CNewCode)
		 int CIDSize=10,CCodeSize=20;
	   	char ID[10],Code[20],NewCode[20];
	    CID.gettext(CIDSize,ID);
	    COriCode.gettext(CCodeSize,Code);
	    CNewCode.gettext(CCodeSize,NewCode);
	   	
	   	if(change(ID,Code,NewCode)){
	   	    outtextxy(130,400,"�޸ĳɹ�!");
		}else{
			outtextxy(130,400,"�޸�ʧ��!");
		}
		    CID.settext("");
			COriCode.settext("");
			CNewCode.settext("");
	
		}
		if(ReturnClick1){
			
		 cleardevice();
		 CID.destory();
		 COriCode.destory();
		 CNewCode.destory();
		 Sleep(100);
		 loginMain(); 
			
		}	
   }

}

//��¼֮��Ĺ���������,����������Ϣ����ɾ�Ĳ�ģ��,��Ϣͳ��ģ��,����Ԥ��ģ�� 
void MainMenu(){ 
	
	initgraph(1440,800,0);			
	setcaption("����������");
	
		//���ñ���ͼƬ 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"����������.jpg",1440,800);
	putimage(0,0,img);
	
    setbkcolor(WHITE);
	
	setcolor(BLACK);
	setbkmode(TRANSPARENT);

    int HeadHeight = 30;
    setfont(HeadHeight,0,"����");
    xyprintf(10,10,"����,Xxx");
    
    int ButtonHeight = 50;
    setfont(ButtonHeight,0,"����");
    xyprintf(570,10,"����������");
    xyprintf(40,90,"���Ӽ�¼");
    xyprintf(320,90,"��ѯͳ��");
	xyprintf(600,90,"ɾ����¼");
	xyprintf(880,90,"����Ԥ��"); 
	xyprintf(1160,90,"�޸ļ�¼"); 
	xyprintf(880,10,"���ص�½");
	
	int InfoHeight = 40;
	setfont(InfoHeight,0,"����");
	xyprintf(20,170,"���");
	xyprintf(280,170,"��ȡ");
	xyprintf(540,170,"���");
	xyprintf(800,170,"�·�");
	xyprintf(1060,170,"����");
	xyprintf(1320,170,"��ע");
	
	//�ָ��� 
	line(0,160,1440,160);
	
	//rectangle ��x,�ϲ�y,�Ҳ�x,�²�y 
	//ѭ������,ֻ��x1,x2�仯,y�����仯,��x1��x2��+=280 
	int i=40,j=240,count=1;
	for(;count<=5;count++,i+=280,j+=280){
		rectangle(i,90,j,140);
	}

    rectangle(880,10,1080,60);
    
    //��ʾ��Ӧ�ļ���Ϣ 
    char filename[20]="PrivateFinance0.txt";
    showData(filename,20,210,0);
    
    //�����Ϣ 
        mouse_msg msg3;

	for(;is_run();delay_fps(60)){
		
	    bool AddClick = false;//ȷ���Ƿ������Ӽ�¼ 
	    bool SearchClick = false; //ȷ���Ƿ�����ѯͳ�� 
	    bool DeleteClick = false; //ȷ���Ƿ���ɾ����¼
		bool BudgetClick = false; //ȷ���Ƿ�������Ԥ��
		bool ChangeClick = false; //ȷ���Ƿ����޸ļ�¼ 
		bool BackClick = false; //ȷ���Ƿ���������һ������
		 
		
		while(mousemsg()){
			msg3 = getmouse();
			
		if((msg3.x>=40&&msg3.x<=240)&&(msg3.y>=90&&msg3.y<=140)){
					
			if(msg3.is_left()&&msg3.is_down()){
				AddClick = true; 		
			}
			
		}else if((msg3.x>=320&&msg3.x<=520)&&(msg3.y>=90&&msg3.y<=140)){
		
			if(msg3.is_left()&&msg3.is_down()){
				SearchClick = true;
			}
			
		}else if((msg3.x>=600&&msg3.x<=800)&&(msg3.y>=90&&msg3.y<=140)){
					
			if(msg3.is_left()&&msg3.is_down()){
				DeleteClick = true; 
			}
	
		}else if((msg3.x>=880&&msg3.x<=1080)&&(msg3.y>=90&&msg3.y<=140)){
			
			if(msg3.is_left()&&msg3.is_down()){
				BudgetClick = true;
			}
			
		}else if((msg3.x>=1160&&msg3.x<=1360)&&(msg3.y>=90&&msg3.y<=140)){
			
			if(msg3.is_left()&&msg3.is_down()){
				ChangeClick = true;
			}
			
		}
		else if((msg3.x>=880&&msg3.x<=1080)&&(msg3.y>=10&&msg3.y<=60)){
			if(msg3.is_left()&&msg3.is_down()){
				BackClick = true;
			}
		}
	} 
	    if(AddClick){
	   	    
			Add_Message();
			
		}
		if(SearchClick){
			
			Query_Message();
			
		}
		if(BudgetClick){
			
			FinancialBudget();
			
		}
		//ɾ�����޸�Ϊ����ԱȨ��
		 
		 	if(DeleteClick){
		        if(is_Admin){
		   	      Delete_Message();
		        }else{
		           setfont(25,0,"����");
		 	       setcolor(RED);
		 	       xyprintf(240,10,"����ʧ��,δ��⵽����Ա���");
				}
		    
		    }
		    if(ChangeClick){
		    	if(is_Admin){
		        	Change_Message();
				}else{
					setfont(25,0,"����");
		 	       setcolor(RED);
		 	       xyprintf(240,10,"����ʧ��,δ��⵽����Ա���");
				}
		    }
		if(BackClick){
			//���ص�½���� 
			loginMain();
		}
   	} 
}

//�����Ӧ���˿�֧��Ϣ:��ȡ�������Ϣ->�����ļ�->����(�ļ�->�ṹ�����) 
void Add_Message(){
	initgraph(960,480,0);
	setcaption("���Ӽ�¼");
	setbkcolor(WHITE);
	setcolor(BLACK);
	setbkmode(TRANSPARENT);
	
		//���ñ���ͼƬ 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"���Ӽ�¼.jpg",960,480);
	putimage(0,0,img);
	
	int HeadHeight = 50;
	setfont(HeadHeight,0,"����");
	xyprintf(10,10,"���Ӽ�¼"); 
	
	line(0,70,800,70); 
	
	int TextHeight = 40;
	setfont(TextHeight,0,"����");
	
	xyprintf(70,120,"���:");
	xyprintf(70,180,"ʱ��:");
	xyprintf(70,240,"In/Out:");
    xyprintf(70,300,"��;:");
    xyprintf(210,360,"ȷ��"); 
    xyprintf(370,360,"����");
	
	rectangle(210,360,318,400);
	rectangle(370,360,468,400);
	
	mouse_msg msg5;
	
    sys_edit Money,Year,Month,Day,IO,Usage;
	InitField(&Money,false,210,120,25,WHITE,BLACK,10,true,10);
    InitField(&Year,false,210,180,25,WHITE,BLACK,4,true,4);
    InitField(&Month,false,350,180,25,WHITE,BLACK,2,true,2);
    InitField(&Day,false,440,180,25,WHITE,BLACK,2,true,2);
	InitField(&IO,false,210,240,25,WHITE,BLACK,1,true,10);
	InitField(&Usage,false,210,300,25,WHITE,BLACK,20,true,10);
	
	xyprintf(300,180,"��");
	xyprintf(400,180,"��");
	xyprintf(500,180,"��");
	
	//˵��ע������
	setfont(22,0,"����");
	setcolor(RED); 
	xyprintf(465,125,"һ��Ϊ����0���������");
	xyprintf(550,180,"�˴�Ϊyyyy-mm-dd��ʽ");
	xyprintf(550,205,"��Ϊ����0������,�����յ�һλ���鲻Ϊ0");
	xyprintf(465,250,"�˴�Ϊ��дI/O,����IΪ����,OΪ֧��");
	xyprintf(465,310,"�˴�Ϊ20���ַ����ڵ���ĸ/����/�ո�");
	for(;is_run();delay_fps(60)){
		
	    bool CheckClick2 = false;//�Ƿ���ȷ�� 
	    bool ReturnClick2 = false; //�Ƿ������� 

		while(mousemsg()){
			msg5 = getmouse();
			
		if((msg5.x>=210&&msg5.x<=318)&&(msg5.y>=360&&msg5.y<=400)){
					
			if(msg5.is_left()&&msg5.is_down()){
				CheckClick2 = true; 		
			}
			
		}else if((msg5.x>=370&&msg5.x<=468)&&(msg5.y>=360&&msg5.y<=400)){
		
			if(msg5.is_left()&&msg5.is_down()){
				ReturnClick2 = true;
			}
			
		}
	} 
	    if(CheckClick2){
	   	
	   	//�������Finance��Ϣ���뵽��Ӧ�ļ���
	   	 int MoneySize=10,YearSize=5,MonthSize=3,DaySize=3,IOSize=2,UsageSize=20;
	   	char amount[10],year[5],month[3],day[3],io[2],usage[20];
	   	Money.gettext(MoneySize,amount);
	   	Year.gettext(YearSize,year);
	   	Month.gettext(MonthSize,month);
	   	Day.gettext(DaySize,day);
	   	IO.gettext(IOSize,io);
	   	Usage.gettext(UsageSize,usage);

	 
	 char filename[20]="PrivateFinance0.txt";
	 	   	
	   	//ÿ��������붼�Ϸ� 
	   	if(RegexMatch(MoneyRS,amount)&&RegexMatch(IORS,io)&&RegexMatch(YearRS,year)&&RegexMatch(MonthRS,month)&&RegexMatch(DayRS,day)&&RegexMatch(UsageRS,usage)){
	   		   //�����������Ӧ�ļ�����
	   		   //�ļ�->�ṹ��;�ṹ������;�ṹ��->�ļ� 
	   		   CopyData(filename,0);
	   		   //addInfo����,��������ȡ����Ϣȫ�����뵽ָ���ļ��� 
			   addInfo(filename,amount,io,year,month,day,usage);
			   
			   UserInfo[0].Rec_len++;//�Լ�¼���ȼ�1 
			   CopyData(filename,0);
			   DateSort(UserInfo[0].List_Finance,UserInfo[0].Rec_len);
			   Synchronize(filename,UserInfo[0]);
			   
			   outtextxy(210,410,"��ӳɹ�!!");
	    }else{
	    	
	    	outtextxy(210,410,"���ʧ��,��ʽ�������");
	    	
		}
		    //�����Կ�����Ϣ����ɾ�� 
			Money.settext("");
	    	Year.settext("");
	    	Month.settext("");
	    	Day.settext("");
	    	IO.settext("");
			Usage.settext("");
	}
		if(ReturnClick2){
			
		cleardevice();
		Money.destory();
		Year.destory();
		Month.destory();
		Day.destory();
        IO.destory();
        Usage.destory();
        Sleep(100);
		MainMenu();
		
		}	
   }
	
}

//ɾ����Ӧ��֧��Ϣ(����ԱȨ��)
void Delete_Message(){
	initgraph(800,480,0);
	setcaption("ɾ����¼");
	setbkcolor(WHITE);
	setcolor(BLACK);
	setbkmode(TRANSPARENT);
	
	//���ñ���ͼƬ 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"ɾ����¼.jpg",800,480);
	putimage(0,0,img);
	
	int HeadHeight = 50;
	setfont(HeadHeight,0,"����");
	xyprintf(10,0,"ɾ����¼"); 
	
	line(0,360,960,360); 
	
	int TextHeight = 40;
	setfont(TextHeight,0,"����");
	xyprintf(30,370,"���");
	xyprintf(190,370,"��ȡ");
	xyprintf(350,370,"ʱ��");
	xyprintf(510,370,"��ע");
	
	setfont(40,0,"����"); 
	xyprintf(270,185,"��");
	xyprintf(360,185,"��");
	xyprintf(440,185,"��");
	
	xyprintf(70,60,"���:");
	xyprintf(70,120,"��ȡ:");
	xyprintf(70,180,"ʱ��:");
	xyprintf(70,240,"��ע:");
	xyprintf(60,300,"��ѯ");
	xyprintf(220,300,"ȷ��ɾ��");
	xyprintf(480,300,"����");
	
	rectangle(60,300,160,340);
	rectangle(220,300,390,340);
	rectangle(480,300,580,340);
	
	//����������Ϣ  
	setfont(20,0,"����");
	setcolor(RED);
	xyprintf(475,60,"���Ȱ���ʽ������Ϣ");
	xyprintf(475,80,"�ٵ����ѯ��ť");
	xyprintf(475,120,"����Ӧ��Ϣ������");
	xyprintf(475,140,"���·������ѯʧ��");
	xyprintf(475,180,"����Ӧ��Ϣƥ��");
	xyprintf(475,200,"�����ѯ��ť,�·���ʾ��Ӧ��Ϣ");
	xyprintf(475,220,"��ʱ�ٴε��ȷ��ɾ������ɾ���ɹ�");
	xyprintf(475,240,"���:10λ��������,��/ȡ:I/O");
	xyprintf(475,260,"ʱ��:yyyy-mm-dd"); 
	xyprintf(475,280,"��ע:20λ��������/��ĸ/�ո�");
	
	//�����Ϣ 
	mouse_msg msg6;
	
	//������ʼ��
	sys_edit DMoney,DIO,DYear,DMonth,DDay,DUsage; //���,��֧,ʱ��,��ע
	InitField(&DMoney,false,170,65,25,WHITE,BLACK,10,true,10); 
	InitField(&DIO,false,170,125,25,WHITE,BLACK,5,true,10);
     InitField(&DYear,false,170,185,25,WHITE,BLACK,4,true,4);
     InitField(&DMonth,false,310,185,25,WHITE,BLACK,2,true,2);
     InitField(&DDay,false,390,185,25,WHITE,BLACK,2,true,2);
	InitField(&DUsage,false,170,245,25,WHITE,BLACK,20,true,10); 
	
	//��ѯ,��ʾ��Ϣ,ȷ��ɾ��
	//1.������ȡ��Ϣ,���ļ�����Ϣ�����ȶ�
	//����ѯ��,����ʾ������Ŀ�; 
	//��δ��ѯ��,���·���ʾδ�ҵ��˼�¼ 
	//2.��ѯ����Ϣ,�����ȷ��ɾ��,���ȶԽṹ����нڵ�ɾ������
	//�ٽ��ṹ������ͬ�����ļ���
	 
	//����д���ļ�����ֵ 
	   	char PFF[20]="PrivateFinance0.txt",DQF[20]="DeleteQuery.txt";
	
		for(;is_run();delay_fps(60)){
			
		bool Query = false; //�Ƿ�����ѯ 
	    bool CheckDel = false;//�Ƿ���ȷ��ɾ�� 
	    bool Return = false; //�Ƿ������� 

		while(mousemsg()){
			msg6 = getmouse();
			
		if((msg6.x>=60&&msg6.x<=160)&&(msg6.y>=300&&msg6.y<=340)){
					
			if(msg6.is_left()&&msg6.is_down()){
				Query = true; 		
			}
			
		}else if((msg6.x>=220&&msg6.x<=390)&&(msg6.y>=300&&msg6.y<=340)){
		
			if(msg6.is_left()&&msg6.is_down()){
				CheckDel = true;
			}
			
		}else if((msg6.x>=480&&msg6.x<=580)&&(msg6.y>=300&&msg6.y<=340)){
			
			if(msg6.is_left()&&msg6.is_down()){
				Return = true;
			}
			
		} 
	} 
	    
	    if(Query){
	    //�ַ��������������Ϣ	
	    int DMoneySize=10,DISize=2,DYSize=5,DMonthSize=3,DDSize=3,DUSize=20;
	   	char DAmount[20],DI[2],DY[5],DM[3],DD[3],DU[20];
	    
	    DMoney.gettext(DMoneySize,DAmount);
	    DIO.gettext(DISize,DI);
	    DYear.gettext(DYSize,DY);
	    DMonth.gettext(DMonthSize,DM);
	    DDay.gettext(DDSize,DD);
	    DUsage.gettext(DUSize,DU);
	    
	    
	   	if(RegexMatch(MoneyRS,DAmount)&&RegexMatch(IORS,DI)&&
		   RegexMatch(YearRS,DY)&&RegexMatch(MonthRS,DM)&&RegexMatch(UsageRS,DU)
		   &&CheckDeleteQuery(PFF,DQF,DAmount,DI,DY,DM,DD,DU)){
	   		xyprintf(30,420,"%s->%s>%s->%s->%s->%s",DAmount,DI,DY,DM,DD,DU);
		}else{
			setfont(25,0,"����"); 
			xyprintf(30,420,"��ѯʧ��,��ʽ����/δ��ѯ����Ӧ��¼");
		}
		DMoney.settext("");
		DIO.settext("");
		DYear.settext("");
		DMonth.settext("");
		DDay.settext("");
		DUsage.settext("");
	 }
	    //���ȷ��ɾ�� 
		if(CheckDel){
			DeleteInfo(DQF,PFF,UserInfo[0],0);
		}
		
		//�������	
		if(Return){
			cleardevice();
			DMoney.destory();
			DIO.destory();
			DYear.destory();
			DMonth.destory();
			DDay.destory();
		    DUsage.destory();
		    Sleep(100);
			MainMenu();			
		}
   }
	
}

//�޸���Ӧ��֧��Ϣ(����ԱȨ��) 
void Change_Message(){
    initgraph(1040,720,0);
	setcaption("�޸ļ�¼");
	setbkcolor(WHITE);
	setcolor(BLACK);
	setbkmode(TRANSPARENT);
	
	//�����湦����Ҫ��ȡ���ļ���
	char filename[20]="PrivateFinance0.txt"; 
	
		//���ñ���ͼƬ 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"�޸ļ�¼.jpg",1040,720);
	putimage(0,0,img);
	
	int InfoHeight = 25;
	setfont(InfoHeight,0,"����");
	setcolor(RED);
	xyprintf(660,100,"Before������Ҫ�Ϳ�֧��¼һһ��Ӧ");
	xyprintf(660,140,"�������������������Ϣ��");
	xyprintf(660,180,"���ȷ���޸İ�ť");
	xyprintf(660,220,"�޸ĳ�,�ɵ��������������У��");
	
	int HeadHeight = 35;
	setfont(HeadHeight,0,"����");
	setcolor(BLACK);
	xyprintf(20,15,"�޸ļ�¼");
	xyprintf(20,95,"���(Before):");
	xyprintf(20,145,"��ȡ(Before):");
	xyprintf(20,205,"ʱ��(Before):");
	xyprintf(400,205,"��"); 
	xyprintf(520,205,"��");
	xyprintf(630,205,"��");
	xyprintf(20,260,"��ע(Before):");
	line(0,320,1040,320);
	xyprintf(20,335,"���(After-):");
	xyprintf(20,415,"��ȡ(After-):");
	xyprintf(20,495,"ʱ��(After-):");
	xyprintf(400,505,"��");
	xyprintf(520,505,"��");
	xyprintf(630,505,"��");
	xyprintf(20,575,"��ע(After-):");
	
	xyprintf(20,655,"ȷ���޸�");
	xyprintf(380,655,"����");
	
	rectangle(20,655,260,705);
	rectangle(380,655,500,705);
	
	//�������ϢA(After),B(Before)
	sys_edit BMoney,BIO,BYear,BMonth,BDay,BUsage,AMoney,AIO,AYear,AMonth,ADay,AUsage;
	InitField(&BMoney,false,280,95,30,WHITE,BLACK,10,true,10);
	InitField(&BIO,false,280,155,30,WHITE,BLACK,5,true,10);
    InitField(&BYear,false,280,205,30,WHITE,BLACK,4,true,4);
    InitField(&BMonth,false,460,205,30,WHITE,BLACK,2,true,2);
    InitField(&BDay,false,570,205,30,WHITE,BLACK,2,true,2);
    InitField(&BUsage,false,280,260,30,WHITE,BLACK,20,true,10); 
    
	InitField(&AMoney,false,280,345,30,WHITE,BLACK,10,true,10);
	InitField(&AIO,false,280,425,30,WHITE,BLACK,5,true,10);
    InitField(&AYear,false,280,505,30,WHITE,BLACK,4,true,4);
    InitField(&AMonth,false,460,505,30,WHITE,BLACK,2,true,2);
    InitField(&ADay,false,570,505,30,WHITE,BLACK,2,true,2);
    InitField(&AUsage,false,280,585,30,WHITE,BLACK,20,true,10);
	
	//�����Ϣ 
	mouse_msg msg8; 
	
	for(;is_run();delay_fps(60)){
			
	    bool CheckCh = false;//�Ƿ���ȷ���޸� 
	    bool Return = false; //�Ƿ������� 

		while(mousemsg()){
			msg8 = getmouse();
			
		if((msg8.x>=20&&msg8.x<=260)&&(msg8.y>=655&&msg8.y<=705)){
					
			if(msg8.is_left()&&msg8.is_down()){
				CheckCh = true; 		
			}
			
		}else if((msg8.x>=380&&msg8.x<=500)&&(msg8.y>=655&&msg8.y<=705)){
		
			if(msg8.is_left()&&msg8.is_down()){
				Return = true;
			}
			
		  }
	   } 
	    if(CheckCh){
	    	
	   	 int MoneySize=10,YearSize=5,MonthSize=3,DaySize=3,IOSize=2,UsageSize=20;
	   	char BAm[10],BYe[5],BMo[3],BDa[3],BIo[2],BUs[20],AAm[10],AYe[5],AMo[3],ADa[3],AIo[2],AUs[20];
	   	//sys_edit BMoney,BIO,BYear,BMonth,BDay,BUsage,AMoney,AIO,AYear,AMonth,ADay,AUsage;
	   	BMoney.gettext(MoneySize,BAm);
	   	BYear.gettext(YearSize,BYe);
	   	BMonth.gettext(MonthSize,BMo);
	   	BDay.gettext(DaySize,BDa);
	   	BIO.gettext(IOSize,BIo);
	   	BUsage.gettext(UsageSize,BUs);
	   	
	   	AMoney.gettext(MoneySize,AAm);
	   	AYear.gettext(YearSize,AYe);
	   	AMonth.gettext(MonthSize,AMo);
	   	ADay.gettext(DaySize,ADa);
	   	AIO.gettext(IOSize,AIo);
	   	AUsage.gettext(UsageSize,AUs);
		   
		   //if�ж϶��޸�ǰ��Ϣ��������ƥ�� 
		  if(RegexMatch(MoneyRS,BAm)&&RegexMatch(YearRS,BYe)&&RegexMatch(MonthRS,BMo)
		  &&RegexMatch(DayRS,BDa)&&RegexMatch(IORS,BIo)&&RegexMatch(UsageRS,BUs)){
		  	//ChangeInfo���޸ĺ���Ϣ�����������ƥ�� 
		  	ChangeInfo(filename,UserInfo[0],0,BAm,BIo,BYe,BMo,BDa,BUs,AAm,AIo,AYe,AMo,ADa,AUs);
			xyprintf(700,180,"�ɹ�,���ز鿴���");
		  }else{
			xyprintf(700,180,"ʧ��,�޸�ǰ�����ʽ����");
		  }   	
	   	 
	   	 BMoney.settext("");
	   	 BIO.settext("");
	   	 BYear.settext("");
	   	 BMonth.settext("");
	   	 BDay.settext("");
	   	 BUsage.settext("");
	   	 AMoney.settext("");
	   	 AIO.settext("");
	   	 AYear.settext("");
	   	 AMonth.settext("");
	   	 ADay.settext("");
	   	 AUsage.settext("");
			   
		}
		if(Return){
			
	    cleardevice();
	    BMoney.destory();
	   	BIO.destory();
        BYear.destory();
        BMonth.destory();
        BDay.destory();
        BUsage.destory();
	   	AMoney.destory();
	   	AIO.destory();
        AYear.destory();
        AMonth.destory();
        ADay.destory();
	   	AUsage.destory();
	   	Sleep(100);
		MainMenu();
		
		}	
		
   }
} 

//��ѯ��Ӧ���˿�֧��Ϣ 
void Query_Message(){
	
	initgraph(960,640,0);
	setcaption("��ѯͳ��");
	setbkcolor(WHITE);
	setcolor(BLACK);
	setbkmode(TRANSPARENT);
	
		//���ñ���ͼƬ 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"��ѯͳ��.jpg",960,640);
	putimage(0,0,img);
	
	int HeadHeight = 50;
	setfont(HeadHeight,0,"����");
	xyprintf(20,20,"��ѯͳ��");
	
	 
	//��ȡ�ļ��� 
	char filename[20]="PrivateFinance0.txt";
	//�ļ�����ͬ��
	 CopyData(filename,0);	
	//ͳ�����������and����� 
	TotalInOut(UserInfo[0],0);
	//��floatתΪstring����
	//��ָ��λ����� 
    char In[15],Out[15],totalMon[15];
    setfont(22,0,"��Բ"); 
    
	sprintf(In,"%.0f",Income[0]); 
	strcat(In,"->TotalIncome"); 
	outtextxy(240,20,In);
	
    sprintf(Out,"%.0f",Outcome[0]);
	strcat(Out,"->TotalOutcome");
	outtextxy(490,20,Out);
	
    sprintf(totalMon,"%.0f",Total[0]);
    strcat(totalMon,"->Total");
	outtextxy(800,20,totalMon);

	int TextHeight = 50;
	setfont(TextHeight,0,"����");
	xyprintf(20,90,"ʱ��:");
	xyprintf(400,90,"��");
	xyprintf(580,90,"��");
	xyprintf(760,90,"��");
	xyprintf(240,160,"��ѯ");
	xyprintf(600,160,"����");
	
	rectangle(240,160,360,210);
	rectangle(600,160,720,210);
	
	
	//���ָ��� 
	line(0,240,960,240); 
	
	int InfoHeight = 30;
	setfont(InfoHeight,0,"����");
	
	//������ʾ˵�� 
	setfont(20,0,"����");
	setcolor(RED);
	xyprintf(10,245,"����˵��");
	xyprintf(130,265,"���Ϸ�ͳ�Ƶ�������ʱ�����֧״��");
	xyprintf(130,285,"�����������ں�,�ٵ����ѯ��ť,ϵͳ���Զ���������������·�");
	xyprintf(130,305,"�����ʽ����:2014 4 3,2014 4,ǰ��ͳ�Ƶ���,����ͳ��һ����(״��)");
	xyprintf(130,325,"�����о�������,��ͳ����Ч");
	
	//���������
	sys_edit Year,Month,Day;
	InitField(&Year,false,150,90,TextHeight,WHITE,BLACK,4,true,4); 
	InitField(&Month,false,460,90,TextHeight,WHITE,BLACK,2,true,2);
	InitField(&Day,false,640,90,TextHeight,WHITE,BLACK,2,true,2);
	
	//���������Ϣ 
	mouse_msg msg8;
			
		for(;is_run();delay_fps(60)){
		
	    bool Query = false;//�Ƿ�������Ԥ�� 
	    bool ReturnMain = false; //�Ƿ������� 

		
		while(mousemsg()){
			msg8 = getmouse();
			
		if((msg8.x>=240&&msg8.x<=360)&&(msg8.y>=160&&msg8.y<=210)){
					
			if(msg8.is_left()&&msg8.is_down()){
				Query = true; 		
			}
			
		}else if((msg8.x>=600&&msg8.x<=720)&&(msg8.y>=160&&msg8.y<=210)){
		
			if(msg8.is_left()&&msg8.is_down()){
				ReturnMain = true;
			}
			
		}
	} 
	    if(Query){ 
		  //��ȡ�������Ϣ,��Ϊ�ַ���
		int QYear=5,QMonth=3,QDay=3;
	   	char QY[5],QM[3],QD[3];
	   	Year.gettext(QYear,QY);
	   	Month.gettext(QMonth,QM);
	   	Day.gettext(QDay,QD);
	   	
	   		 //���ַ���ת��Ϊ��Ӧ������,���û������,������"",�򽫶�Ӧ����������Ϊ0
		  int TransNum[3];//�ֱ��Ӧ������
		  char Sym[2]=""; //���ַ� 
		  //�� 
		  if(strcmp(QY,Sym)==0){
		  	 TransNum[0]=0;
		  }else{
		  	 TransNum[0]=atoi(QY);
		  }  
		  //�� 
		  if(strcmp(QM,Sym)==0){
		  	 TransNum[1]=0;
		  }else{
		  	 TransNum[1]=atoi(QM);
		  }
		  //��
		  if(strcmp(QD,Sym)==0){
		  	TransNum[2]=0;
		  }else{
		  	TransNum[2]=atoi(QD);
		  } 
		  //�Ѷ�Ӧ�Ĳ�������Statistics(....)�����н���ȫ�����鸳ֵ;
		  Statistics(filename,UserInfo[0],0,TransNum[0],TransNum[1],TransNum[2]);
          
          //��Ӧλ�������������
          char TI[15],TO[15],TT[15];
         sprintf(TI,"%.0f",StatisticIncome[0]);
         sprintf(TO,"%.0f",StatisticOutcome[0]);
         sprintf(TT,"%.0f",StatisticTotal[0]);

         //������������Լ�λ�� 
         setfont(40,0,"����");
          xyprintf(100,480,"AreaIncome:");
		  xyprintf(400,480,TI);
		  xyprintf(100,520,"AreaOutcome:");
		  xyprintf(400,520,TO);
		  xyprintf(100,560,"AreaBalance:"); 
          xyprintf(400,560,TT);
          //ȫ�ֱ�����ֵ��ʾ���ʼ�� 
          StatisticIncome[0]=0;
          StatisticOutcome[0]=0;
          StatisticTotal[0]=0;
          
          //�������� 
	   	  Year.settext("");
	   	  Month.settext("");
	   	  Day.settext("");
			
		}
		if(ReturnMain){
		 
		 cleardevice();
		 Year.destory();
		 Month.destory();
		 Day.destory();
		 Sleep(100);
		 MainMenu();
			
		}	
   }
}

//����Ԥ����湦�����
void FinancialBudget(){
	
	initgraph(800,480,0);
	setcaption("����Ԥ��");
	setbkcolor(WHITE);
	setcolor(BLACK);
	setbkmode(TRANSPARENT);
	
	//���ñ���ͼƬ 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"����Ԥ��.jpg",800,480);
	putimage(0,0,img);
	
	int HeadHeight = 50;
	setfont(HeadHeight,0,"����");
	xyprintf(10,10,"����Ԥ��"); 
	
	line(0,70,800,70); 
	
	int TextHeight = 30;
	setfont(TextHeight,0,"����");

	xyprintf(70,130,"����Ŀ��");
	xyprintf(70,170,"��֧");
	xyprintf(70,210,"�̶���֧");
    xyprintf(70,250,"���ɿ�֧");
    xyprintf(70,290,"������  ");
    xyprintf(130,360,"����Ԥ��"); 
    xyprintf(370,360,"����");
	
	//��ʾ��Ϣ
	setfont(20,0,"����");
	setcolor(RED);
	xyprintf(480,130,"�밴��Ӧ��֧���������Ӧ���");
	xyprintf(480,160,"��д��Ϻ�������Ԥ�㰴ť"); 
	xyprintf(480,190,"ϵͳ���Զ��������Ӧ�Բ�����");
	xyprintf(480,220,"��д��ΧΪ(0~9999999999)");
	//��ť�� 
	setcolor(BLACK);
	rectangle(130,360,290,400);
	rectangle(370,360,468,400);
	
	    //�����
		sys_edit Tag,Flex,Fixed,Free,PureIn; //����Ŀ��,��֧,�̶���֧,���ɿ�֧,������ 
		InitField(&Tag,false,200,130,25,WHITE,BLACK,10,true,10);
		InitField(&Flex,false,200,170,25,WHITE,BLACK,10,true,10);
		InitField(&Fixed,false,200,210,25,WHITE,BLACK,10,true,10);
		InitField(&Free,false,200,250,25,WHITE,BLACK,10,true,10);
		InitField(&PureIn,false,200,290,25,WHITE,BLACK,10,true,10);
		
		//��ȡ�ļ�����
		char filename[25]="FinancialBudget.txt"; 
		//���������Ϣ 
			mouse_msg msg7;
			
		for(;is_run();delay_fps(60)){
		
	    bool Budget = false;//�Ƿ�������Ԥ�� 
	    bool Return = false; //�Ƿ������� 

		while(mousemsg()){
			msg7 = getmouse();
			
		if((msg7.x>=130&&msg7.x<=290)&&(msg7.y>=360&&msg7.y<=400)){
					
			if(msg7.is_left()&&msg7.is_down()){
				Budget = true; 		
			}
			
		}else if((msg7.x>=370&&msg7.x<=468)&&(msg7.y>=360&&msg7.y<=400)){
		
			if(msg7.is_left()&&msg7.is_down()){
				Return = true;
			}
			
		}
	} 
	    if(Budget){
	   	
	   	//��ȡ�������Ϣ,��Ϊ�ַ���
		int BudSize = 10;
		//Tag,Flex,Fixed,Free,PureIn; ����Ŀ��,��֧,�̶���֧,���ɿ�֧,������ 
	   	char BTag[10],BFlex[10],BFixed[10],BFree[10],BPureIn[10]; 

	   	Tag.gettext(BudSize,BTag);
	   	Flex.gettext(BudSize,BFlex);
	   	Fixed.gettext(BudSize,BFixed);
	   	Free.gettext(BudSize,BFree);
	   	PureIn.gettext(BudSize,BPureIn);
	    //�ж������Ƿ�Ϸ�,�Ϸ�����к������ò���
		  if(RegexMatch(BTS,BTag)&&RegexMatch(BTS,BFlex)&&
		  RegexMatch(BTS,BFixed)&&RegexMatch(BTS,BFree)&&RegexMatch(BTS,BPureIn)){
		  	 //�ڶ�Ӧ�������Ԥ����
			 BudgetOP(filename,BTag,BFlex,BFixed,BFree,BPureIn); 
		  	 xyprintf(300,50,"Ԥ��ɹ���");
		  }else{
		  	 xyprintf(300,50,"�����ʽ����!");
		  }
		  
		  //�������� 
		  Tag.settext("");
		  Flex.settext("");
		  Fixed.settext("");
		  Free.settext("");
		  PureIn.settext(""); 
	   	
		}
		if(Return){
			
		 cleardevice();
		 Tag.destory();
		 Flex.destory();
		 Fixed.destory();
		 Free.destory();
		 PureIn.destory();
		 Sleep(100);
		 MainMenu();
			
		}	
   }
}  

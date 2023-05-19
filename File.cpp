#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <string> 
#include "Struct.h"
#include "File.h"
#include "OP.h"
#include "View.h"
using namespace std;
   extern float Total[Umax]; //�ִ��ܽ�� 
   extern float Income[Umax]; //������ 
   extern float Outcome[Umax]; //��֧��  
   extern Users UserInfo[Umax]; //�û���������Ϣ
   extern float StatisticIncome[Umax]; //ͳ�ƽ�������
   extern float StatisticOutcome[Umax]; //ͳ�ƽ���֧��
   extern float StatisticTotal[Umax]; //ͳ�ƽ����ִ���
   extern string  MoneyRS;
	extern	string  IORS;
	extern	string  YearRS;//����ҿ�
	extern	string  MonthRS;//����ҿ�
	extern	string  DayRS;
	extern	string  UsageRS; 

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



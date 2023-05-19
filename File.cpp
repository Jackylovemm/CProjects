#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <string> 
#include "Struct.h"
#include "File.h"
#include "OP.h"
#include "View.h"
using namespace std;
   extern float Total[Umax]; //现存总金额 
   extern float Income[Umax]; //纯收入 
   extern float Outcome[Umax]; //纯支出  
   extern Users UserInfo[Umax]; //用户的所有信息
   extern float StatisticIncome[Umax]; //统计界面收入
   extern float StatisticOutcome[Umax]; //统计界面支出
   extern float StatisticTotal[Umax]; //统计界面现存金额
   extern string  MoneyRS;
	extern	string  IORS;
	extern	string  YearRS;//左闭右开
	extern	string  MonthRS;//左闭右开
	extern	string  DayRS;
	extern	string  UsageRS; 

//修改密码函数 
//步骤:先进行修改->TestCode覆盖NewTestCode->NewTestCode覆盖TestCode 
bool change(char* OriID,char* OriPass,char* NewPass){
	
	if(strcmp(OriPass,NewPass)==0){
	   return false;
	}
	
	//1.原密码和ID确认
	FILE *fp,*fp1,*fp2,*fp3,*fp4;
	fp = fopen("TestCode.txt","r");
	fp1 = fopen("NewTestCode.txt","w+");
	fp2 = fopen("TestCode.txt","r+");
    //找到需要修改密码的哪一行 实现 
	int line=0;
	//确认是否找到 
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
	
	//最后需要改密码的行数 
    if(isFound){
      	char str[100]="",str1[100]="";
	int i=0;
	while(!feof(fp2)){
		i++;
		//比较行数是否相等
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

//校验登陆 
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
	
    //关闭文件流 
	fclose(fp);
	return Check; 
} 

//校验注册功能 
bool Register(char* RId,char* RPass){
	//a+,在原有文件基础上添加内容 
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

//相应开支统计,若相应year,month,day未输入,则自动设置为0;
void Statistics(char* filename,Users user,int UserNum,int year,int month,int day){
	
	int count=0; 
	
	//统计前把文件中信息拷贝至结构体中
	CopyData(filename,UserNum);
	
	int TempY,TempM,TempD;
	float TempAmount=0;
    char TempIO[5];
    
	//把七种情况均考虑到
    if((year!=0)&&(month!=0)&&(day!=0)){
    	//1.年月日均有输入且合法 
        for(;count<user.Rec_len;count++){
        	TempY=user.List_Finance[count].year;
        	TempM=user.List_Finance[count].month;
        	TempD=user.List_Finance[count].day;
        	TempAmount=user.List_Finance[count].amount;
        	strcpy(TempIO,user.List_Finance[count].InOrOut);
        	
        	//如果找到了对应的年月日 
        	if((year==TempY)&&(month==TempM)&&(day==TempD)){
        		if(strcmp(TempIO,"I")==0){
        		   StatisticIncome[UserNum]+=TempAmount;
			    }else if(strcmp(TempIO,"O")==0){
				   StatisticOutcome[UserNum]+=TempAmount;
			    }
			}
        	
		}
    	
	}else if((year!=0)&&(month!=0)&&(day==0)){
		//2.输入年和月,不输入日
		for(;count<user.Rec_len;count++){
        	TempY=user.List_Finance[count].year;
        	TempM=user.List_Finance[count].month;
        	TempAmount=user.List_Finance[count].amount;
        	strcpy(TempIO,user.List_Finance[count].InOrOut);
        	
        	//如果找到了对应的年月 
        	if((year==TempY)&&(month==TempM)){
        		if(strcmp(TempIO,"I")==0){
        		   StatisticIncome[UserNum]+=TempAmount;
			    }else if(strcmp(TempIO,"O")==0){
				   StatisticOutcome[UserNum]+=TempAmount;
			    }
			}
		} 
		 
	}else if((year!=0)&&(month==0)&&(day!=0)){
		//3.输入年和日,不输入月
		  for(;count<user.Rec_len;count++){
        	TempY=user.List_Finance[count].year;
        	TempD=user.List_Finance[count].day;
        	TempAmount=user.List_Finance[count].amount;
        	strcpy(TempIO,user.List_Finance[count].InOrOut);
        	
        	//如果找到了对应的年和日 
        	if((year==TempY)&&(day==TempD)){
        		if(strcmp(TempIO,"I")==0){
        		   StatisticIncome[UserNum]+=TempAmount;
			    }else if(strcmp(TempIO,"O")==0){
				   StatisticOutcome[UserNum]+=TempAmount;
			    }
			}
        	
		}
		 
	}else if((year==0)&&(month!=0)&&(day!=0)){
		//4.输入月和日,不输入年份
		  for(;count<user.Rec_len;count++){
        	TempM=user.List_Finance[count].month;
        	TempD=user.List_Finance[count].day;
        	TempAmount=user.List_Finance[count].amount;
        	strcpy(TempIO,user.List_Finance[count].InOrOut);
        	
        	//如果找到了对应的月和日 
        	if((month==TempM)&&(day==TempD)){
        		if(strcmp(TempIO,"I")==0){
        		   StatisticIncome[UserNum]+=TempAmount;
			    }else if(strcmp(TempIO,"O")==0){
				   StatisticOutcome[UserNum]+=TempAmount;
			    }
			}
        	
		}
	}else if((year!=0)&&(month==0)&&(day==0)){
		//5.只输入年,不输入月和日 
		 for(;count<user.Rec_len;count++){
        	TempY=user.List_Finance[count].year;
        	TempAmount=user.List_Finance[count].amount;
        	strcpy(TempIO,user.List_Finance[count].InOrOut);
        	
        	//如果找到了对应的年份 
        	if((year==TempY)){
        		if(strcmp(TempIO,"I")==0){
        		   StatisticIncome[UserNum]+=TempAmount;
			    }else if(strcmp(TempIO,"O")==0){
				   StatisticOutcome[UserNum]+=TempAmount;
			    }
			}
        	
		}
	}else if((year==0)&&(month!=0)&&(day==0)){
		//6.只输入月,不输入年和日 
		 for(;count<user.Rec_len;count++){
        	TempM=user.List_Finance[count].month;
        	TempAmount=user.List_Finance[count].amount;
        	strcpy(TempIO,user.List_Finance[count].InOrOut);
        	
        	//如果找到了对应的月 
        	if((month==TempM)){
        		if(strcmp(TempIO,"I")==0){
        		   StatisticIncome[UserNum]+=TempAmount;
			    }else if(strcmp(TempIO,"O")==0){
				   StatisticOutcome[UserNum]+=TempAmount;
			    }
			}
        	
		}
		
	}else if((year==0)&&(month==0)&&(day!=0)){
		//7.只输入日,不输入年和月 
		for(;count<user.Rec_len;count++){
        	TempD=user.List_Finance[count].day;
        	TempAmount=user.List_Finance[count].amount;
        	strcpy(TempIO,user.List_Finance[count].InOrOut);
        	
        	//如果找到了对应的日 
        	if((day==TempD)){
        		if(strcmp(TempIO,"I")==0){
        		   StatisticIncome[UserNum]+=TempAmount;
			    }else if(strcmp(TempIO,"O")==0){
				   StatisticOutcome[UserNum]+=TempAmount;
			    }
			}
        	
		}
	}
	//统计余额 
	StatisticTotal[UserNum]=StatisticIncome[UserNum]-StatisticOutcome[UserNum];
}

//往对应的文件中添加个人Finance信息 
void addInfo(char* filename,char* money,char* io,char* year,char* month,char* day,char* usage){
		//a+,在原有文件基础上添加内容 
	FILE *fp = fopen(filename,"a+");
	
	if(fp==NULL){
		return ;
	}
    fprintf(fp,"\n%s %s %s %s %s %s",money,io,year,month,day,usage);
    
	fclose(fp);
} 

//结构体->文件
void Synchronize(char* filename,Users user){
		
    FILE *fp = fopen(filename,"w+");
    
    int line=0;
    //从文件中读出的信息都是字符数组类型的 
    //itoa(num, str, 10); 可以把对应的数字转化为字符串,第三个数字是基数(n进制);
	//函数 atof() 用于将字符串转换为双精度浮点数(double)，
	//其原型为：
    //double atof (const char* str);
	//赋值操作 
	
	while(line<user.Rec_len){
		char FileInfo[100]="",Year[4]="",Month[2]="",Day[2]="";
		char *amount;
		
		//每个属性变成char类型,然后赋值
		//int->string 
		sprintf(Year,"%d",user.List_Finance[line].year);
		sprintf(Month,"%d",user.List_Finance[line].month);
		sprintf(Day,"%d",user.List_Finance[line].day);
		//float->string
		 int sign,dec_pl;
         amount = fcvt(user.List_Finance[line].amount,0,&dec_pl,&sign);
		//将一整个结构体的信息拼成一个格式字符串 
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
		
			line++; //对计数器加1 
	
	} 
	fclose(fp);	
} 


//文件->结构体 
void CopyData(char* filename,int UserNum){
	
	//找到指定文件,将文件内的信息展示出来并存储至相应结构体中 
    FILE *fp = fopen(filename,"r");
    
    //初始化结构体
	InitStruct(&UserInfo[UserNum]); 
    
    int line=0;
    //从文件中读出的信息都是字符数组类型的 
	//atoi(char *str)可以把对应的str转换为数字 用int接收 
    //itoa(num, str, 10); 可以把对应的数字转化为字符串,第三个数字是基数(n进制);
	//函数 atof() 用于将字符串转换为双精度浮点数(double)，
	//其原型为：
    //double atof (const char* str);
	//赋值操作 
	char money[10],IO[5],Year[5],Month[2],Day[2],Usage[20];
	while(!feof(fp)&&line<count(filename)){
		
		fscanf(fp,"%s%s%s%s%s%s",money,IO,Year,Month,Day,Usage);
		UserInfo[UserNum].List_Finance[line].amount=atof(money);
        strcpy(UserInfo[UserNum].List_Finance[line].InOrOut,IO);
		UserInfo[UserNum].List_Finance[line].year=atoi(Year);
		UserInfo[UserNum].List_Finance[line].month=atoi(Month);
		UserInfo[UserNum].List_Finance[line].day=atoi(Day);
	    strcpy(UserInfo[UserNum].List_Finance[line].Usage,Usage);
		UserInfo[UserNum].Rec_len++;//读进一条数据,长度加1 
		line++;//行数自加 
	} 
	fclose(fp);	
}

//文件行数计数器 
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

//校验删除查询结果并存入对应文件中,若查询到,则写入对应文件,若未查询到,不进行文件操作,直接关闭读入流 
bool CheckDeleteQuery(char* ReadFilename,char* WriteFilename,char* Amount,char* IO,char* Year,char* Month,char* Day,char* Usage){
	//读取相应文件中的内容,打开方式r 
	FILE *fp = fopen(ReadFilename,"r");
	FILE *fp1 = fopen(WriteFilename,"w+");
	char FileA[10]={""},FileI[2]={""},FileY[5]={""},FileM[3]={""},FileD[3]={""},FileU[20]={""},Rec[100]={""};
	
	 bool DQuery=false;
	while(!feof(fp)){
		fscanf(fp,"%s%s%s%s%s%s",FileA,FileI,FileY,FileM,FileD,FileU);
		//文件信息均匹配,连接字符串存储,再写入对应文件 
		if(strcmp(Amount,FileA)==0&&strcmp(IO,FileI)==0&&strcmp(Year,FileY)==0&&strcmp(Month,FileM)==0&&strcmp(Day,FileD)==0&&strcmp(Usage,FileU)==0){
			//现在信息存入Rec字符串中
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
			//将Rec字符串写入 
		    fprintf(fp1,"%s",Rec);
			
			DQuery=true;
			break;
		}
	}

    //关闭文件读入流 
	fclose(fp);
	fclose(fp1);
	return DQuery; 
}

//删除信息的操作函数 出现错误,删除失败 
void DeleteInfo(char* RFName,char* WFName,Users user,int UserNum){
	
	FILE *fp = fopen(RFName,"r");
	
	int index; //记录对应下标
	bool FindCheck=false; //标志位 
	char money[10],IO[5],Year[5],Month[2],Day[2],Usage[20];
	float amount;
	int year,month,day;
	char io[2],usage[20];
	//只有一条信息,所以直接读取
	    
		 
	    while(!feof(fp)){
	    	fscanf(fp,"%s%s%s%s%s%s",money,IO,Year,Month,Day,Usage);
		}
		//方便比较,直接赋值 
		amount=atof(money);
        strcpy(io,IO);
		year=atoi(Year);
		month=atoi(Month);
	    day=atoi(Day);
	    strcpy(usage,Usage);
	    
	    //文件->结构体同步
		CopyData(WFName,0);
		
		//结构体遍历查找 
		for(int i=0;i<user.Rec_len;i++){
		
			if((user.List_Finance[i].amount==amount)&&(user.List_Finance[i].day==day)
			&&(user.List_Finance[i].year==year)&&(user.List_Finance[i].month==month)
			&&(strcmp(user.List_Finance[i].InOrOut,io)==0)&&(strcmp(user.List_Finance[i].Usage,usage)==0)){
				//查找到了,对应下标赋值以及标志位设为真值 
				index=i; 
				FindCheck=true;
				break;
			} 
			
		}
		
		if(FindCheck){
			for(int j=index;j<user.Rec_len-1;j++){
			     user.List_Finance[j]=user.List_Finance[j+1];
			}
			user.List_Finance[user.Rec_len-1]={};//最后一项置空 
			user.Rec_len--;//删除记录,记录长度减一 
			//TestCode
		}
		
		//结构体同步文件
		Synchronize(WFName,user);
		//关闭文件流
		fclose(fp); 
}

void ChangeInfo(char* filename,Users user,int UserNum,char* BAmount,char* BIO,char* BYear,char* BMonth,char* BDay,char* BUsage,char* AAmount,char* AIO,char* AYear,char* AMonth,char* ADay,char* AUsage){
	//从输入框获取信息,对修改之前的信息进行文件核对(核对前,文件->结构体同步信息),若核对成功,返回对应下标
	
	char FIO[2]={""},FUs[20]={""};//IO,备注 
    int TransY,TransM,TransD; //年月日
	float TransA; //金额 
	bool FindCheck = false; //查询标志
	int index; //查询对应下标 
    
    strcpy(FIO,BIO);
    strcpy(FUs,BUsage);
    TransY = atoi(BYear);
    TransM = atoi(BMonth);
    TransD = atoi(BDay);
    TransA = atof(BAmount);
    
    	//文件->结构体同步 
	CopyData(filename,UserNum);
    
	for(int i=0;i<user.Rec_len;i++){
		//testcode
	   if((user.List_Finance[i].amount==TransA)&&(strcmp(user.List_Finance[i].InOrOut,FIO)==0)
	   &&(user.List_Finance[i].year==TransY)&&(user.List_Finance[i].month==TransM)&&
	   (user.List_Finance[i].day==TransD)&&(strcmp(user.List_Finance[i].Usage,FUs)==0)){
	   	index = i; //下标赋值 
	   	FindCheck = true; //查询成功 
	   	break;
	   }
	}
	// 直接对对应下标的结构体信息进行修改,修改前记得把类型转换
	
    if(RegexMatch(MoneyRS,AAmount)&&RegexMatch(DayRS,ADay)
	&&RegexMatch(IORS,AIO)&&RegexMatch(MonthRS,AMonth)
	&&RegexMatch(UsageRS,AUsage)&&RegexMatch(YearRS,AYear)){
	    if(FindCheck){
		//如果找到且输入格式均正确
			user.List_Finance[index].amount=atof(AAmount);
			user.List_Finance[index].day=atoi(ADay);
			strcpy(user.List_Finance[index].InOrOut,AIO);
			user.List_Finance[index].month=atoi(AMonth);
			strcpy(user.List_Finance[index].Usage,AUsage);
			user.List_Finance[index].year=atoi(AYear);
	    }else{
	     xyprintf(700,280,"查询失败");
      	}	
	}else{
		xyprintf(700,280,"输入格式错误");
	}
	
	// 再对结构体->文件信息同步,关闭文件流 
	Synchronize(filename,user);
}

//生成对应预算,并将信息同步
void BudgetOP(char *WFName,char* tag,char* flex,char* fixed,char* free,char* purein){
	//输出格式:SavingTag:Xxx \n Flex:Xxx \n Fixed:Xxx \n Free:Xxx \n PureIn:Xxx \n(先输出,无论哪种情况) 
    FILE* fp = fopen(WFName,"w+");
    
    //设置字体格式 
    setfont(20,0,"楷体"); 
    
    char BRec[15]={""};
    //对结构体的不同属性赋值
	UserInfo[0].List_Budget[0].kind=Saving; //储蓄目标
	UserInfo[0].List_Budget[1].kind=Flex; //灵活开支
	UserInfo[0].List_Budget[2].kind=Fixed; //固定开支
	UserInfo[0].List_Budget[3].kind=Free; //自由开支
	UserInfo[0].List_Budget[4].kind=PureIn; //纯收入 
	//再将对应数值赋值到Budget中,通过相应规则计算出预算
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
	
	//结构体->文件
	
	int sign,dec_pl;
	for(int i=0;i<5;i++){
		char Statistic[20]={""},Kind[2]={""};
		char* Money;
        Money = fcvt(UserInfo[0].List_Budget[i].money,0,&dec_pl,&sign);
		//int->string 
		sprintf(Kind,"%d",UserInfo[0].List_Budget[i].kind);
        //按格式赋值 
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
	//如果PureIn-tag数值>=flex+fixed+free,在底下输出:
	//本月可储存:Xxx,其中SavingTag: Xxx,ExtraSave: Xxx; 
	//按照此预算,一年后约可存储:Xxx,请继续保持 
	if((FPure-FTag)>=(FFlex+FFixed+FFree)){
		xyprintf(450,240,"本月可存储:%.0f元",MonthlySave);
		xyprintf(450,265,"其中SavingTag:%.0f元",FTag);
		xyprintf(450,290,"ExtraSave:%.0f元",ExtraSave);
		xyprintf(450,315,"按照此预算,一年后约可存储:%.0f元",(12*MonthlySave));
		xyprintf(450,340,"请继续保持");
	}
	
	//如果PureIn数值<free+flex+fixed之和,在底下输出:
	//本月将透支:Xxx,其中SavingTag无法完成;
	//按照此预算,一年后约透支:Xxx,请及时调整消费结构 
	if(FPure<FFree+FFlex+FFixed){
	     MonthlySave = -MonthlySave;
	     xyprintf(450,240,"本月将透支:%.0f元",MonthlySave);
	     xyprintf(450,265,"其中SavingTag将无法完成");
	     xyprintf(450,290,"按照此预算,一年后约透支:%.0f元",(12*MonthlySave));
	     xyprintf(450,315,"请及时调整消费结构");
	}else if(FPure-FTag<FFlex+FFixed+FFree){
		//如果PureIn-tag数值<flex+fixed+free,在底下输出:
	    //无法达成此存储预预算,建议可将存储预算调整至 0~MonthlySave;
		xyprintf(450,240,"无法达成此存储预算:%.0f",FTag);
		xyprintf(450,265,"建议将存储预算调整至0~%.0f",MonthlySave);
		xyprintf(450,290,"或及时调整消费结构");
	}
	
	//关闭文件流 
	fclose(fp);
}



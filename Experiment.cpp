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

#define Umax 40 //用户最多 
#define PFmax 75 //PrivateFinance信息一组最多 
#define FBmax 5  //FBudget 信息一组最多 
#define FNmax 50 //文件名最长 
    
//个人财务信息 
typedef struct PriFinance{
	
    float amount; //存入or取出的金额
    char InOrOut[5]; //存入or取出  
	int year; //存入年份 
    int month; //存入月份 
    int day; //存入日期 
    char Usage[50]; //备注(用途or来源)

}PFRec;

typedef enum BudgetKind{
	Saving=1,  //储蓄目标 ----1
	Flex,    //灵活开支  ----2
	Fixed,   //固定开支  ----3
 	Free,    //自由开支   ----4
 	PureIn   //纯收入  ----5 
}Kind;

//财务预算信息
typedef struct FinanceBudget{
 
   Kind kind; //开支类型 
   float money; //开支数量金额
   	
}FBudget; 
 
//用户信息 
typedef struct UserRecord{
	
	//这三个存入 UserInfo文件中 
	int UserNum;  //用户编号,从0开始 
	char name[10]; //用户名 
	char code[20]; //密码
	
	//这个数据不存储至文件中,设为结构体属性 
	int Rec_len; //现存记录条数   
	int Is_administrators; //代表用户是否为管理员,如果是,为1,若不是,为0
	
	char FileLocate[FNmax];   //相应的文件位置 
	
	//分别存入对应的PrivateFinance.txt和FinancialBudget.txt中  
    struct PriFinance List_Finance[PFmax]; //用户对应的个人财务信息
	struct FinanceBudget List_Budget[FBmax]; //用户对应的财务预算信息 
	
}Users; 

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
	 string MoneyRS = "([1-9])(\\d{0,10})";
	//IO ,指定2个字母
	 string IORS = "([IO])";
	//年份,指定1-4位数字,首位不能为0 
	 string YearRS = "([1-9])(\\d{0,4})";//左闭右开
	//月份,指定1-2位数字,首位不为0 
	 string MonthRS = "([1-9])(\\d{0,2})";//左闭右开
	 //日期,指定1-2位数字,首位不为0 
	 string DayRS = "([1-9])(\\d{0,2})";
	 //备注,指定1-20位字母or数字or空格 
	 string UsageRS = "([a-z0-9A-Z ]{1,21})";
	 //匹配U/A开头,后边数字位数4-9的字符串;即ID输入规范 
	 string rs = "([UA])(\\d{4,10})";   
	 //预算输入规范,10位任意数字,首位不能为0 
	 string BTS = "([1-9])(\\d{0,10})"; 
	 
void loginMain(); //登录主界面
void SignIn(); //注册账号主界面
void ChangePassWord(); //修改密码主界面
void MainMenu(); //登录之后的功能主界面,包含基本信息的增删改查模块,信息统计模块,财务预算模块 
void Add_Message(); //添加相应个人开支信息 
void Delete_Message(); //删除相应开支信息(管理员权限) 
void Change_Message(); //修改相应开支信息(管理员权限)
void Query_Message(); //查询相应个人开支信息 
void FinancialBudget(); //财务预算界面功能设计 
/*以上为界面函数*/
void Statistics(char* filename,Users user,int UserNum,int year,int month,int day); //相应开支统计,若相应year,month,day未输入,则自动设置为0; 
void iMage(PIMAGE pimg, const char* filename, int width, int height);//插入图片专用函数
bool change(char* OriID,char* OriPass,char* NewPass);//修改密码功能函数 
bool readIn(char* InID,char* InPass); //核对账号密码函数 
bool Register(char* RId,char* RPass); //校验注册功能 
void InitField(sys_edit* Field,bool ismultline,int x,int y,int Height,COLORS bkcolor,COLORS txcolor,int maxlen,bool isView,int textNum); //生成拥有特定属性的输入框 
void showData(char* filename,int x,int y,int UserNum); //多行展示文件内容,x,y为初始坐标值,UserNum为展示的用户编号 
int count(char* filename); //计算相应文件的行数 
int InitStruct(Users* user); //结构体初始化
void CopyData(char* filename,int UserNum); //文件->结构体
void TotalInOut(Users user,int UserNum); //统计纯收入,输出,目前剩余 并赋值
bool RegexMatch(string Regex,char* MatchString);//前面是正则表达式,后面是待匹配的字符串 
void addInfo(char* filename,char* money,char* io,char* year,char* month,char* day,char* usage); //往对应的文件中添加个人Finance信息 
void Synchronize(char* filename,Users user); //结构体->文件
void DateSort(PFRec RecArray[],int length); //根据日期由前至后排序 早的在前面,晚的在后面 
void SwapRec(PFRec* Pri1,PFRec* Pri2);//交换结构体的信息;
bool CheckDeleteQuery(char* ReadFilename,char* WriteFilename,char* Amount,char* IO,char* Year,char* Month,char* Day,char* Usage);//校验删除查询结果并存入对应文件中,若查询到,则写入对应文件,若未查询到,不进行文件操作,直接关闭读入流  
void DeleteInfo(char* RFName,char* WFName,Users user,int UserNum);//删除信息的操作函数 
void ChangeInfo(char* filename,Users user,int UserNum,char* BAmount,char* BIO,char* BYear,char* BMonth,char* BDay,char* BUsage,char* AAmount,char* AIO,char* AYear,char* AMonth,char* ADay,char* AUsage);
void BudgetOP(char *WFName,char* tag,char* flex,char* fixed,char* free,char* purein);//生成对应预算,并将信息同步 

int main(){
	loginMain();
	closegraph();
	return 0;
}

int InitStruct(Users* user){
	*user = {};
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
    
    //testcode
    printf("\n%.0f->%s->%d->%d->%d->%s\n",TransA,FIO,TransY,TransM,TransD,FUs);
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

//根据日期由前至后排序 早的在前面,晚的在后面 
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
//交换结构体的信息; 
void SwapRec(PFRec* Pri1,PFRec* Pri2){
	
	PFRec temp;
	temp = *Pri1;
	*Pri1 = *Pri2;
	*Pri2 = temp;
	
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

//前面是正则表达式,后面是待匹配的字符串 
bool RegexMatch(string Regex,char* MatchString){
		
		regex expression(Regex); // 字符串传递给构造函数，建立正则表达式
	   	bool ret = regex_match((string)MatchString,expression); //对应正则匹配,匹配正确为true,错误为false 
	   	
	   	return ret;
}

//统计纯收入,输出,目前剩余 并赋值 
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
/*
       声明 sys_edit xxx;然后对其属性进行赋值 
	   bool ismultline //是否多行
	   int x,int y; //对应的x和y坐标
	   int Height; //文字高度
	   COLORS bkcolor; //背景颜色 
	   COLORS txcolor; //文字颜色 
	   int maxlen; //设置输入最大长度
	   bool isView; //设置是否可见 
	   int textNum; //设置输入框长度 
	   默认设置focus() 与 字体为楷体 
*/

//文本框生成函数 
void InitField(sys_edit* Field,bool ismultline,int x,int y,int Height,COLORS bkcolor,COLORS txcolor,int maxlen,bool isView,int textNum){
	
	Field->create(ismultline);     //创建:false单行,true多行 
    Field->move(x,y);       //设置位置 
	Field->size(Height * textNum, Height + 5);	//设置尺寸
	Field->setbgcolor(bkcolor);					//设置背景颜色
	Field->setcolor(txcolor);					//设置文字颜色
	Field->setfont(Height, 0, "楷体");	//设置字体

	Field->setmaxlen(maxlen);					//设置输入最大长度
	Field->visable(isView);					//使可见
    Field->setfocus();                       //设置焦点 
        
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

//插入图片专用函数 
void iMage(PIMAGE pimg, const char* filename, int width, int height){
		PIMAGE temp = newimage();
	    getimage(temp, filename);
	
	if(getwidth(pimg) != width || getheight(pimg) != height)
		resize(pimg, width, height);
		
	putimage(pimg, 0, 0, width, height, temp, 0, 0, getwidth(temp), getheight(temp));

	delimage(temp);
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

//信息展示函数 
void showData(char* filename,int x,int y,int UserNum){
	
	//找到指定文件,将文件内的信息展示出来并存储至相应结构体中 
    FILE *fp = fopen(filename,"r");
    
    int fontHeight = 20;
    setfont(fontHeight,0,"楷体");

    //文件->结构体
    CopyData(filename,UserNum);
    if(UserInfo[UserNum].Rec_len==0){
    	outtextxy(x,y,"尚未查询到记账信息");
    	fclose(fp);
    	return ;
	}
    //下方为SaveFInfo函数 
    int line=0;
    //从文件中读出的信息都是字符数组类型的 
	//atoi(char *str)可以把对应的str转换为数字 用int接收 
    //itoa(num, str, 10); 可以把对应的数字转化为字符串,第三个数字是基数(n进制);
	//函数 atof() 用于将字符串转换为双精度浮点数(double)，
	//其原型为：
    //double atof (const char* str);
	//赋值操作 
	
	//locate记录变化位置 
	int locatex[6],locatey,Orix,Oriy; 
	Orix=x; Oriy=y;  //记录初始位置 
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
		line++;//行数自加 
	}
	fclose(fp);
	
}

//登录主界面
void loginMain(){
 
	initgraph(640,480,0);			
	setcaption("登陆主界面");
	
	//输入框设置 
	sys_edit editBox,passBox;
    InitField(&editBox,false,190,100,25,WHITE,BLACK,10,true,10);
    InitField(&passBox,false,190,180,25,WHITE,BLACK,20,true,10);
    
	//放置背景图片 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"登陆.jpg",640,480);
	putimage(0,0,img);
	
    setbkcolor(WHITE);
	setbkmode(TRANSPARENT);
	
	setcolor(RED);
	setfont(70,15,"楷体");
	setlinewidth(2); 
    outtextxy(0,0,"个人财务管理软件");
    
    setfont(40,10,"楷体");
    xyprintf(100,100,"用户名:");
    xyprintf(100,180,"密码:");
    
    //设置字体背景色为白色 
	setbkcolor(EGERGB(0xFC,0xFC,0XFC));
	
		    setfont(30,15,"楷体");
		    setlinewidth(1);
		
		    rectangle(100,320,220,390);
			rectangle(260,320,380,390);
			rectangle(420,320,540,390);
			
			setcolor(EGERGB(44,0,15));
			
			outtextxy(130,337,"注册");
			outtextxy(290,337,"登陆");
			outtextxy(420,337,"修改密码");
			
			//设置输入判断正则表达式
			string Admin = "([A])(\\d{4,10})";
			string User = "([U])(\\d{4,10})"; 
			
			//设置鼠标信息 
			mouse_msg msg;
	
	for(;is_run();delay_fps(60)){
		
	    bool ChangeClick = false;//确认是否点击修改密码 
	    bool RegisterClick = false; //确认是否点击注册
	    bool SignClick = false; //确认是否点击登录 
		
		
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
	    //核对账号密码是否正确 
		//输入缓冲区
	     int buffSize = 100,passSize = 100;
	    char strBuff[10] = "",strPass[20] = ""; 
	    //获取输入框内容 
		editBox.gettext(buffSize, strBuff);
		passBox.gettext(passSize, strPass);
	
	     if(readIn(strBuff,strPass)){
	     	//若为管理员 
	     	if(RegexMatch(Admin,strBuff)){
	     		is_Admin = true;
			 }
			 //若为用户
			 if(RegexMatch(User,strBuff)){
			 	is_Admin = false;
			 } 
	     	
	     	cleardevice();
	     	editBox.destory();
			passBox.destory();
			Sleep(100); 
		    MainMenu();
		   }else{
		   	setfont(30,10,"楷体");
		   	setcolor(RED); 
		   	outtextxy(130,450,"输入错误,请重新检查后输入!");
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

//注册账号主界面
void SignIn(){
	
	initgraph(720,480,0);
	setcaption("注册账号");
	
	//放置背景图片 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"注册.jpg",720,480);
	putimage(0,0,img);
	
	setbkmode(TRANSPARENT);
	
	setcolor(BLACK);
	setfont(70,15,"楷体");
	setlinewidth(2); 
    outtextxy(0,0,"注册账号");
    
    setfont(40,10,"楷体");
    xyprintf(100,100,"用户名:");
    xyprintf(100,170,"密码:");

    //设置字体背景色为白色 
	setbkcolor(EGERGB(0xFC,0xFC,0XFC));
	
	        setfont(30,0,"楷体");
		    setlinewidth(1);
		    
		
		    rectangle(100,320,220,390);
			rectangle(260,320,380,390);
			
			setcolor(EGERGB(44,0,15));
			
			outtextxy(130,337,"确认");
			outtextxy(290,337,"返回");
			
			//捕获鼠标信息 
			mouse_msg msg1;
			
            //设置输入框
			sys_edit RID,RCode; //Register ID&&Register PassCode
			InitField(&RID,false,210,100,25,WHITE,BLACK,10,true,10);
			InitField(&RCode,false,210,170,25,WHITE,BLACK,20,true,10);
			//设置输入提示
			 setfont(20,0,"楷体");
			 setcolor(RED);
			xyprintf(140,10,"用户名以A/U开头,后方接4~9个数字(注意格式规范)");
			xyprintf(140,40,"其中A开头为管理员,可使用修改与删除记录,U开头则无法使用");
		    
						
		for(;is_run();delay_fps(60)){
		
	    bool CheckClick = false;//是否点击确认 
	    bool ReturnClick = false; //是否点击返回 

		
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
        	
		regex expression(rs); // 字符串传递给构造函数，建立正则表达式
		   	
	   	//把新注册的用户信息存入到相应文件内
	   	 int IDSize=10,CodeSize=20;
	   	char ID[10],Code[20];
	   	RID.gettext(IDSize,ID);
	   	RCode.gettext(CodeSize,Code);
	   	
	   	bool ret = regex_match((string)ID,expression); //ID对应正则匹配,匹配正确为true,错误为false 
	   	
	   	//其中Register包含操作,返回一个bool类型的值 
		if(ret){
			outtextxy(130,400,"注册成功!");
		     Register(ID,Code);
		    
		}else{
	       outtextxy(130,400,"注册失败:错误格式/已存在该用户");
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

//修改密码主界面
void ChangePassWord(){
	
	initgraph(640,480,0);
	setcaption("修改密码");
	
	//同上,放置背景图片 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"修改密码.jpg",640,480);
	putimage(0,0,img);
	
	setcolor(RED);
	setfont(70,15,"楷体");
	setlinewidth(2); 
    outtextxy(0,0,"修改密码");
    
    setfont(40,10,"楷体");
    xyprintf(100,100,"用户名:");
    xyprintf(100,150,"原密码:");
    xyprintf(100,200,"现密码:");
//    xyprintf(100,250,"确认密码:");
    
    //设置字体背景色为白色 
	setbkcolor(EGERGB(0xFC,0xFC,0XFC));
	
	        setfont(30,0,"楷体");
		    setlinewidth(1);
		    setbkmode(TRANSPARENT);
		
		    rectangle(100,320,220,390);
			rectangle(260,320,380,390);
			
			setcolor(EGERGB(44,0,15));
			
			outtextxy(130,337,"确认");
			outtextxy(290,337,"返回");
			
			//设置输入框信息
			sys_edit CID,COriCode,CNewCode; //ID,原密码,新密码
			InitField(&CID,false,190,100,25,WHITE,BLACK,10,true,10);
			InitField(&COriCode,false,190,150,25,WHITE,BLACK,20,true,10);
			InitField(&CNewCode,false,190,200,25,WHITE,BLACK,20,true,10); 
			
			//捕获鼠标信息 
			mouse_msg msg2;
			
		for(;is_run();delay_fps(60)){
		
	    bool CheckClick1 = false;//是否点击确认 
	    bool ReturnClick1 = false; //是否点击返回 

		
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
	   	
	   	//一通文件操作.....把修改密码后的用户信息存入相应文件中(CID,COriCode,CNewCode)
		 int CIDSize=10,CCodeSize=20;
	   	char ID[10],Code[20],NewCode[20];
	    CID.gettext(CIDSize,ID);
	    COriCode.gettext(CCodeSize,Code);
	    CNewCode.gettext(CCodeSize,NewCode);
	   	
	   	if(change(ID,Code,NewCode)){
	   	    outtextxy(130,400,"修改成功!");
		}else{
			outtextxy(130,400,"修改失败!");
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

//登录之后的功能主界面,包含基本信息的增删改查模块,信息统计模块,财务预算模块 
void MainMenu(){ 
	
	initgraph(1440,800,0);			
	setcaption("功能主界面");
	
		//放置背景图片 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"功能主界面.jpg",1440,800);
	putimage(0,0,img);
	
    setbkcolor(WHITE);
	
	setcolor(BLACK);
	setbkmode(TRANSPARENT);

    int HeadHeight = 30;
    setfont(HeadHeight,0,"楷体");
    xyprintf(10,10,"您好,Xxx");
    
    int ButtonHeight = 50;
    setfont(ButtonHeight,0,"楷体");
    xyprintf(570,10,"功能主界面");
    xyprintf(40,90,"增加记录");
    xyprintf(320,90,"查询统计");
	xyprintf(600,90,"删除记录");
	xyprintf(880,90,"财务预算"); 
	xyprintf(1160,90,"修改记录"); 
	xyprintf(880,10,"返回登陆");
	
	int InfoHeight = 40;
	setfont(InfoHeight,0,"楷体");
	xyprintf(20,170,"金额");
	xyprintf(280,170,"存取");
	xyprintf(540,170,"年份");
	xyprintf(800,170,"月份");
	xyprintf(1060,170,"日期");
	xyprintf(1320,170,"备注");
	
	//分割线 
	line(0,160,1440,160);
	
	//rectangle 左部x,上部y,右部x,下部y 
	//循环画框,只有x1,x2变化,y不做变化,且x1与x2均+=280 
	int i=40,j=240,count=1;
	for(;count<=5;count++,i+=280,j+=280){
		rectangle(i,90,j,140);
	}

    rectangle(880,10,1080,60);
    
    //显示相应文件信息 
    char filename[20]="PrivateFinance0.txt";
    showData(filename,20,210,0);
    
    //鼠标信息 
        mouse_msg msg3;

	for(;is_run();delay_fps(60)){
		
	    bool AddClick = false;//确认是否点击增加记录 
	    bool SearchClick = false; //确认是否点击查询统计 
	    bool DeleteClick = false; //确认是否点击删除记录
		bool BudgetClick = false; //确认是否点击财务预算
		bool ChangeClick = false; //确认是否点击修改记录 
		bool BackClick = false; //确认是否点击返回上一个界面
		 
		
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
		//删除与修改为管理员权限
		 
		 	if(DeleteClick){
		        if(is_Admin){
		   	      Delete_Message();
		        }else{
		           setfont(25,0,"楷体");
		 	       setcolor(RED);
		 	       xyprintf(240,10,"进入失败,未检测到管理员身份");
				}
		    
		    }
		    if(ChangeClick){
		    	if(is_Admin){
		        	Change_Message();
				}else{
					setfont(25,0,"楷体");
		 	       setcolor(RED);
		 	       xyprintf(240,10,"进入失败,未检测到管理员身份");
				}
		    }
		if(BackClick){
			//返回登陆界面 
			loginMain();
		}
   	} 
}

//添加相应个人开支信息:获取输入框信息->存入文件->进行(文件->结构体操作) 
void Add_Message(){
	initgraph(960,480,0);
	setcaption("增加记录");
	setbkcolor(WHITE);
	setcolor(BLACK);
	setbkmode(TRANSPARENT);
	
		//放置背景图片 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"增加记录.jpg",960,480);
	putimage(0,0,img);
	
	int HeadHeight = 50;
	setfont(HeadHeight,0,"楷体");
	xyprintf(10,10,"增加记录"); 
	
	line(0,70,800,70); 
	
	int TextHeight = 40;
	setfont(TextHeight,0,"楷体");
	
	xyprintf(70,120,"金额:");
	xyprintf(70,180,"时间:");
	xyprintf(70,240,"In/Out:");
    xyprintf(70,300,"用途:");
    xyprintf(210,360,"确定"); 
    xyprintf(370,360,"返回");
	
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
	
	xyprintf(300,180,"年");
	xyprintf(400,180,"月");
	xyprintf(500,180,"日");
	
	//说明注释文字
	setfont(22,0,"楷体");
	setcolor(RED); 
	xyprintf(465,125,"一般为大于0的整数金额");
	xyprintf(550,180,"此处为yyyy-mm-dd格式");
	xyprintf(550,205,"均为大于0的整数,年月日第一位建议不为0");
	xyprintf(465,250,"此处为大写I/O,其中I为存入,O为支出");
	xyprintf(465,310,"此处为20个字符以内的字母/数字/空格");
	for(;is_run();delay_fps(60)){
		
	    bool CheckClick2 = false;//是否点击确认 
	    bool ReturnClick2 = false; //是否点击返回 

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
	   	
	   	//把增添的Finance信息存入到相应文件内
	   	 int MoneySize=10,YearSize=5,MonthSize=3,DaySize=3,IOSize=2,UsageSize=20;
	   	char amount[10],year[5],month[3],day[3],io[2],usage[20];
	   	Money.gettext(MoneySize,amount);
	   	Year.gettext(YearSize,year);
	   	Month.gettext(MonthSize,month);
	   	Day.gettext(DaySize,day);
	   	IO.gettext(IOSize,io);
	   	Usage.gettext(UsageSize,usage);

	 
	 char filename[20]="PrivateFinance0.txt";
	 	   	
	   	//每个框的输入都合法 
	   	if(RegexMatch(MoneyRS,amount)&&RegexMatch(IORS,io)&&RegexMatch(YearRS,year)&&RegexMatch(MonthRS,month)&&RegexMatch(DayRS,day)&&RegexMatch(UsageRS,usage)){
	   		   //进行添加至对应文件操作
	   		   //文件->结构体;结构体排序;结构体->文件 
	   		   CopyData(filename,0);
	   		   //addInfo操作,将输入框获取的信息全部加入到指定文件中 
			   addInfo(filename,amount,io,year,month,day,usage);
			   
			   UserInfo[0].Rec_len++;//对记录长度加1 
			   CopyData(filename,0);
			   DateSort(UserInfo[0].List_Finance,UserInfo[0].Rec_len);
			   Synchronize(filename,UserInfo[0]);
			   
			   outtextxy(210,410,"添加成功!!");
	    }else{
	    	
	    	outtextxy(210,410,"添加失败,格式输入错误");
	    	
		}
		    //输入后对框内信息进行删除 
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

//删除相应开支信息(管理员权限)
void Delete_Message(){
	initgraph(800,480,0);
	setcaption("删除记录");
	setbkcolor(WHITE);
	setcolor(BLACK);
	setbkmode(TRANSPARENT);
	
	//放置背景图片 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"删除记录.jpg",800,480);
	putimage(0,0,img);
	
	int HeadHeight = 50;
	setfont(HeadHeight,0,"楷体");
	xyprintf(10,0,"删除记录"); 
	
	line(0,360,960,360); 
	
	int TextHeight = 40;
	setfont(TextHeight,0,"楷体");
	xyprintf(30,370,"金额");
	xyprintf(190,370,"存取");
	xyprintf(350,370,"时间");
	xyprintf(510,370,"备注");
	
	setfont(40,0,"楷体"); 
	xyprintf(270,185,"年");
	xyprintf(360,185,"月");
	xyprintf(440,185,"日");
	
	xyprintf(70,60,"金额:");
	xyprintf(70,120,"存取:");
	xyprintf(70,180,"时间:");
	xyprintf(70,240,"备注:");
	xyprintf(60,300,"查询");
	xyprintf(220,300,"确认删除");
	xyprintf(480,300,"返回");
	
	rectangle(60,300,160,340);
	rectangle(220,300,390,340);
	rectangle(480,300,580,340);
	
	//设置输入信息  
	setfont(20,0,"楷体");
	setcolor(RED);
	xyprintf(475,60,"首先按格式输入信息");
	xyprintf(475,80,"再点击查询按钮");
	xyprintf(475,120,"若对应信息不存在");
	xyprintf(475,140,"则下方输出查询失败");
	xyprintf(475,180,"若对应信息匹配");
	xyprintf(475,200,"点击查询按钮,下方显示对应信息");
	xyprintf(475,220,"此时再次点击确认删除即可删除成功");
	xyprintf(475,240,"金额:10位以内整数,存/取:I/O");
	xyprintf(475,260,"时间:yyyy-mm-dd"); 
	xyprintf(475,280,"备注:20位以内数字/字母/空格");
	
	//鼠标信息 
	mouse_msg msg6;
	
	//输入框初始化
	sys_edit DMoney,DIO,DYear,DMonth,DDay,DUsage; //金额,收支,时间,备注
	InitField(&DMoney,false,170,65,25,WHITE,BLACK,10,true,10); 
	InitField(&DIO,false,170,125,25,WHITE,BLACK,5,true,10);
     InitField(&DYear,false,170,185,25,WHITE,BLACK,4,true,4);
     InitField(&DMonth,false,310,185,25,WHITE,BLACK,2,true,2);
     InitField(&DDay,false,390,185,25,WHITE,BLACK,2,true,2);
	InitField(&DUsage,false,170,245,25,WHITE,BLACK,20,true,10); 
	
	//查询,显示信息,确认删除
	//1.输入框获取信息,与文件里信息挨个比对
	//若查询到,则显示在下面的框; 
	//若未查询到,在下方显示未找到此记录 
	//2.查询到信息,若点击确认删除,则先对结构体进行节点删除操作
	//再将结构体内容同步至文件中
	 
	//读入写入文件名赋值 
	   	char PFF[20]="PrivateFinance0.txt",DQF[20]="DeleteQuery.txt";
	
		for(;is_run();delay_fps(60)){
			
		bool Query = false; //是否点击查询 
	    bool CheckDel = false;//是否点击确认删除 
	    bool Return = false; //是否点击返回 

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
	    //字符串接收输入框信息	
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
			setfont(25,0,"楷体"); 
			xyprintf(30,420,"查询失败,格式错误/未查询到相应记录");
		}
		DMoney.settext("");
		DIO.settext("");
		DYear.settext("");
		DMonth.settext("");
		DDay.settext("");
		DUsage.settext("");
	 }
	    //点击确认删除 
		if(CheckDel){
			DeleteInfo(DQF,PFF,UserInfo[0],0);
		}
		
		//点击返回	
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

//修改相应开支信息(管理员权限) 
void Change_Message(){
    initgraph(1040,720,0);
	setcaption("修改记录");
	setbkcolor(WHITE);
	setcolor(BLACK);
	setbkmode(TRANSPARENT);
	
	//本界面功能需要读取的文件名
	char filename[20]="PrivateFinance0.txt"; 
	
		//放置背景图片 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"修改记录.jpg",1040,720);
	putimage(0,0,img);
	
	int InfoHeight = 25;
	setfont(InfoHeight,0,"楷体");
	setcolor(RED);
	xyprintf(660,100,"Before输入需要和开支记录一一对应");
	xyprintf(660,140,"按输入规则输入所有信息后");
	xyprintf(660,180,"点击确认修改按钮");
	xyprintf(660,220,"修改成,可点击返回至主界面校对");
	
	int HeadHeight = 35;
	setfont(HeadHeight,0,"楷体");
	setcolor(BLACK);
	xyprintf(20,15,"修改记录");
	xyprintf(20,95,"金额(Before):");
	xyprintf(20,145,"存取(Before):");
	xyprintf(20,205,"时间(Before):");
	xyprintf(400,205,"年"); 
	xyprintf(520,205,"月");
	xyprintf(630,205,"日");
	xyprintf(20,260,"备注(Before):");
	line(0,320,1040,320);
	xyprintf(20,335,"金额(After-):");
	xyprintf(20,415,"存取(After-):");
	xyprintf(20,495,"时间(After-):");
	xyprintf(400,505,"年");
	xyprintf(520,505,"月");
	xyprintf(630,505,"日");
	xyprintf(20,575,"备注(After-):");
	
	xyprintf(20,655,"确认修改");
	xyprintf(380,655,"返回");
	
	rectangle(20,655,260,705);
	rectangle(380,655,500,705);
	
	//输入框信息A(After),B(Before)
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
	
	//鼠标信息 
	mouse_msg msg8; 
	
	for(;is_run();delay_fps(60)){
			
	    bool CheckCh = false;//是否点击确认修改 
	    bool Return = false; //是否点击返回 

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
		   
		   //if判断对修改前信息进行正则匹配 
		  if(RegexMatch(MoneyRS,BAm)&&RegexMatch(YearRS,BYe)&&RegexMatch(MonthRS,BMo)
		  &&RegexMatch(DayRS,BDa)&&RegexMatch(IORS,BIo)&&RegexMatch(UsageRS,BUs)){
		  	//ChangeInfo对修改后信息输入进行正则匹配 
		  	ChangeInfo(filename,UserInfo[0],0,BAm,BIo,BYe,BMo,BDa,BUs,AAm,AIo,AYe,AMo,ADa,AUs);
			xyprintf(700,180,"成功,返回查看结果");
		  }else{
			xyprintf(700,180,"失败,修改前输入格式错误");
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

//查询相应个人开支信息 
void Query_Message(){
	
	initgraph(960,640,0);
	setcaption("查询统计");
	setbkcolor(WHITE);
	setcolor(BLACK);
	setbkmode(TRANSPARENT);
	
		//放置背景图片 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"查询统计.jpg",960,640);
	putimage(0,0,img);
	
	int HeadHeight = 50;
	setfont(HeadHeight,0,"楷体");
	xyprintf(20,20,"查询统计");
	
	 
	//读取文件名 
	char filename[20]="PrivateFinance0.txt";
	//文件内容同步
	 CopyData(filename,0);	
	//统计总输入输出and净余额 
	TotalInOut(UserInfo[0],0);
	//把float转为string类型
	//在指定位置输出 
    char In[15],Out[15],totalMon[15];
    setfont(22,0,"幼圆"); 
    
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
	setfont(TextHeight,0,"楷体");
	xyprintf(20,90,"时间:");
	xyprintf(400,90,"年");
	xyprintf(580,90,"月");
	xyprintf(760,90,"日");
	xyprintf(240,160,"查询");
	xyprintf(600,160,"返回");
	
	rectangle(240,160,360,210);
	rectangle(600,160,720,210);
	
	
	//画分割线 
	line(0,240,960,240); 
	
	int InfoHeight = 30;
	setfont(InfoHeight,0,"楷体");
	
	//数据显示说明 
	setfont(20,0,"楷体");
	setcolor(RED);
	xyprintf(10,245,"数据说明");
	xyprintf(130,265,"最上方统计的是所有时间的收支状况");
	xyprintf(130,285,"请先输入日期后,再点击查询按钮,系统将自动计算结果并输出至下方");
	xyprintf(130,305,"输入格式举例:2014 4 3,2014 4,前者统计当天,后者统计一个月(状况)");
	xyprintf(130,325,"若框中均无数据,则统计无效");
	
	//定义输入框
	sys_edit Year,Month,Day;
	InitField(&Year,false,150,90,TextHeight,WHITE,BLACK,4,true,4); 
	InitField(&Month,false,460,90,TextHeight,WHITE,BLACK,2,true,2);
	InitField(&Day,false,640,90,TextHeight,WHITE,BLACK,2,true,2);
	
	//定义鼠标信息 
	mouse_msg msg8;
			
		for(;is_run();delay_fps(60)){
		
	    bool Query = false;//是否点击生成预算 
	    bool ReturnMain = false; //是否点击返回 

		
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
		  //获取输入框信息,均为字符串
		int QYear=5,QMonth=3,QDay=3;
	   	char QY[5],QM[3],QD[3];
	   	Year.gettext(QYear,QY);
	   	Month.gettext(QMonth,QM);
	   	Day.gettext(QDay,QD);
	   	
	   		 //将字符串转换为对应的数字,如果没有输入,即等于"",则将对应的数字设置为0
		  int TransNum[3];//分别对应年月日
		  char Sym[2]=""; //空字符 
		  //年 
		  if(strcmp(QY,Sym)==0){
		  	 TransNum[0]=0;
		  }else{
		  	 TransNum[0]=atoi(QY);
		  }  
		  //月 
		  if(strcmp(QM,Sym)==0){
		  	 TransNum[1]=0;
		  }else{
		  	 TransNum[1]=atoi(QM);
		  }
		  //日
		  if(strcmp(QD,Sym)==0){
		  	TransNum[2]=0;
		  }else{
		  	TransNum[2]=atoi(QD);
		  } 
		  //把对应的参数传入Statistics(....)函数中进行全局数组赋值;
		  Statistics(filename,UserInfo[0],0,TransNum[0],TransNum[1],TransNum[2]);
          
          //对应位置输出上述数据
          char TI[15],TO[15],TT[15];
         sprintf(TI,"%.0f",StatisticIncome[0]);
         sprintf(TO,"%.0f",StatisticOutcome[0]);
         sprintf(TT,"%.0f",StatisticTotal[0]);

         //设置输出字体以及位置 
         setfont(40,0,"楷体");
          xyprintf(100,480,"AreaIncome:");
		  xyprintf(400,480,TI);
		  xyprintf(100,520,"AreaOutcome:");
		  xyprintf(400,520,TO);
		  xyprintf(100,560,"AreaBalance:"); 
          xyprintf(400,560,TT);
          //全局变量赋值显示后初始化 
          StatisticIncome[0]=0;
          StatisticOutcome[0]=0;
          StatisticTotal[0]=0;
          
          //输入框清空 
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

//财务预算界面功能设计
void FinancialBudget(){
	
	initgraph(800,480,0);
	setcaption("财务预算");
	setbkcolor(WHITE);
	setcolor(BLACK);
	setbkmode(TRANSPARENT);
	
	//放置背景图片 
	PIMAGE img;
	img = newimage(50,50);
	iMage(img,"财务预算.jpg",800,480);
	putimage(0,0,img);
	
	int HeadHeight = 50;
	setfont(HeadHeight,0,"楷体");
	xyprintf(10,10,"财务预算"); 
	
	line(0,70,800,70); 
	
	int TextHeight = 30;
	setfont(TextHeight,0,"楷体");

	xyprintf(70,130,"储蓄目标");
	xyprintf(70,170,"灵活开支");
	xyprintf(70,210,"固定开支");
    xyprintf(70,250,"自由开支");
    xyprintf(70,290,"纯收入  ");
    xyprintf(130,360,"生成预算"); 
    xyprintf(370,360,"返回");
	
	//提示信息
	setfont(20,0,"楷体");
	setcolor(RED);
	xyprintf(480,130,"请按相应开支类型填入对应金额");
	xyprintf(480,160,"填写完毕后点击生成预算按钮"); 
	xyprintf(480,190,"系统将自动计算出相应对策与结果");
	xyprintf(480,220,"填写范围为(0~9999999999)");
	//按钮框 
	setcolor(BLACK);
	rectangle(130,360,290,400);
	rectangle(370,360,468,400);
	
	    //输入框
		sys_edit Tag,Flex,Fixed,Free,PureIn; //储蓄目标,灵活开支,固定开支,自由开支,纯收入 
		InitField(&Tag,false,200,130,25,WHITE,BLACK,10,true,10);
		InitField(&Flex,false,200,170,25,WHITE,BLACK,10,true,10);
		InitField(&Fixed,false,200,210,25,WHITE,BLACK,10,true,10);
		InitField(&Free,false,200,250,25,WHITE,BLACK,10,true,10);
		InitField(&PureIn,false,200,290,25,WHITE,BLACK,10,true,10);
		
		//读取文件名称
		char filename[25]="FinancialBudget.txt"; 
		//捕获鼠标信息 
			mouse_msg msg7;
			
		for(;is_run();delay_fps(60)){
		
	    bool Budget = false;//是否点击生成预算 
	    bool Return = false; //是否点击返回 

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
	   	
	   	//获取输入框信息,均为字符串
		int BudSize = 10;
		//Tag,Flex,Fixed,Free,PureIn; 储蓄目标,灵活开支,固定开支,自由开支,纯收入 
	   	char BTag[10],BFlex[10],BFixed[10],BFree[10],BPureIn[10]; 

	   	Tag.gettext(BudSize,BTag);
	   	Flex.gettext(BudSize,BFlex);
	   	Fixed.gettext(BudSize,BFixed);
	   	Free.gettext(BudSize,BFree);
	   	PureIn.gettext(BudSize,BPureIn);
	    //判断输入是否合法,合法则进行函数调用操作
		  if(RegexMatch(BTS,BTag)&&RegexMatch(BTS,BFlex)&&
		  RegexMatch(BTS,BFixed)&&RegexMatch(BTS,BFree)&&RegexMatch(BTS,BPureIn)){
		  	 //在对应区域输出预算结果
			 BudgetOP(filename,BTag,BFlex,BFixed,BFree,BPureIn); 
		  	 xyprintf(300,50,"预算成功！");
		  }else{
		  	 xyprintf(300,50,"输入格式错误!");
		  }
		  
		  //输入框清空 
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

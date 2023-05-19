#ifndef STRUCT_H
#define STRUCT_H

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
	
	
	//分别存入对应的PrivateFinance.txt和FinancialBudget.txt中  
    struct PriFinance List_Finance[PFmax]; //用户对应的个人财务信息
	struct FinanceBudget List_Budget[FBmax]; //用户对应的财务预算信息 
	
}Users; 

#endif 

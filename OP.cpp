#include <graphics.h>
#include <stdlib.h>
#include <ege/sys_edit.h>
#include <sys/types.h>
#include <regex> 
#include <string>
#include <iostream>
#include "Struct.h"
#include "OP.h"
#include "File.h"
#include "View.h"
using namespace std;

extern float Total[Umax]; //现存总金额 
extern float Income[Umax]; //纯收入 
extern float Outcome[Umax]; //纯支出  
extern Users UserInfo[Umax]; //用户的所有信息

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

//插入图片专用函数 
void iMage(PIMAGE pimg, const char* filename, int width, int height){
		PIMAGE temp = newimage();
	    getimage(temp, filename);
	
	if(getwidth(pimg) != width || getheight(pimg) != height)
		resize(pimg, width, height);
		
	putimage(pimg, 0, 0, width, height, temp, 0, 0, getwidth(temp), getheight(temp));

	delimage(temp);
}

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


//前面是正则表达式,后面是待匹配的字符串 
bool RegexMatch(string Regex,char* MatchString){
		
		regex expression(Regex); // 字符串传递给构造函数，建立正则表达式
	   	bool ret = regex_match((string)MatchString,expression); //对应正则匹配,匹配正确为true,错误为false 
	   	
	   	return ret;
}

//交换结构体的信息; 
void SwapRec(PFRec* Pri1,PFRec* Pri2){
	
	PFRec temp;
	temp = *Pri1;
	*Pri1 = *Pri2;
	*Pri2 = temp;
	
} 

int InitStruct(Users* user){
	*user = {};
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


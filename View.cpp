#include <graphics.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ege/sys_edit.h>
#include <sys/types.h>
#include <regex> 
#include <string>
#include <iostream>
#include "OP.h"
#include "File.h"
#include "Struct.h"
#include "View.h"
using namespace std;


  extern float Total[Umax]; //现存总金额 
   extern float Income[Umax]; //纯收入 
   extern float Outcome[Umax]; //纯支出  
   extern Users UserInfo[Umax]; //用户的所有信息
   extern float StatisticIncome[Umax]; //统计界面收入
   extern float StatisticOutcome[Umax]; //统计界面支出
   extern float StatisticTotal[Umax]; //统计界面现存金额 
   extern bool is_Admin; //判断是否为管理员 
	
//正则表达式规定输入格式 
	//金额,1~10位任意数字,首位不为0; 
	extern	string  MoneyRS;
	//IO ,指定2个字母
	extern	string  IORS;
	//年份,指定1-4位数字,首位不能为0 
	extern	string  YearRS;//左闭右开
	//月份,指定1-2位数字,首位不为0 
	extern	string  MonthRS;//左闭右开
	 //日期,指定1-2位数字,首位不为0 
	extern	string  DayRS;
	 //备注,指定1-20位字母or数字or空格 
	extern	string  UsageRS;
	 //匹配U/A开头,后边数字位数4-9的字符串;即ID输入规范 
	extern	string  rs;   
	 //预算输入规范,10位任意数字,首位不能为0 
	extern	string  BTS;


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

//	 string MoneyRS = "([1-9])(\\d{0,10})";
//	 string IORS = "([IO])";
//	 string YearRS = "([1-9])(\\d{0,4})";//左闭右开
//	 string MonthRS = "([1-9])(\\d{0,2})";//左闭右开
//	 string DayRS = "([1-9])(\\d{0,2})";
//	 string UsageRS = "([a-z0-9A-Z ]{1,21})";
	 
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
			xyprintf(350,420,"删除成功！");
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
		   
//	 string MoneyRS = "([1-9])(\\d{0,10})";
//	 string IORS = "([IO])";
//	 string YearRS = "([1-9])(\\d{0,4})";//左闭右开
//	 string MonthRS = "([1-9])(\\d{0,2})";//左闭右开
//	 string DayRS = "([1-9])(\\d{0,2})";
//	 string UsageRS = "([a-z0-9A-Z ]{1,21})";
		   
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
	   	
//	 string BTS = "([1-9])(\\d{0,10})";
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

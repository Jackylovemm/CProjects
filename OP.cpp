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

extern float Total[Umax]; //�ִ��ܽ�� 
extern float Income[Umax]; //������ 
extern float Outcome[Umax]; //��֧��  
extern Users UserInfo[Umax]; //�û���������Ϣ

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

//����ͼƬר�ú��� 
void iMage(PIMAGE pimg, const char* filename, int width, int height){
		PIMAGE temp = newimage();
	    getimage(temp, filename);
	
	if(getwidth(pimg) != width || getheight(pimg) != height)
		resize(pimg, width, height);
		
	putimage(pimg, 0, 0, width, height, temp, 0, 0, getwidth(temp), getheight(temp));

	delimage(temp);
}

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


//ǰ����������ʽ,�����Ǵ�ƥ����ַ��� 
bool RegexMatch(string Regex,char* MatchString){
		
		regex expression(Regex); // �ַ������ݸ����캯��������������ʽ
	   	bool ret = regex_match((string)MatchString,expression); //��Ӧ����ƥ��,ƥ����ȷΪtrue,����Ϊfalse 
	   	
	   	return ret;
}

//�����ṹ�����Ϣ; 
void SwapRec(PFRec* Pri1,PFRec* Pri2){
	
	PFRec temp;
	temp = *Pri1;
	*Pri1 = *Pri2;
	*Pri2 = temp;
	
} 

int InitStruct(Users* user){
	*user = {};
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


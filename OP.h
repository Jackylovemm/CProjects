#ifndef OP_H
#define OP_H

#include <stdio.h>
#include <graphics.h>
#include <ege/sys_edit.h>
#include <sys/types.h>
#include <string>
#include "Struct.h"
using namespace std;

int InitStruct(Users* user); //�ṹ���ʼ��
void DateSort(PFRec RecArray[],int length);//����������ǰ�������� �����ǰ��,����ں��� 
void SwapRec(PFRec* Pri1,PFRec* Pri2);//�����ṹ�����Ϣ
bool RegexMatch(string Regex,char* MatchString); //ǰ����������ʽ,�����Ǵ�ƥ����ַ���
void TotalInOut(Users user,int UserNum); //ͳ�ƴ�����,���,Ŀǰʣ�� ����ֵ
void InitField(sys_edit* Field,bool ismultline,int x,int y,int Height,COLORS bkcolor,COLORS txcolor,int maxlen,bool isView,int textNum);//�ı������ɺ���
void iMage(PIMAGE pimg, const char* filename, int width, int height);//����ͼƬר�ú���
void showData(char* filename,int x,int y,int UserNum);//��Ϣչʾ���� 

#endif

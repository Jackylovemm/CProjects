#ifndef VIEW_H
#define VIEW_H

//所有的界面函数 
void loginMain(); //登录主界面
void SignIn(); //注册账号主界面
void ChangePassWord(); //修改密码主界面
void MainMenu(); //登录之后的功能主界面,包含基本信息的增删改查模块,信息统计模块,财务预算模块 
void Add_Message(); //添加相应个人开支信息 
void Delete_Message(); //删除相应开支信息(管理员权限) 
void Change_Message(); //修改相应开支信息(管理员权限)
void Query_Message(); //查询相应个人开支信息 
void FinancialBudget(); //财务预算界面功能设计 

#endif

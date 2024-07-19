**《高校教务管理系统》开发报告**

**开发成员：本人**

**权重分配：无**

1. 项目概述

**基本内容：**

教务管理系统在高校教学管理信息化中起着关键的角色，帮助高校提高教学效率、优化资源利用、加强学生管理和教师管理，并提供数据支持和决策依据，促进高校教学质量的提升。

教务管理系统中有三类角色：管理员、教师和学生。

1、管理员：管理员可以管理课程、管理教师、管理学生、重置人员密码、发布校园公告、校建管理功能；简化起见，只设置了admin 管理员。

2、教师：教师根据职工编号和密码登录教师系统，可以查看自己所教授的课程信息、查看某门课程所选学生信息和录入考试成绩、修改自己的登录密码；

3、学生：学生根据学号和密码登录自己的学生系统，可以查看所有的课程信息、选课以及退课、查看已选课程、绩点显示。

**主要参加人员：**本人

主要技术：mysql, Qt ,navicat数据库设计

功能概述：通过对学生的账号和密码以及管理员账号和密码以及老师的账号和密码，通过与数据库的匹配，识别进入不同的系统。实现管理员的增删改查，以及学生和老师的查询等

系统测试：

## **登录画面：**
![image](https://github.com/user-attachments/assets/b0a40b6a-2165-4924-986c-efdc4bc055ac)

## **教务功能界面**

教务-学生设置画面：
![image](https://github.com/user-attachments/assets/0faa069f-63c3-4bed-b1a2-45e29b9a6436)


教务-老师设置画面：
![image](https://github.com/user-attachments/assets/093da723-8433-4867-83fc-d4be6771bc0e)

教务-课程设置画面：
![image](https://github.com/user-attachments/assets/636763ca-ce47-409a-adc1-7626e8a16c38)

教务-校园公告设置
![image](https://github.com/user-attachments/assets/cb9f2458-1f86-4cdc-89cf-85743239b671)

## **教师功能界面**

老师成绩录入画面：
![image](https://github.com/user-attachments/assets/c1359a05-6d59-489c-ab5c-766369db25a7)

老师课程查看：
![image](https://github.com/user-attachments/assets/e6cd23dc-0a00-4491-a0cb-c652aebc2826)

老师-学生信息
![image](https://github.com/user-attachments/assets/bddb4e14-0569-4ed6-9166-1aba61cade1c)


老师-修改密码
![image](https://github.com/user-attachments/assets/280ae3d4-526e-4bc6-a68f-54251e9ff048)

## **学生功能界面**

学生成绩显示画面：
![image](https://github.com/user-attachments/assets/bb218755-c446-4e38-b442-7c9d38618b56)


学生查看已选课程
![image](https://github.com/user-attachments/assets/30e94729-72d0-4f07-85c0-7f4094cbd581)

学生选退课：
![image](https://github.com/user-attachments/assets/21b4a768-dae5-4de1-bf81-066bb2d80784)

学生-通知查看
![image](https://github.com/user-attachments/assets/ba03bf90-594a-4c9d-8b3d-251a754545e4)

## **数据库设计：**
![image](https://github.com/user-attachments/assets/be1c5908-9413-4eb0-8d27-64d45fb8baef)
![image](https://github.com/user-attachments/assets/99712b88-5592-45ea-bbe6-c6a8fc06727f)
![image](https://github.com/user-attachments/assets/d2fe5c48-7358-4f54-8334-9225c3fb7670)
![image](https://github.com/user-attachments/assets/57010c7a-0255-40e8-9570-44ef2a298416)
![image](https://github.com/user-attachments/assets/4424bb2d-5a6e-4852-9d48-b4629078c110)
![image](https://github.com/user-attachments/assets/643414ec-f4f0-4fbd-9ed2-f718de11215e)
![image](https://github.com/user-attachments/assets/85b5cd34-a171-47e7-b170-eb1e2678376b)
![image](https://github.com/user-attachments/assets/958e55c9-3798-45bf-993b-0b4b5fb030d0)
![image](https://github.com/user-attachments/assets/b90b7ffa-aabd-4e40-a64e-2323ba8cfa04)
![image](https://github.com/user-attachments/assets/3803f8d1-7b11-4108-abaa-54264f76c8f8)
![image](https://github.com/user-attachments/assets/29dd2ee3-fa3c-46db-a367-c9d9eca6501d)

数据库连接代码：

DB=QSqlDatabase::addDatabase("QODBC");

`    `DB.setDatabaseName("managesystem");//此处需要与ODBC中的数据源名称一致

`    `DB.setHostName("localhost");   //我连到了名字为main数据的库中，这个由odbc中的database:main决定

`    `DB.setPort(13306);

`    `DB.setUserName("root");

`    `DB.setPassword("271828");

`    `if(!DB.open()){

`        `QMessageBox::warning(this,"错啦！","数据库打开失败！:"+DB.lastError().text());

`    `}

`    `else{

`        `qDebug()<<"okk";

`    `}


编码说明：

项目组成：一个带ui的登录界面；3个带ui界面的c++用户角色类（管理员、教师、学生）；一个QSS渲染文件，一个资源文件res.qrc以及图标文件夹image

每个用户角色类中，由以下几个部分组成

1\.数据库，表模型，选择模型，数据映射定义

QSqlDatabase DB\_i;

`    `QSqlRelationalTableModel\*tabModel\_i;

`    `QItemSelectionModel\*selModel\_i;

`    `QDataWidgetMapper\*dataMapper\_i;

2\.自定义代理

`    `TComboBoxDelegate delegateSex;

`    `TComboBoxDelegate delegateDepart;

3\.功能类函数：打开表，获取字段名，显示在窗口底端显示记录总数量

void openTable();

`    `void getFeildNames();

void showRecordCount();

4\.用于selectmodel的自定义槽函数

void do\_currentChanged(const QModelIndex &current, const QModelIndex &previous);

void do\_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

5\.界面按钮对应的槽函数，如

void on\_actOpenRecAppend\_clicked();

void on\_actRecInsert\_clicked();

等

主要的功能定义在了opentable（）函数中，该函数在进入相应用户界面后立即调用


项目展望：

由于时间仓促，项目的实际使用时还有许多不足，如添加记录时不会自动将页面滚动至底端；虽然没有达到最后自己所想的预期，不过通过本次经历也学习到了很多知识。总的来说，本次课程设计，收获颇多，不仅储备了自己的知识，还学习到了许多新知识，对今后的学习与应用有了进一步的学习。


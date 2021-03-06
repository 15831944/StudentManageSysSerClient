
#ifndef __PROTODEF_H__
#define __PROTODEF_H__

//服务端默认端口号
#define  DefaultPort        6000

//接收数据长度
#define  DefaultRecvMSGLen  2048
//最长接收长度
#define  MaxRecvMSGLen      4098

//发送数据长度 客户端给服务端发送的数据一般不大
#define  DefaultSendMSGLen  2048

//最大科目数
#define MAXSUBJECTSCOUNT 20
//批量最大接收或者发送记录数
#define MAXBATCHREQACKCOUNT 10
//最大操作权限数目
#define MAXAUTHORITBYCOUNT 30
//用户信息最大字段数量
#define MAXUSERINFOFIELDCOUNT 8

//操作权限宏 与服务端对应  登录和注册操作权限，每个玩家都拥有
enum OperPermission
{
	OPER_PER_INVALID                          =    0,
	OPER_PER_START	                          =    0, //有效值起始值
	OPER_PER_LOGIN                            =    1, 
	OPER_PER_REGISTER                         =    2,

	OPER_PER_ADDBATCHSCOREBYONESUBJECT        =    3, //单科批量增加成绩 -- 不是班级性也可以 （管理员和老师）
	OPER_PER_ADDBATCHSCOREBYSUBJECTS          =    4, //现有所有科目（或者多科）批量增加成绩 -- 不是班级性也可以 （管理员和老师）
	OPER_PER_ADDSINGLESCOREBYONESUBJECT       =    5, //单科单条增加成绩 （管理员和老师）
	OPER_PER_ADDSINGLESCOREBYSUBJECTS         =    6, //现有所有科目单条增加成绩 （管理员和老师）
	OPER_PER_UPDATEBATCHSCOREBYONESUBJECT     =    7, //单科批量更改成绩 -- 班级性，可选哪个班级；操作前，还要先把班级列出来 （管理员和老师）
	OPER_PER_UPDATEBATCHSCOREBYSUBJECTS       =    8, //现有所有科目批量更改成绩 -- 班级性，可选哪个班级 （管理员和老师）
	OPER_PER_UPDATESINGLESCOREBYONESUBJECT    =    9, //单科单条更改成绩 （管理员和老师）
	OPER_PER_UPDATESINGLESCOREBYSUBJECTS      =    10,//现有所有科目单条更改成绩 （管理员和老师）
	OPER_PER_SELECTBATCHSCOREBYONESUBJECT     =    11,//单科批量查询成绩 -- 班级性，可选哪个班级 （管理员和老师）
	OPER_PER_SELECTBATCHSCOREBYSUBJECTS       =    12,//现有所有科目批量查询成绩 -- 班级性，可选哪个班级 （管理员和老师）
	OPER_PER_SELECTSINGLESCOREBYONESUBJECT    =    13,//单科单条查询成绩 （管理员和老师； 学生：学生自己）
	OPER_PER_SELECTSINGLESCOREBYSUBJECTS      =    14,//现有所有科目单条查询成绩 （管理员和老师； 学生：学生自己）
	OPER_PER_DELETEBATCHSCORE			      =    15,//批量删除成绩 -- 通过用户id （管理员和老师）
	OPER_PER_DELETESINGLESCORE		          =    16,//单条删除成绩 （管理员和老师）
	OPER_PER_ALTERADDONESCORESUBJECT		  =    17,//增加成绩科目 （管理员和老师）
	OPER_PER_ALTERDELETEONESCORESUBJECT		  =    18,//删除成绩科目 （管理员和老师）

	OPER_PER_ADDBATCHUSERINFO	              =    19,//批量增加用户信息 -- 不是班级性也可以，固定字段增加 （管理员和老师）
	OPER_PER_ADDSINGLEUSERINFO		          =    20,//单条增加用户信息 -- 固定字段增加 （管理员和老师）
	OPER_PER_UPDATEBATCHUSERINFOBYONE		  =    21,//单个字段批量更改用户信息 -- 班级性，可选哪个班级 （管理员和老师）
	OPER_PER_UPDATEBATCHUSERINFOBYMORE        =    22,//部分固定字段批量更改用户信息 -- 班级性，可选哪个班级；固定字段更改 （管理员和老师）
	OPER_PER_UPDATESINGLEUSERINFOEBYONE       =    23,//单个字段单条更改用户信息 （管理员：学生和老师 ；老师：老师自己，和学生； 学生：学生自己）
	OPER_PER_UPDATESINGLEUSERINFOBYMORE       =    24,//部分固定字段单条更改用户信息 -- 固定字段更改 （管理员：学生和老师； 老师：老师自己，和学生； 学生：学生自己）
	OPER_PER_SELECTBATCHUSERINFO	          =    25,//批量查询用户信息 -- 班级性，可选哪个班级；查询固定字段 （管理员和老师）
	OPER_PER_SELECTSINGLEUSERINFO		      =    26,//单条查询用户信息 -- 查询固定字段 （管理员和老师，学生自己）
	OPER_PER_DELETEBATCHUSERINFO			  =    27,//批量删除用户信息 -- 通过用户id （管理员和老师）
	OPER_PER_DELETESINGLEUSERINFO		      =    28,//单条删除用户信息 （管理员和老师）

	OPER_PER_ADDAUTHORITY					  =    29,//增加某用户一种或者多种权限 （管理员）
	OPER_PER_DELETEAUTHORITY			      =    30,//删除某用户一种或者多种权限 （管理员）

	OPER_PER_END									  //有效值终止值
};

//批量查询成绩可用条件
enum SelectScoreCondition
{
	SELECT_SCORE_CONDITION_INVALID                 =    0,
	SELECT_SCORE_CONDITION_START	               =    0, //有效值起始值
	SELECT_SCORE_CONDITION_RANK     	           =    1, //排序 -- 分为升序和降序
	SELECT_SCORE_CONDITION_RANGE    	           =    2, //分数范围 -- 起始分数结束分数
	SELECT_SCORE_CONDITION_TOTAL    	           =    3, //总分查询
	SELECT_SCORE_CONDITION_AVERAGE    	           =    4, //平均分查询

	SELECT_SCORE_CONDITION_END							   //有效值终止值
};

//排序查询成绩升降序条件
enum SelectScoreRank
{
	SELECT_SCORE_RANK_INVALID                  =    0,
	SELECT_SCORE_RANK_START	                   =    0, //有效值起始值
	SELECT_SCORE_RANK_ASC	                   =    1, //升序
	SELECT_SCORE_RANK_DESC					   =    2, //降序

	SELECT_SCORE_RANK_END						      //有效值终止值
};

//批量查询用户信息可用条件
enum SelectUserInfoCondition
{
	SELECT_USERINFO_INVALID                   =    0,
	SELECT_USERINFO_START	                  =    0, //有效值起始值
	SELECT_USERINFO_RANGE    	              =    1, //用户id范围 -- 起始id结束id
	SELECT_USERINFO_GRADE    	              =    2, //班级查询
	SELECT_USERINFO_PART_NAME    	          =    3, //姓名模糊查询
	SELECT_USERINFO_SEX         	          =    4, //性别查询


	SELECT_USERINFO_END						         //有效值终止值
};

//学科
enum SubjectsType
{
	SUBJECTS_TYPE_INVALID                    =    0,
	SUBJECTS_TYPE_START	                     =    0, //有效值起始值
	SUBJECTS_TYPE_CHINESE                    =    1, //语文
	SUBJECTS_TYPE_MATH                       =    2, //数学
	SUBJECTS_TYPE_ENGLISH                    =    3, //英语
	SUBJECTS_TYPE_PHYSICS                    =    4, //物理
	SUBJECTS_TYPE_CHEMISTRY                  =    5, //化学
	SUBJECTS_TYPE_BIOLOGY                    =    6, //生物
	SUBJECTS_TYPE_HISTORY                    =    7, //历史
	SUBJECTS_TYPE_POLITICS                   =    8, //政治
	SUBJECTS_TYPE_GEOGRAPHY                  =    9, //地理

	SUBJECTS_TYPE_END						         //有效值终止值
};

#if 0
//协议主ID
enum Main_ID
{
	MAIN_ID_INVALID          =   0,
	MAIN_ID_START            =   0, //有效值起始值
	MAIN_ID_LOGINREGISTER    =   1,
	MAIN_ID_SCORE            =   2,
	MAIN_ID_USERINFO         =   3,

	MAIN_ID_END                     //有效值终止值
};
#endif

//协议辅助ID
enum Assist_ID
{
	ASSIST_ID_INVALID								 =   0,
	ASSIST_ID_START									 =   10000, //有效值起始值
	ASSIST_ID_LOGIN_REQ								 =   10001, //登录请求
	ASSIST_ID_LOGIN_ACK								 =   10002, //登录回复
	ASSIST_ID_REGISTER_REQ							 =   10003, //注册请求
	ASSIST_ID_REGISTER_ACK							 =   10004, //注册回复
	ASSIST_ID_EXIT_SYS_REQ							 =   10005, //退出系统 
	ASSIST_ID_GET_SUBJECTS_REQ						 =   10006, //获取剩余和现有科目请求
	ASSIST_ID_GET_SUBJECTS_ACK						 =   10007, //获取剩余和现有科目回复
	ASSIST_ID_ALTER_SUBJECTS_REQ					 =   10008, //增减科目请求
	ASSIST_ID_ALTER_SUBJECTS_ACK					 =   10009, //增减科目回复
	ASSIST_ID_ADD_SINGLE_SCORE_REQ					 =   10010, //单条添加成绩请求
	ASSIST_ID_ADD_SINGLE_SCORE_ACK					 =   10011, //单条添加成绩回复
	ASSIST_ID_SELECT_SINGLE_SCORE_REQ			     =   10012, //单条查询成绩请求
	ASSIST_ID_SELECT_SINGLE_SCORE_ACK			     =   10013, //单条查询成绩回复
	ASSIST_ID_SELECT_BATCH_SCORE_REQ			     =   10014, //批量查询成绩请求
	ASSIST_ID_SELECT_BATCH_SCORE_ACK			     =   10015, //批量查询成绩回复
	ASSIST_ID_UPDATE_SINGLE_SCORE_REQ			     =   10016, //单条更新成绩请求
	ASSIST_ID_UPDATE_SINGLE_SCORE_ACK			     =   10017, //单条更新成绩回复
	ASSIST_ID_DELETE_SCORE_REQ					     =   10018, //删除成绩请求
	ASSIST_ID_DELETE_SCORE_ACK					     =   10019, //删除成绩回复
	ASSIST_ID_ADD_BATCH_SCORE_REQ					 =   10020, //批量添加成绩请求
	ASSIST_ID_ADD_BATCH_SCORE_ACK					 =   10021, //批量添加成绩回复
	ASSIST_ID_GET_AUTHORITY_REQ						 =   10022, //获取对象可删除或可增加的权限请求
	ASSIST_ID_GET_AUTHORITY_ACK						 =   10023, //获取对象可删除或可增加的权限回复
	ASSIST_ID_EDIT_AUTHORITY_REQ					 =   10024, //删除或增加对象权限请求
	ASSIST_ID_EDIT_AUTHORITY_ACK					 =   10025, //删除或增加对象权限回复
	ASSIST_ID_ADD_SINGLE_USERINFO_REQ				 =   10026, //单条增加用户信息请求
	ASSIST_ID_ADD_SINGLE_USERINFO_ACK				 =   10027, //单条增加用户信息回复
	ASSIST_ID_ADD_BATCH_USERINFO_REQ				 =   10028, //批量增加用户信息请求
	ASSIST_ID_ADD_BATCH_USERINFO_ACK				 =   10029, //批量增加用户信息回复
	ASSIST_ID_SELECT_SINGLE_USERINFO_REQ			 =   10030, //单条查询用户信息请求
	ASSIST_ID_SELECT_SINGLE_USERINFO_ACK			 =   10031, //单条查询用户信息回复
	ASSIST_ID_SELECT_BATCH_USERINFO_REQ				 =   10032, //批量查询用户信息请求
	ASSIST_ID_SELECT_BATCH_USERINFO_ACK				 =   10033, //批量查询用户信息回复
	ASSIST_ID_UPDATE_SINGLE_USERINFO_REQ			 =   10034, //单条更新用户信息请求
	ASSIST_ID_UPDATE_SINGLE_USERINFO_ACK			 =   10035, //单条更新用户信息回复
	ASSIST_ID_DELETE_USERINFO_REQ					 =   10036, //删除用户信息请求
	ASSIST_ID_DELETE_USERINFO_ACK					 =   10037, //删除用户信息回复
	ASSIST_ID_NOTIFT_USER_EXIT_SYS_ACK				 =   10038, //通知用户强退出系统 


	ASSIST_ID_END												//有效值终止值
};


//协议头定义
struct HeadData
{
//	int MainID;
	int AssistID;
	int Len;

	HeadData()
	{
		memset(this, 0, sizeof(HeadData));
	}
};

//登录请求   assist[10001]
struct CS_MSG_LOGIN_REQ
{
	char cAccount[31];
	char cPWD[31];
//	int  OperPerId;


	CS_MSG_LOGIN_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_LOGIN_REQ));
	}
};

//登录回复   assist[10002]
struct SC_MSG_LOGIN_ACK
{
	bool bSucceed;
	unsigned int iUserId;
	char cAccount[31];
	char cName[31];
	short sIdent;
	short sSex;
	char cOperPer[150];

	SC_MSG_LOGIN_ACK()
	{
		memset(this, 0, sizeof(CS_MSG_LOGIN_REQ));
	}
};

//注册请求   assist[10003]
struct CS_MSG_REGISTER_REQ
{
	char cName[31];
	char cAccount[31];
	char cPWD[31];
	char cSex[3];
	char cIdent[3]; //只能注册学生和老师。 1学生 2老师
//	int  OperPerId;

	CS_MSG_REGISTER_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_REGISTER_REQ));
	}
};

//注册回复   assist[10004]
struct SC_MSG_REGISTER_ACK
{
	bool bSucceed;
	unsigned int iUserId;
	char cAccount[31];
	char cName[31];
	short sIdent;
	short sSex;
	char cOperPer[150];

	SC_MSG_REGISTER_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_REGISTER_ACK));
	}
};

//退出系统 -- 只是用户不在系统内，但是长连接还在   assist[10005]
struct CS_MSG_EXIT_SYS_REQ
{
	bool bExit;
	CS_MSG_EXIT_SYS_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_EXIT_SYS_REQ));
	}
};

//获取剩余和现有科目请求   assist[10006]
struct CS_MSG_GET_SUBJECTS_REQ
{
	short sGetType; //1剩余科目（未增加）  2现有科目
	CS_MSG_GET_SUBJECTS_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_GET_SUBJECTS_REQ));
	}
};

//获取剩余和现有科目回复   assist[10007]
struct SC_MSG_GET_SUBJECTS_ACK
{
	bool bSucceed;
	short sGetType; //1剩余科目（未添加）  2现有科目
	char cCanAlterSubjects[60];
	SC_MSG_GET_SUBJECTS_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_GET_SUBJECTS_ACK));
	}
};

//增减科目请求   assist[10008]
struct CS_MSG_ALTER_SUBJECTS_REQ
{
	short sGetType; //1增加科目  2删除科目
	short sSubjectId;
	CS_MSG_ALTER_SUBJECTS_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_ALTER_SUBJECTS_REQ));
	}
};

//增减科目回复   assist[10009]
struct SC_MSG_ALTER_SUBJECTS_ACK
{
	bool bSucceed;
	short sGetType;
	short sSubjectId;
	SC_MSG_ALTER_SUBJECTS_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_ALTER_SUBJECTS_ACK));
	}
};

//单条添加成绩请求   assist[10010]
struct CS_MSG_ADD_SINGLE_SCORE_REQ
{
	short sType; //1单科单条增加成绩 2现有所有科目单条增加成绩
	char cAccount[31]; //通过账号添加成绩  如果数据库没有这个账号，就得创建新的账号。所以不能通过userid，userid是自动增加的
	char sSubjectId[60]; //每个ID使用分隔符|隔开
	char sScore[90]; //每科分数使用分隔符|隔开
	CS_MSG_ADD_SINGLE_SCORE_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_ADD_SINGLE_SCORE_REQ));
	}
};

//单条添加成绩回复   assist[10011]
struct SC_MSG_ADD_SINGLE_SCORE_ACK
{
	bool bSucceed;
	short sType; //1单科单条增加成绩 2现有所有科目单条增加成绩
	char cAccount[31];
	SC_MSG_ADD_SINGLE_SCORE_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_ADD_SINGLE_SCORE_ACK));
	}
};

//单条查询成绩请求   assist[10012]
struct CS_MSG_SELECT_SINGLE_SCORE_REQ
{
	short sType; //1单科单条查询成绩 2现有所有科目单条查询成绩
	char cAccount[31]; //通过账号查询成绩  userid是自动增加的，所以不能用userid
	unsigned char sSubjectId[MAXSUBJECTSCOUNT]; //每个科目ID
	unsigned char bSubjectCount; //科目数
	CS_MSG_SELECT_SINGLE_SCORE_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_SELECT_SINGLE_SCORE_REQ));
	}
};

//单条查询成绩回复   assist[10013]
struct SC_MSG_SELECT_SINGLE_SCORE_ACK
{
	short sType; //1单科单条查询成绩 2现有所有科目单条查询成绩
	unsigned int uUserid; //用户id
	char cName[31]; //姓名
	char cAccount[31]; //账号
	char cGrade[31]; //班级
	unsigned char bSubjectId[MAXSUBJECTSCOUNT]; //每个科目ID
	unsigned char bScore[MAXSUBJECTSCOUNT]; //每科分数
	unsigned char bSubjectCount; //科目数
	unsigned char bResCode; //0成功 1失败 2数据库没有账号信息或者没有成绩信息 3其他异常
	SC_MSG_SELECT_SINGLE_SCORE_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_SELECT_SINGLE_SCORE_ACK));
	}
};

//批量查询成绩请求   assist[10014]
struct CS_MSG_SELECT_BATCH_SCORE_REQ
{
	short sType; //1单科批量查询成绩 2现有所有科目批量查询成绩
	char cGrade[31]; //班级 -- 为空则查询所有班级
	unsigned char sSubjectId[MAXSUBJECTSCOUNT]; //每个科目ID
	unsigned char bSubjectCount; //科目数
	unsigned char cCondition[5]; //查询条件，每一个单元是一个条件 
	unsigned char bRankFlag; //0没有排序 1升序 2降序 都是根据总分（没选总分这条件也是这样）排序
	unsigned char bScoreRange[3]; //分数范围，第三个元素为0数据没设置，为1设置数据了
	CS_MSG_SELECT_BATCH_SCORE_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_SELECT_BATCH_SCORE_REQ));
	}
};

//批量查询成绩回复 -- 每次10组数据   assist[10015]
struct SC_MSG_SELECT_BATCH_SCORE_ACK
{
	short sType; //1单科批量查询成绩 2现有所有科目批量查询成绩
	unsigned char cCondition[5]; //查询条件，每一个单元是一个条件  -- 单科批量只有排序和分数范围查询这两种条件
	unsigned char bRankFlag; //0没有排序 1升序 2降序 全科根据总分（没选总分这条件也是这样）排序，单科根据这科目排序
	unsigned int uUserid[MAXBATCHREQACKCOUNT]; //用户id
	char cName[MAXBATCHREQACKCOUNT][31]; //姓名
	char cAccount[MAXBATCHREQACKCOUNT][31]; //账号 
	char cGrade[MAXBATCHREQACKCOUNT][31]; //班级 
	short sRank[MAXBATCHREQACKCOUNT]; //排名
	unsigned short bSum[MAXBATCHREQACKCOUNT]; //总分
	unsigned char bAverage[MAXBATCHREQACKCOUNT]; //平均分
	unsigned char bSubjectId[MAXBATCHREQACKCOUNT][MAXSUBJECTSCOUNT]; //每个科目ID
	unsigned char bScore[MAXBATCHREQACKCOUNT][MAXSUBJECTSCOUNT]; //每科分数
	unsigned char bSubjectCount; //科目数
	unsigned char bDataRecord[3]; //[0]查询结果记录数量<= MAXBATCHREQACKCOUNT； [1]发送序号，从1开始； [2]发送完毕标志 0没有完毕 1完毕
	unsigned char bResCode; //0成功 1失败 2数据库没有账号信息或者没有成绩信息 3其他异常
	SC_MSG_SELECT_BATCH_SCORE_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_SELECT_BATCH_SCORE_ACK));
	}
};

//单条更新成绩请求   assist[10016]
struct CS_MSG_UPDATE_SINGLE_SCORE_REQ
{
	short sType; //1单科单条更新成绩 2现有所有科目单条更新成绩
	char cAccount[31]; //通过账号更改成绩  userid是自动增加的，所以不能用userid
	unsigned char sSubjectId[MAXSUBJECTSCOUNT]; //每个科目ID
	unsigned char bScore[MAXSUBJECTSCOUNT]; //每科分数
	unsigned char bSubjectCount; //科目数
	CS_MSG_UPDATE_SINGLE_SCORE_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_UPDATE_SINGLE_SCORE_REQ));
	}
};

//单条更新成绩回复   assist[10017]
struct SC_MSG_UPDATE_SINGLE_SCORE_ACK
{
	bool bSucceed;
	short sType; //1单科单条更新成绩 2现有所有科目单条更新成绩
	char cAccount[31]; //通过账号更改成绩 
	SC_MSG_UPDATE_SINGLE_SCORE_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_UPDATE_SINGLE_SCORE_ACK));
	}
};

//删除成绩请求   assist[10018]
struct CS_MSG_DELETE_SCORE_REQ
{
	short sType; //1单条删除成绩 2批量删除成绩
	char cAccount[31]; //单条通过账号删除成绩  userid是自动增加的，所以不能用userid
	unsigned int uUserid[3]; //用户id范围，用于批量删除，第三个元素为0数据没设置，为1设置数据了
	CS_MSG_DELETE_SCORE_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_DELETE_SCORE_REQ));
	}
};

//删除成绩回复   assist[10019]
struct SC_MSG_DELETE_SCORE_ACK
{
	bool bSucceed;
	short sType; //1单条删除成绩 2批量删除成绩
	SC_MSG_DELETE_SCORE_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_DELETE_SCORE_ACK));
	}
};

//批量添加成绩请求 -- 每次10组数据    assist[10020]
struct CS_MSG_ADD_BATCH_SCORE_REQ
{
	short sType; //1批量单科增加成绩 2现有所有（或者多科）科目批量增加成绩
	char cAccount[MAXBATCHREQACKCOUNT][31]; //通过账号添加成绩  如果数据库没有这个账号，就得创建新的账号。所以不能通过userid，userid是自动增加的
	unsigned char bSubjectId[MAXBATCHREQACKCOUNT][MAXSUBJECTSCOUNT]; //科目id
	unsigned char bScore[MAXBATCHREQACKCOUNT][MAXSUBJECTSCOUNT]; //科目分数
	unsigned char bSubjectCount; //科目数
	unsigned char bRecordCount; //成绩数据记录数量，要小于等于MAXBATCHREQACKCOUNT
	unsigned char bRecordNO; //发送批次，从1开始
	unsigned char bEnd; //0没有发送完毕 1完毕
	CS_MSG_ADD_BATCH_SCORE_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_ADD_BATCH_SCORE_REQ));
	}
};

//批量添加成绩回复  assist[10021]
struct SC_MSG_ADD_BATCH_SCORE_ACK
{
	bool bSucceed; //客户端接收到返回成功，才继续发剩下的数据给服务端
	short sType; //1批量单科增加成绩 2现有所有科目批量增加成绩
	SC_MSG_ADD_BATCH_SCORE_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_ADD_BATCH_SCORE_ACK));
	}
};

//根据账号获取对象可删除或者可增加的权限请求 assist[10022]
struct CS_MSG_GET_AUTHORITY_REQ
{
	unsigned char sType; //1可删除的 2可增加的
	char cAccount[31]; //操作对象账号
	CS_MSG_GET_AUTHORITY_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_GET_AUTHORITY_REQ));
	}
};

//根据账号获取对象可删除或者可增加的权限回复 assist[10023]
struct SC_MSG_GET_AUTHORITY_ACK
{
	bool bSucceed;
	unsigned char sType; //1可删除的 2可增加的
	char cAccount[31]; //操作对象账号
	unsigned char cAuthority[MAXAUTHORITBYCOUNT]; //操作权限
	unsigned char cAuthorityCount; //权限数量
	SC_MSG_GET_AUTHORITY_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_GET_AUTHORITY_ACK));
	}
};

//根据账号增或删对象权限请求 assist[10024]
struct CS_MSG_EDIT_AUTHORITY_REQ
{
	unsigned char sType; //1删除权限 2增加权限
	char cAccount[31]; //操作对象账号
	unsigned char cAuthority[MAXAUTHORITBYCOUNT]; //增加或删除对象的操作权限
	unsigned char cAuthorityCount; //权限数量
	CS_MSG_EDIT_AUTHORITY_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_EDIT_AUTHORITY_REQ));
	}
};

//根据账号增或删对象权限回复 assist[10025]
struct SC_MSG_EDIT_AUTHORITY_ACK
{
	bool bSucceed;
	unsigned char sType; //1删除权限 2增加权限
	char cAccount[31]; //操作对象账号
	SC_MSG_EDIT_AUTHORITY_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_EDIT_AUTHORITY_ACK));
	}
};

//单条增加用户信息请求   assist[10026]
struct CS_MSG_ADD_SINGLE_USERINFO_REQ
{
	char cAccount[31];
	char cPWD[31];
	char cName[31];
	unsigned char sSex;
	unsigned char sIdent;
	char cMajor[41];
	char cGrade[31];
	CS_MSG_ADD_SINGLE_USERINFO_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_ADD_SINGLE_USERINFO_REQ));
	}
};

//单条增加用户信息回复   assist[10027]
struct SC_MSG_ADD_SINGLE_USERINFO_ACK
{
	bool bSucceed;
	char cAccount[31];
	SC_MSG_ADD_SINGLE_USERINFO_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_ADD_SINGLE_USERINFO_ACK));
	}
};

//批量增加用户信息请求 -- 每次10组数据   assist[10028]
struct CS_MSG_ADD_BATCH_USERINFO_REQ
{
	char cAccount[MAXBATCHREQACKCOUNT][31];
	char cPWD[MAXBATCHREQACKCOUNT][31];
	char cName[MAXBATCHREQACKCOUNT][31];
	unsigned char sSex[MAXBATCHREQACKCOUNT];
	unsigned char sIdent[MAXBATCHREQACKCOUNT];
	char cMajor[MAXBATCHREQACKCOUNT][41];
	char cGrade[MAXBATCHREQACKCOUNT][31];
	unsigned char bRecordCount; //当次数据记录数量，要小于等于MAXBATCHREQACKCOUNT
	unsigned char bRecordNO; //发送批次，从1开始
	unsigned char bEnd; //0没有发送完毕 1完毕
	CS_MSG_ADD_BATCH_USERINFO_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_ADD_BATCH_USERINFO_REQ));
	}
};

//批量增加用户信息回复 -- 每次10组数据   assist[10029]
struct SC_MSG_ADD_BATCH_USERINFO_ACK
{
	bool bSucceed; //客户端接收到返回成功，才继续发剩下的数据给服务端 --如果当次全部添加失败，就返回失败，否则还是成功
	unsigned char bSucceedRecordCount; //本次成功增加到数据库的数量，要小于等于MAXBATCHREQACKCOUNT
	SC_MSG_ADD_BATCH_USERINFO_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_ADD_BATCH_USERINFO_ACK));
	}
};

//单条查询用户信息请求   assist[10030]
struct CS_MSG_SELECT_SINGLE_USERINFO_REQ
{
	char cAccount[31]; //账号  
	unsigned char uRecordDataTmp; // 0不需要记录临时数据 1记录被查询玩家身边标识和账号到临时数据
	CS_MSG_SELECT_SINGLE_USERINFO_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_SELECT_SINGLE_USERINFO_REQ));
	}
};

//单条查询用户信息回复   assist[10031]
struct SC_MSG_SELECT_SINGLE_USERINFO_ACK
{
	unsigned uUserID;
	char cAccount[31];
	char cPWD[31]; //查询的要管理员才有权限看密码
	char cName[31];
	unsigned char sSex;
	unsigned char sIdent;
	char cMajor[41];
	char cGrade[31];
	unsigned char bResCode; //0成功 1失败 2数据库没有账号信息 3身份标识不符合查询不了该用户 4其他异常
	SC_MSG_SELECT_SINGLE_USERINFO_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_SELECT_SINGLE_USERINFO_ACK));
	}
};

//批量查询用户信息请求   assist[10032]
struct CS_MSG_SELECT_BATCH_USERINFO_REQ
{
	unsigned char cCondition[5]; //查询条件，每一个单元是一个条件 
	char cGrade[31]; //班级 查询
	char cName[31]; //姓名模糊查询
	unsigned char cSex; //性别查询
	unsigned uUserIdRange[3]; //用户id范围查询，第三个元素为0数据没设置，为1设置数据了
	CS_MSG_SELECT_BATCH_USERINFO_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_SELECT_BATCH_USERINFO_REQ));
		cSex = 255;
	}
};

//批量查询用户信息回复 -- 每次返回10条   assist[10033]
struct SC_MSG_SELECT_BATCH_USERINFO_ACK
{
	unsigned uUserID[MAXBATCHREQACKCOUNT];
	char cAccount[MAXBATCHREQACKCOUNT][31];
	char cPWD[MAXBATCHREQACKCOUNT][31]; //查询的要管理员才有权限看密码
	char cName[MAXBATCHREQACKCOUNT][31];
	unsigned char sSex[MAXBATCHREQACKCOUNT];
	unsigned char sIdent[MAXBATCHREQACKCOUNT];
	char cMajor[MAXBATCHREQACKCOUNT][41];
	char cGrade[MAXBATCHREQACKCOUNT][31];
	unsigned char bDataRecord[3]; //[0]查询结果记录数量<= MAXBATCHREQACKCOUNT； [1]发送序号，从1开始； [2]发送完毕标志 0没有完毕 1完毕
	unsigned char bResCode; //0成功 1失败 2数据库没有账号信息 3身份标识不符合查询不了该用户 4其他异常
	SC_MSG_SELECT_BATCH_USERINFO_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_SELECT_BATCH_USERINFO_ACK));
	}
};

//单条更新用户信息请求   assist[10034]
struct CS_MSG_UPDATE_SINGLE_USERINFO_REQ
{
	short sType; //1单字段单条更新用户信息 2固定字段单条更新用户信息
	char cAccount[31]; //通过账号更改用户信息 
	unsigned char sUserInfoField[MAXUSERINFOFIELDCOUNT]; //用户信息字段id
	char cUserInfoValue[MAXUSERINFOFIELDCOUNT][41];
	unsigned char bUserInfoFieldCount; //用户信息字段数
	CS_MSG_UPDATE_SINGLE_USERINFO_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_UPDATE_SINGLE_USERINFO_REQ));
	}
};

//单条更新用户信息回复   assist[10035]
struct SC_MSG_UPDATE_SINGLE_USERINFO_ACK
{
	bool bSucceed;
	short sType; //1单字段单条更新用户信息 2固定字段单条更新用户信息
	char cAccount[31]; //通过账号更改用户信息
	SC_MSG_UPDATE_SINGLE_USERINFO_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_UPDATE_SINGLE_USERINFO_ACK));
	}
};

//删除用户信息请求   assist[10036]
struct CS_MSG_DELETE_USERINFO_REQ
{
	short sType; //1单条删除用户信息 2批量删除用户信息
	char cAccount[31]; //单条通过账号删除用户信息 
	unsigned int uUserid[3]; //用户id范围，用于批量删除，第三个元素为0数据没设置，为1设置数据了
	CS_MSG_DELETE_USERINFO_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_DELETE_USERINFO_REQ));
	}
};

//删除用户信息回复   assist[10037]
struct SC_MSG_DELETE_USERINFO_ACK
{
	bool bSucceed;
	short sType; //1单条删除用户信息 2批量删除用户信息
	SC_MSG_DELETE_USERINFO_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_DELETE_USERINFO_ACK));
	}
};

//通知用户强退出系统   assist[10038]
struct SC_MSG_NOTIFT_USER_EXIT_SYS_ACK
{
	bool bExit;
	unsigned char iCode;//0正常通知退出 1账号异地登录被迫强退
	SC_MSG_NOTIFT_USER_EXIT_SYS_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_NOTIFT_USER_EXIT_SYS_ACK));
	}
};

#endif
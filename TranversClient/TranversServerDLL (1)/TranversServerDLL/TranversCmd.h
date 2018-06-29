#pragma once
//终端已经知道公网IP，现在处于打洞阶段（点对点打洞）
#define  TRANVERS_CLIENT_REQ_CMD_S            4001//业务服务器作为打洞客户端的请求命令
#define  TRANVERS_CLIENT_RESP_CMD_S           4002

#define  TRANVERS_CLIENT_REQ_CMD_C            4003//业务终端作为打洞客户端的请求命令
#define  TRANVERS_CLIENT_RESP_CMD_C           4004

#define  TRANVERS_SERVER_NOTIFY_CLINET_CMD    4005 //打洞服务器 转发 业务终端发来的消息

#define  TRANVERS_CLIENT_TO_CLINET_REQ_CMD    4007
#define  TRANVERS_CLIENT_TO_CLINET_RESP_CMD   4008


struct  TranversClientReqCmd_S
{
	int              dominId;
	std::string    serverType;
	int Seralize(char* buff, size_t len)
	{
		return 0;
	}
	int UnSeralize(const char * buff, size_t len)
	{
		return 0;
	}
	unsigned int GetRealSize()
	{
		return 0;
	}
};

struct  TranversClientRespCmd_S
{
	char            OuterIP[16];
	unsigned short     OuterPort;
	int Seralize(char* buff, size_t len)
	{
		return 0;
	}
	int UnSeralize(const char * buff, size_t len)
	{
		return 0;
	}
	unsigned int GetRealSize()
	{
		return 0;
	}
};

struct  TranversClientReqCmd_C
{
	char            interestIP[16];
	unsigned short  interestPort;

	std::string     userID;//逻辑上不需要，但打日志时会用到
	int             dominId;
	
	int Seralize(char* buff, size_t len)
	{
		return 0;
	}
	int UnSeralize(const char * buff, size_t len)
	{
		return 0;
	}
	unsigned int GetRealSize()
	{
		return 0;
	}
};

typedef   TranversClientReqCmd_C  TranversServerNotifyCmd;

struct  TranversClientToClientRespCmd
{
	int              dominId;
	std::string      nodeId;

	int Seralize(char* buff, size_t len)
	{
		return 0;
	}
	int UnSeralize(const char * buff, size_t len)
	{
		return 0;
	}
	unsigned int GetRealSize()
	{
		return 0;
	}
};
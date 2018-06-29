#pragma once
//�ն��Ѿ�֪������IP�����ڴ��ڴ򶴽׶Σ���Ե�򶴣�
#define  TRANVERS_CLIENT_REQ_CMD_S            4001//ҵ���������Ϊ�򶴿ͻ��˵���������
#define  TRANVERS_CLIENT_RESP_CMD_S           4002

#define  TRANVERS_CLIENT_REQ_CMD_C            4003//ҵ���ն���Ϊ�򶴿ͻ��˵���������
#define  TRANVERS_CLIENT_RESP_CMD_C           4004

#define  TRANVERS_SERVER_NOTIFY_CLINET_CMD    4005 //�򶴷����� ת�� ҵ���ն˷�������Ϣ

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

	std::string     userID;//�߼��ϲ���Ҫ��������־ʱ���õ�
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
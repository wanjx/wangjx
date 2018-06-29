#pragma once
#include <list>
#include <string>

#define  CONFIG_FILE_NAME  "TranversServer.xml"

struct serverInfo
{
	serverInfo():port(0)
	{}

	std::string ip;
	unsigned short port;

	bool operator<(const serverInfo &info) const 
	{
		if( ip < info.ip)
			return true;
		else if(ip > info.ip)
			return false;
		else
		{
			return port < info.port;
		}
	}

	serverInfo & operator=(const serverInfo &src)
	{
		if(this != &src)
		{
			ip   = src.ip;
			port = src.port;
		}

		return *this;
	}

	bool operator==(const serverInfo &src) const
	{
		if(this == &src)
			return true;

		return ip == src.ip && port == src.port;

	}

};

class CConfigParser
{
public:
	CConfigParser();
	~CConfigParser();


	serverInfo     GetServerInfo();
	std::string    GetRouteUrl();
	unsigned short GetRouteCommPort();
	std::string    GetBackManageServerUrl(bool bBest = true);
	bool           ParseConfigFile();

	std::string GetLocalConnIp();
	std::string GetDataBaseIP();
	unsigned short GetDataBasePort();
	std::string    GetDataBaseName();
	std::string    GetDataBaseUser();
	std::string    GetDataBasePWD();
	unsigned short GetDataBaseConnNum();

	//新增
	//李少鹏
	serverInfo     GetTranversServerInfo();

	serverInfo     GetBSServerInfo();
	serverInfo     GetStpRCServerInfo();
	serverInfo     GetMsRCServerInfo();
	serverInfo     GetMsRCServer2Info();
	serverInfo     GetLocalInfo();
	serverInfo     GetPublishInfo();
	serverInfo     GetLocalDHTInfo();
	GMUint128      GetNodeId();
	int            GetUpQuota();
	int            GetDownQuota();
	int            GetConnNumQuota();

	//X1新增
	int            GetHostConnNumQuota();
	serverInfo     GetHostServer();
	serverInfo     GetPublishHostServer();

	bool           GetStpRCServerList(std::list<serverInfo> & serverList);
	bool           GetMsRCServerList(std::list<serverInfo> & serverList);

	serverInfo     GetRobotInfo();
	AsynModel::ConnID  GetRobotConnId();

	//N8极会议新增
	std::string    GetEnterpriseCenterServerUrl();

	//2015.12.2
	int               GetIsNeedMCU();
	serverInfo        GetMCUInfo();
	AsynModel::ConnID GetMCUConnId();
	int               GetIsNeedUniformUserCenter();

	//2016.3.9
	unsigned short     GetStartTcpPort();
	unsigned short     GetEndTcpPort();

	//2016.3.10
	unsigned short     GetStartPostMsgPort();
	unsigned short     GetEndPostMsgPort();

	//2016.3.14 
	std::string   GetSecondBMServer();
	void          SetSecondBMServer(std::string& BMServer);
	std::string   GetBestBMServer();
	void          SetBestBMServer(std::string& BMServer);

	std::string    GetOriginalBackManageServerUrl();
	//获取 monitro tools ip port
	serverInfo GetMonitorServerInfo();

private:

	std::string    m_routeUrl;
	std::string    m_backManageServerUrl;

	serverInfo      m_localServer;
	serverInfo      m_publishServer;
	serverInfo      m_localDHTServer;
	unsigned short  m_routeCommPort;

	//数据库相关
	std::string m_LocalIP ;
	std::string m_DataBaseIP;
	unsigned short m_DataBasePort;

	std::string m_DataBaseName;
	std::string m_DataBaseUser;
	std::string m_DataBasePwd;

	unsigned short  m_DataBaseConnNum;

	//新增
	//李少鹏
	//增加对TranversServer.xml的解析
	serverInfo	  m_TranversServer;

	serverInfo     m_bsServer;
	serverInfo     m_stpRcServer;
	serverInfo     m_msRcServer;
	serverInfo     m_msRcServer2;
	serverInfo     m_MonitorServer;   //监听server ip port
	GMUint128      m_nodeId;

	int            m_upQuota;
	int            m_downQuota; 
	int            m_connNumQuota;

	//新增
	std::list<serverInfo>  m_stpRcServerList;
	std::list<serverInfo>  m_msRcServerList;

	serverInfo             m_robot;
	AsynModel::ConnID      m_robotConnId;

	serverInfo             m_hostServer;
	serverInfo             m_hostPublishServer;
	int                    m_hostConnNumQuota;

	//新增
	std::string            m_enterpriseCenterServerUrl;

	//2015.12.3
	int                    m_isNeedMCU; //是否部署MCU服务器，0表示没部署，1表示部署
	serverInfo             m_MCUServer;
	AsynModel::ConnID      m_MCUConnId; 
	
	//2015.12.19
	int                    m_isNeedUniformUserCenter; //是否连接个人用户中心 0表示连接企业用户中心 1表示连接个人用户中心 默认企业用户中心 

	//2016.3.9
	unsigned short         m_startTcpPort;
	unsigned short         m_endTcpPort;

	//2016.3.10
	unsigned short         m_startPostMsgPort;
	unsigned short         m_endPostMsgPort;

	//2016.3.14
	std::string            m_bestBMServer;
	std::string            m_secondBMServer;


};
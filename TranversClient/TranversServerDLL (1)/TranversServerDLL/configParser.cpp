#include "stdafx.h"
#include "configParser.h"
#include "GMFilesystemutility.h"
#include "GMMarkupSTL.h"


CConfigParser::CConfigParser()
{

}
CConfigParser::~CConfigParser()
{

}

serverInfo  CConfigParser::GetServerInfo()
{
	return m_localServer;
}
std::string    CConfigParser::GetRouteUrl()
{
	return m_routeUrl;
}
unsigned short CConfigParser::GetRouteCommPort()
{
	return m_routeCommPort;
}
std::string    CConfigParser::GetBackManageServerUrl(bool bBest)
{
	if(bBest)
		return m_bestBMServer;
	else
		return m_secondBMServer;
}

std::string CConfigParser::GetLocalConnIp()
{
	return m_LocalIP;

}

std::string CConfigParser::GetDataBaseIP()
{
	return m_DataBaseIP;

}
unsigned short CConfigParser::GetDataBasePort()
{
	return m_DataBasePort;

}
std::string    CConfigParser::GetDataBaseName()
{
	return m_DataBaseName;

}
std::string    CConfigParser::GetDataBaseUser()
{

	return m_DataBaseUser;
}
std::string    CConfigParser::GetDataBasePWD()
{

	return m_DataBasePwd;
}
unsigned short CConfigParser::GetDataBaseConnNum()
{
	return m_DataBaseConnNum;

}
bool  CConfigParser::GetStpRCServerList(std::list<serverInfo> & serverList)
{
	serverList = m_stpRcServerList;
	return true;
}

bool CConfigParser::GetMsRCServerList(std::list<serverInfo> & serverList)
{
	serverList = m_msRcServerList;
	return true;
}
bool  CConfigParser::ParseConfigFile()
{
	GMMarkupSTL markup;
	std::string szPath;
	GMFileSystemUtility::getModulePath( szPath );

#ifdef WIN32
	szPath += "\\";
#else
	szPath += "/";
#endif

	szPath += CONFIG_FILE_NAME;

	bool succ = markup.Load(szPath.c_str());
	if(!succ)
	{
		////Log::writeError(MEETING_CTRL_LOG_ID, 70, "文件%s不存在或格式不正确--CConfigParser::parserConfig", szPath);
		return false;
	}

	markup.ResetMainPos();
	int count = 0;
	if(markup.FindChildElem("TranversServerInfo"))
	{
		markup.IntoElem();

		if(markup.FindChildElem("Ip"))
		{
			m_TranversServer.ip = markup.GetChildData();
		}
		if(markup.FindChildElem("Port"))
		{
			m_TranversServer.port = atoi(markup.GetChildData().c_str());
		}

		markup.OutOfElem();
	}
	if(markup.FindChildElem("LocalServerInfo"))
	{
		markup.IntoElem();

		if(markup.FindChildElem("LocalIp"))
		{
			m_localServer.ip = markup.GetChildData();
		}
		if(markup.FindChildElem("LocalPort"))
		{
			m_localServer.port = atoi(markup.GetChildData().c_str());
		}
		if(markup.FindChildElem("RouteCommPort"))
		{
			m_routeCommPort = atoi(markup.GetChildData().c_str());
		}

		if(markup.FindChildElem("PublishIp"))
		{
			m_publishServer.ip = markup.GetChildData();
		}
		if(markup.FindChildElem("PublishPort"))
		{
			m_publishServer.port = atoi(markup.GetChildData().c_str());
		}

		markup.OutOfElem();
	}

	if(markup.FindChildElem("RouteUrl"))
	{
		m_routeUrl  = markup.GetChildData();
	}
	if(markup.FindChildElem("BackManageServerUrl"))
	{
		m_backManageServerUrl = markup.GetChildData().c_str();
		m_bestBMServer        = m_backManageServerUrl;
	}
	//2016.3.14 双接口服务器
	if(markup.FindChildElem("BackManageServerUrl2"))
	{
		std::string  backManageServerUrl2 = markup.GetChildData().c_str();
		m_secondBMServer                  = backManageServerUrl2;
	}
	if(markup.FindChildElem("dataBase"))
	{
		markup.IntoElem();
		if(markup.FindChildElem("localIp"))
		{
			m_LocalIP = markup.GetChildData();
		}
		if(markup.FindChildElem("dataBaseIp"))
		{
			m_DataBaseIP = markup.GetChildData();
		}

		if(markup.FindChildElem("dataBasePort"))
		{
			m_DataBasePort = atoi(markup.GetChildData().c_str());
		}
		if(markup.FindChildElem("dataBaseConnNum"))
		{
			m_DataBaseConnNum = atoi(markup.GetChildData().c_str());
		}
		if(markup.FindChildElem("dataBaseName"))
		{
			m_DataBaseName = markup.GetChildData();
		}
		if(markup.FindChildElem("dataBaseUser"))
		{
			m_DataBaseUser = markup.GetChildData();
		}
		if(markup.FindChildElem("dataBasePWD"))
		{
			m_DataBasePwd = markup.GetChildData();
		}

		markup.OutOfElem();

	}
	if(markup.FindChildElem("LocalDHTServerInfo"))
	{
		markup.IntoElem();


		if(markup.FindChildElem("Ip"))
		{
			m_localDHTServer.ip = markup.GetChildData();
		}
		if(markup.FindChildElem("Port"))
		{
			m_localDHTServer.port = atoi(markup.GetChildData().c_str());
		}

		markup.OutOfElem();
	}

	if(markup.FindChildElem("BSServerInfo"))
	{
		markup.IntoElem();

		if(markup.FindChildElem("Ip"))
		{
			m_bsServer.ip = markup.GetChildData();
		}
		if(markup.FindChildElem("Port"))
		{
			m_bsServer.port = atoi(markup.GetChildData().c_str());
		}

		markup.OutOfElem();
	}

	if(markup.FindChildElem("nodeId"))
	{
		std::string nodeId  = markup.GetChildData();
		m_nodeId.SetValueBE((U8*)nodeId.c_str());
	}

	if(markup.FindChildElem("STPRCServerInfo"))
	{
		markup.IntoElem();

		while(markup.FindChildElem("server"))
		{
			markup.IntoElem();
			serverInfo info;
			if(markup.FindChildElem("Ip"))
			{
				info.ip = markup.GetChildData();
			}
			if(markup.FindChildElem("Port"))
			{
				info.port = atoi(markup.GetChildData().c_str());
			}

			m_stpRcServerList.push_back(info);

			markup.OutOfElem();
		}

		markup.OutOfElem();
	}
	if(markup.FindChildElem("MSRCServerInfo"))
	{
		markup.IntoElem();

		while(markup.FindChildElem("server"))
		{
			markup.IntoElem();
			serverInfo info;
			if(markup.FindChildElem("Ip"))
			{
				info.ip = markup.GetChildData();
			}
			if(markup.FindChildElem("Port"))
			{
				info.port = atoi(markup.GetChildData().c_str());
			}

			m_msRcServerList.push_back(info);

			markup.OutOfElem();
		}

		markup.OutOfElem();
	}


	if(markup.FindChildElem("MSRCServerInfo"))
	{
		markup.IntoElem();

		if(markup.FindChildElem("Ip"))
		{
			m_msRcServer.ip = markup.GetChildData();
		}
		if(markup.FindChildElem("Port"))
		{
			m_msRcServer.port = atoi(markup.GetChildData().c_str());
		}

		markup.OutOfElem();
	}

	if(markup.FindChildElem("MSRCServerInfo2"))
	{
		markup.IntoElem();

		if(markup.FindChildElem("Ip"))
		{
			m_msRcServer2.ip = markup.GetChildData();
		}
		if(markup.FindChildElem("Port"))
		{
			m_msRcServer2.port = atoi(markup.GetChildData().c_str());
		}

		markup.OutOfElem();
	}

	if(markup.FindChildElem("RESInfo"))
	{
		markup.IntoElem();

		if(markup.FindChildElem("upQuota"))
		{
			m_upQuota = atoi(markup.GetChildData().c_str());
		}
		if(markup.FindChildElem("downQuota"))
		{
			m_downQuota = atoi(markup.GetChildData().c_str());
		}
		if(markup.FindChildElem("connNumQuota"))
		{
			m_connNumQuota = atoi(markup.GetChildData().c_str());
		}

		if(markup.FindChildElem("hostConnNumQuota"))
		{
			m_hostConnNumQuota = atoi(markup.GetChildData().c_str());
		}
		markup.OutOfElem();
	}

	if(markup.FindChildElem("robotInfo"))
	{
		markup.IntoElem();

		if(markup.FindChildElem("Ip"))
		{
			m_robot.ip = markup.GetChildData();
		}
		if(markup.FindChildElem("Port"))
		{
			m_robot.port = atoi(markup.GetChildData().c_str());
		}
		markup.OutOfElem();

		m_robotConnId.hosttype = AsynModel::ConnID::CONNID_OPEN_;
		memcpy(m_robotConnId.openhost.openUDPIP,m_robot.ip.c_str(),16);
		m_robotConnId.openhost.openUdpPort = m_robot.port;
	}

	if(markup.FindChildElem("hostInfo"))
	{
		markup.IntoElem();

		if(markup.FindChildElem("Ip"))
		{
			m_hostServer.ip = markup.GetChildData();
		}
		if(markup.FindChildElem("Port"))
		{
			m_hostServer.port = atoi(markup.GetChildData().c_str());
		}
		markup.OutOfElem();
	}

	if(markup.FindChildElem("hostPublishInfo"))
	{
		markup.IntoElem();

		if(markup.FindChildElem("Ip"))
		{
			m_hostPublishServer.ip = markup.GetChildData();
		}
		if(markup.FindChildElem("Port"))
		{
			m_hostPublishServer.port = atoi(markup.GetChildData().c_str());
		}
		markup.OutOfElem();
	}
	if(markup.FindChildElem("isNeedMCU"))
	{
		m_isNeedMCU = atoi(markup.GetChildData().c_str());
	}
	if(markup.FindChildElem("MCUInfo"))
	{
		markup.IntoElem();

		if(markup.FindChildElem("Ip"))
		{
			m_MCUServer.ip = markup.GetChildData();
		}
		if(markup.FindChildElem("Port"))
		{
			m_MCUServer.port = atoi(markup.GetChildData().c_str());
		}
		markup.OutOfElem();
		m_MCUConnId.hosttype = AsynModel::ConnID::CONNID_OPEN_;
		memcpy(m_MCUConnId.openhost.openUDPIP,m_MCUServer.ip.c_str(),16);
		m_MCUConnId.openhost.openUdpPort = m_MCUServer.port;

	}
	if(markup.FindChildElem("isNeedUniformUserCenter"))
	{
		m_isNeedUniformUserCenter = atoi(markup.GetChildData().c_str());
	}
	if(markup.FindChildElem("EnterpriseCenterServerUrl"))
	{
		m_enterpriseCenterServerUrl = markup.GetChildData().c_str();
	}

	if(markup.FindChildElem("HttpUsePort"))
	{
		markup.IntoElem();

		if(markup.FindChildElem("startPort"))
		{
			m_startTcpPort = atoi(markup.GetChildData().c_str());
		}
		if(markup.FindChildElem("endPort"))
		{
			m_endTcpPort  = atoi(markup.GetChildData().c_str());
		}
		markup.OutOfElem();
	}

	if(markup.FindChildElem("PostMsgUsePort"))
	{
		markup.IntoElem();

		if(markup.FindChildElem("startPort"))
		{
			m_startPostMsgPort = atoi(markup.GetChildData().c_str());
		}
		if(markup.FindChildElem("endPort"))
		{
			m_endPostMsgPort  = atoi(markup.GetChildData().c_str());
		}
		markup.OutOfElem();
	}
	if(markup.FindChildElem("MonitorServerInfo"))
	{
		markup.IntoElem();
		if(markup.FindChildElem("Ip"))
		{
			m_MonitorServer.ip = markup.GetChildData();
		}
		if(markup.FindChildElem("Port"))
		{
			m_MonitorServer.port = atoi(markup.GetChildData().c_str());
		}
		markup.OutOfElem();
	}
	return true;
}

//新增
//李少鹏
serverInfo    CConfigParser::GetTranversServerInfo()
{
	return m_TranversServer;
}

serverInfo     CConfigParser::GetBSServerInfo()
{
	return m_bsServer;
}

serverInfo     CConfigParser::GetStpRCServerInfo()
{
	return m_stpRcServer;

}
serverInfo     CConfigParser::GetMsRCServerInfo()
{
	return m_msRcServer;
}

serverInfo     CConfigParser::GetMsRCServer2Info()
{
	return m_msRcServer2;

}

int            CConfigParser::GetUpQuota()
{
	return m_upQuota;
}
int            CConfigParser::GetDownQuota()
{
	return m_downQuota;
}
int            CConfigParser::GetConnNumQuota()
{
	return m_connNumQuota;
}

serverInfo     CConfigParser::GetLocalInfo()
{
	return m_localServer;
}

serverInfo     CConfigParser::GetPublishInfo()
{
	return m_publishServer;
}

GMUint128      CConfigParser::GetNodeId()
{
	return  m_nodeId;
}
serverInfo     CConfigParser::GetLocalDHTInfo()
{
	printf("m_localDHTServer: ip:%s port:%d\n",m_localDHTServer.ip.c_str(),m_localDHTServer.port);
	return m_localDHTServer;

}

serverInfo     CConfigParser::GetRobotInfo()
{
	return m_robot;
}

AsynModel::ConnID  CConfigParser::GetRobotConnId()
{
	return m_robotConnId;
}

//X1新增
int            CConfigParser::GetHostConnNumQuota()
{
	return m_hostConnNumQuota;

}
serverInfo     CConfigParser::GetHostServer()
{
	return m_hostServer;

}

serverInfo     CConfigParser::GetPublishHostServer()
{
	return m_hostPublishServer;

}

std::string    CConfigParser::GetEnterpriseCenterServerUrl()
{
	return m_enterpriseCenterServerUrl;
}

int CConfigParser::GetIsNeedMCU()
{
	return m_isNeedMCU;
}

serverInfo CConfigParser::GetMCUInfo()
{
	return m_MCUServer;
}

AsynModel::ConnID CConfigParser::GetMCUConnId()
{
	return m_MCUConnId;
}

int CConfigParser::GetIsNeedUniformUserCenter()
{
	return m_isNeedUniformUserCenter;
}

unsigned short     CConfigParser::GetStartTcpPort()
{
	return m_startTcpPort;
}

unsigned short     CConfigParser::GetEndTcpPort()
{
	return m_endTcpPort;
}

unsigned short     CConfigParser::GetStartPostMsgPort()
{
	return m_startPostMsgPort;
}
unsigned short     CConfigParser::GetEndPostMsgPort()
{
	return m_endPostMsgPort;
}

//2016.3.14 
std::string   CConfigParser::GetSecondBMServer()
{
	return m_secondBMServer;
}

void          CConfigParser::SetSecondBMServer(std::string& BMServer)
{
	Log::writeWarning(TRANVERSSERVER_LOG,LOG_TYPE_ID,"设置次优接口服务器：%s --CConfigParser::SetBestBMServer",BMServer.c_str());
	m_secondBMServer = BMServer;
}

std::string CConfigParser::GetBestBMServer()
{
	return m_bestBMServer;
}

void        CConfigParser::SetBestBMServer(std::string& BMServer)
{
	Log::writeWarning(TRANVERSSERVER_LOG,LOG_TYPE_ID,"设置最优接口服务器：%s --CConfigParser::SetBestBMServer",BMServer.c_str());
	m_bestBMServer = BMServer;
}


std::string    CConfigParser::GetOriginalBackManageServerUrl()
{
	return m_backManageServerUrl;
}
serverInfo CConfigParser::GetMonitorServerInfo()
{
	return m_MonitorServer;
}
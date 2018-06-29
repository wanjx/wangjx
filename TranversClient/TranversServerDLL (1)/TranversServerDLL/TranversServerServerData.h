#pragma  once
#include "stdafx.h"


struct	ServerInfo
{
	char					      m_Outip[16];
	U16					      m_Outport;
	std::string                m_servertype;
};

typedef int (*ServerOuterAddrCallBack)(ServerInfo *pinfo);
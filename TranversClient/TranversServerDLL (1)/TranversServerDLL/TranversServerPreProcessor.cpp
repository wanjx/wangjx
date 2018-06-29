#include "StdAfx.h"
#include "TranversServerPreProcessor.h"

GMLock                   g_TranversPreProcLock;
bool                     g_TranversPreProcCanWork = false;

TranversServerPreProcessor::TranversServerPreProcessor(void)
{
	m_pTranversSession = NULL;
}

TranversServerPreProcessor::~TranversServerPreProcessor(void)
{
}


void TranversServerPreProcessor::SetWorkSession(AsynModel::Session* sptr)
{
	m_pTranversSession = sptr;
}

int TranversServerPreProcessor::PreProc(unsigned int destSid,unsigned int srcSid,unsigned int msgID,char * pbuff,
	                                const int len,SptrISender sender,SptrSession& sptrSession)
{
	GMAutoLock<GMLock> al(&g_TranversPreProcLock);
	if(g_TranversPreProcCanWork)
	{
		if(destSid == 0)
		{
			sptrSession.Reset(m_pTranversSession);
		}
	}
	return 0;
}
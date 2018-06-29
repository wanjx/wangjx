#include "StdAfx.h"
#include "TranversClientPreProcessor.h"

GMLock                   g_TranversPreProcLock;
bool                     g_TranversPreProcCanWork = false;

TranversClientPreProcessor::TranversClientPreProcessor(void)
{
	m_pTranversSession = NULL;
}

TranversClientPreProcessor::~TranversClientPreProcessor(void)
{
}


void TranversClientPreProcessor::SetWorkSession(AsynModel::Session* sptr)
{
	m_pTranversSession = sptr;
}

int TranversClientPreProcessor::PreProc(unsigned int destSid,unsigned int srcSid,unsigned int msgID,char * pbuff,
	                                const int len,SptrISender sender,SptrSession& sptrSession)
{
		if(destSid == 0)
		{
			sptrSession.Reset(m_pTranversSession);
		}
	
	return 0;
}
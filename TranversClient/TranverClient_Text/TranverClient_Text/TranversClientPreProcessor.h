#pragma once
#include "iomsgpreprocessor.h"
using namespace AsynModel;

class TranversClientPreProcessor :public IOMsgPreProcessor
{
public:
	TranversClientPreProcessor(void);
	virtual ~TranversClientPreProcessor(void);
	void SetWorkSession(AsynModel::Session* sptr);
	virtual int PreProc(unsigned int destSid,unsigned int srcSid,unsigned int msgID,char * pbuff,
		const int len,SptrISender sender,SptrSession& sptrSession);
private:
	AsynModel::Session* m_pTranversSession;
};
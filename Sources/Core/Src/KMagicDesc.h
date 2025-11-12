#ifndef KMagicDescH
#define	KMagicDescH

#include "KIniFile.h"

class KMagicDesc
{
private:
	KIniFile	m_IniFile;
	char		m_szDesc[128];
public:
	KMagicDesc();
	~KMagicDesc();
	BOOL		Init();
	const char*	GetDesc(void* pData);
	const char* GetDesc2(int nType);
	int	String2MagicID(char * szMagicAttribName);
};

extern KMagicDesc	g_MagicDesc;
#endif

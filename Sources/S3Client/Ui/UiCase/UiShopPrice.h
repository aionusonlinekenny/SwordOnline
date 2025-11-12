// -------------------------------------------------------------------------
//	文件名		：	交易确认框
//	创建者		：	Wooy(Wu yue)
//	创建时间	：	2003-5-28
// -------------------------------------------------------------------------
#ifndef __UiShopPrice_H__
#define __UiShopPrice_H__

#include "../Elem/WndButton.h"
#include "../Elem/WndText.h"
#include "../Elem/WndEdit.h"
#include "../../../core/src/gamedatadef.h"



class KUiShopPrice : protected KWndImage
{
public:
	
	static KUiShopPrice*	OpenWindow(KUiObjAtContRegion* pObj,
			KUiItemBuySelInfo* pPriceInfo);	//打开窗口，返回唯一的一个类对象实例
	static KUiShopPrice*	GetIfVisible();				//如果窗口正被显示，则返回实例指针
	static void			CloseWindow(bool bDestroy);		//关闭窗口
private:
	KUiShopPrice();
	~KUiShopPrice() {}
	int		Initialize();								//初始化
	void	Show();
	void	Hide();
	void	LoadScheme(const char* pScheme);			//载入界面方案
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();

private:
	static KUiShopPrice*	m_pSelf;
	KWndText32			m_ItemName;
	KWndEdit32			m_ItemPrice;
	KWndButton	m_PriceType;
	KWndButton			m_OkBtn;
	KWndButton			m_CancelBtn;

	KUiObjAtContRegion	m_ItemInfo;
	KUiItemBuySelInfo	m_PriceInfo;
};


#endif // __UiTradeConfirm_H__
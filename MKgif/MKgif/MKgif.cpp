#define _CRT_SECURE_NO_WARNINGS 1
#include"UIlib.h"
using namespace DuiLib;
#pragma comment(lib,"DuiLib_ud.lib")


class CDuiFramWnd : public WindowImplBase
{
	virtual LPCTSTR GetWindowClassName() const   
	{
		return _T("DUIwind");
	}    
	virtual CDuiString GetSkinFile()   //��ȡxml�ļ�����             
	{ 
		return _T("duilib.xml"); 
	}   
	virtual CDuiString GetSkinFolder()  // ��ȡxml�ļ�·��          
	{ 
		return _T("");   //�Ѿ����ù�Ĭ��·����������
	}

	virtual void Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("click"))
		{
			CDuiString strName = msg.pSender->GetName();
			if (strName == _T("Btn_close"))
			{
				MessageBox(m_hWnd, _T("�رմ���"), _T("test"), IDOK);
				Close();
			}
			else if (strName == _T("Btn_min"))
			{
				MessageBox(m_hWnd, _T("��С��"), _T("test"), IDOK);
			
			}
		}
		else if (msg.sType == _T("itemselect"))
		{
			CComboBoxUI * pComboSelect = (CComboBoxUI*)m_PaintManager.FindControl(_T("Combo_select"));
			int iSelect = pComboSelect->GetCurSel();
			if (0 == iSelect)
			{
				MessageBox(m_hWnd, _T("ͼƬ"), _T("test"), IDOK);
			}
			else
			{
				MessageBox(m_hWnd, _T("��Ƶ"), _T("test"), IDOK);

			}
		}

	}
};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int
	nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	// ������Դ��Ĭ��·�����˴�����Ϊ��exe��ͬһĿ¼�� 
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CDuiFramWnd framWnd;
	// Cashier���ڴ������Ͻ���ʽ������
	// UI_WNDSTYLE_FRAME: duilib��װ�ĺ꣬�����ڿ��ӣ����б������������С�����رչ��ܵ�
	// WS_EX_WINDOWEDGE: Win32�Ĵ��ڷ�񣬴��б߿�
	framWnd.Create(NULL, _T("Cashier"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	framWnd.CenterWindow();
	//��ʾ���ڣ�������Ϣѭ��
	framWnd.ShowModal();
	return 0;
}

#if 0
class CDuiFramWnd : public CWindowWnd,public INotifyUI
{
public:
	// CWindowWnd��Ĵ��麯�����ڸú����б��뷵���û������崰�ڵ������ƣ�ע�ᴰ��ʱ��Ҫ�õ�
	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("DuiFramWnd");
	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		m_PaintManager.Init(m_hWnd);
		if (WM_CREATE == uMsg)
		{
			m_PaintManager.Init(m_hWnd);

			CDialogBuilder builder;     // duilib.xml��Ҫ�ŵ�exeĿ¼��     
			CControlUI* pRoot = builder.Create(_T("MKgif.xml"), (UINT)0, NULL, &m_PaintManager);      
			m_PaintManager.AttachDialog(pRoot);     
			m_PaintManager.AddNotifier(this);      
			return 0; 
		}
		else if (uMsg == WM_NCACTIVATE)         
		{ 
			if (!::IsIconic(m_hWnd))             
			{ 
				return (wParam == 0) ? TRUE : FALSE; 
			}
		}
		else if (uMsg == WM_NCCALCSIZE)         
		{ 
			return 0; 
		}
		else if (uMsg == WM_NCPAINT)         
		{ 
			return 0;
		}

		LRESULT lRes = 0;
		if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
		{
			return lRes;
		}
		return __super::HandleMessage(uMsg, wParam, lParam);
	}

	virtual void Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("click"))
		{
			MessageBox(m_hWnd, _T("��ť����"), _T("test"), IDOK);
		}

	}
private:
	CPaintManagerUI m_PaintManager;
};
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int
	nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance); 
	// ������Դ��Ĭ��·�����˴�����Ϊ��exe��ͬһĿ¼�� 
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CDuiFramWnd framWnd;
	// Cashier���ڴ������Ͻ���ʽ������
	// UI_WNDSTYLE_FRAME: duilib��װ�ĺ꣬�����ڿ��ӣ����б������������С�����رչ��ܵ�
	// WS_EX_WINDOWEDGE: Win32�Ĵ��ڷ�񣬴��б߿�
	framWnd.Create(NULL, _T("Cashier"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	//��ʾ���ڣ�������Ϣѭ��
	framWnd.ShowModal();
	return 0;
}

# endif
#if 0
class CDuiFramWnd : public WindowImplBase            //�̳��ڿ��еĻ�����
{
protected:
	virtual CDuiString GetSkinFolder()
	{
		return _T("");
	}

	virtual CDuiString GetSkinFile()
	{
		return _T("Mgif.xml");                  //������exe��Ŀ¼�е�.xml�ļ�����ͬ
	}

	virtual LPCTSTR GetWindowClassName(void)const
	{
		return _T("DuiWnd");
	}

	virtual void Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("click"))
		{
			MessageBox(m_hWnd, _T("��ť����"), _T("test"), IDOK);
		}
	}
};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int
	nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	// ������Դ��Ĭ��·�����˴�����Ϊ��exe��ͬһĿ¼��
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	//CPaintManagerUI::SetCurrentPath(CPaintManagerUI::GetInstancePath());
	CDuiFramWnd framWnd;
	// Cashier���ڴ������Ͻ���ʽ������
	// UI_WNDSTYLE_FRAME: duilib��װ�ĺ꣬�����ڿ��ӣ����б������������С�����رչ��ܵ�
	// WS_EX_WINDOWEDGE: Win32�Ĵ��ڷ�񣬴��б߿�
	framWnd.Create(NULL, _T("DuilibTest"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);

	framWnd.CenterWindow();

	//��ʾ���ڣ�������Ϣѭ��
	framWnd.ShowModal();
	return 0;
}

#endif












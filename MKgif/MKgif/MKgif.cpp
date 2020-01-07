#define _CRT_SECURE_NO_WARNINGS 1
#include"UIlib.h"
using namespace DuiLib;
#pragma comment(lib,"DuiLib_ud.lib")
#include<fstream>


class CDuiFramWnd : public WindowImplBase
{
	virtual LPCTSTR GetWindowClassName() const   
	{
		return _T("DUIwind");
	}    
	virtual CDuiString GetSkinFile()   //获取xml文件名字             
	{ 
		return _T("duilib.xml"); 
	}   
	virtual CDuiString GetSkinFolder()  // 获取xml文件路径          
	{ 
		return _T("");   //已经设置过默认路径则不用设置
	}

//消息循环
	virtual void Notify(TNotifyUI& msg)
	{
		CDuiString strName = msg.pSender->GetName();
		if (msg.sType == _T("click"))
		{
			if (strName == _T("Btn_close"))
			{
				MessageBox(m_hWnd, _T("关闭窗口"), _T("test"), IDOK);
				Close();
			}
			else if (strName == _T("Btn_min"))
			{
				MessageBox(m_hWnd, _T("最小化"), _T("test"), IDOK);
			
			}
			else if (strName == _T("Btn_cut"))
			{
				CutView();
			}
			else if (strName == _T("Btn_push"))
			{
				//找到要编辑框控件
				CEditUI* pEdit = (CEditUI*)m_PaintManager.FindControl(_T("Edit_word"));
				//将edit中的文本信息获取到
				CDuiString strWord=pEdit->GetText();
				//找到选中的list控件行
				CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("list_srt"));
				//找到文本被选中的那一项，即文本行
				CListTextElementUI* pListItem = (CListTextElementUI*)pList->GetItemAt(pList->GetCurSel()); //找到当前选中行
				pListItem->SetText(1, strWord);

			}
			else if (strName == _T("Btn_WriteTxt"))
			{
				WriteSRT();
			}
			else if (strName == _T("Btn_CreateGif"))
			{
				//生成GIF
				GenerateGifWitPic();

			}
			else if (strName == _T("Btn_GetTxt"))
			{
				LoadSRT();
			}
		}
		//检查控件被选中
		else if (msg.sType == _T("itemselect"))
		{
			if (strName == _T("list_srt"))
			{
				CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("list_srt"));
				//找到文本被选中的那一项，即文本行
				CListTextElementUI* pListItem = (CListTextElementUI*)pList->GetItemAt(pList->GetCurSel()); //找到当前选中行
				
				//找到要写入的控件
				CEditUI* pEdit=(CEditUI*)m_PaintManager.FindControl(_T("Edit_word"));
				//将list选中的行中的文本信息增加到edit控件中
				pEdit->SetText(pListItem->GetText(1));

			}
			if (strName == _T("Combo_select"))
			{    //选中图片生成，让以下控件失效
				CComboBoxUI* pCountUI =(CComboBoxUI*)m_PaintManager.FindControl(_T("Combo_select"));
				if (0 == pCountUI->GetCurSel())
				{
					((CEditUI*)m_PaintManager.FindControl(_T("edit_strat")))->SetEnabled(false);

				}
				else
				{
					((CEditUI*)m_PaintManager.FindControl(_T("edit_strat")))->SetEnabled(true);

				}
			}
			//通过，名字找控件
			CComboBoxUI * pComboSelect = (CComboBoxUI*)m_PaintManager.FindControl(_T("Combo_select"));
			int iSelect = pComboSelect->GetCurSel();
			if (0 == iSelect)
			{
				//MessageBox(m_hWnd, _T("图片"), _T("test"), IDOK);
				GenerateGifWitPic();
			}
			else
			{
				MessageBox(m_hWnd, _T("视频"), _T("test"), IDOK);
				GenerateGifWithView();
			}
		}

	}
	//给cmd控制台发消息
	void SendMessage(CDuiString strCMD)
	{
		//命令窗口从该结构体中通过成员调用cmd，并将strCMD通过成员传给cmd
		SHELLEXECUTEINFO strSEInfo;
		memset(&strSEInfo, 0, sizeof(SHELLEXECUTEINFO));
		strSEInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		strSEInfo.fMask= SEE_MASK_NOCLOSEPROCESS;
		strSEInfo.lpFile = _T("C:\\Windows\\System32\\cmd.exe");
		strSEInfo.lpParameters = strCMD;
		strSEInfo.nShow = SW_HIDE;

		//给命令行发消息
		ShellExecuteEx(&strSEInfo);
		//MessageBox(NULL, _T("命令操作完成"), _T("MakeGif"), IDOK);

	}

	void GenerateGifWitPic() //图片生成gif
	{
		//构造命令
		//CDuiString strCMD(_T("ffmpeg -r 3 -i .\\Pictrue\\%d.jpg output.gif -y"));
		//CDuiString strCMD(_T("D:\\软件工具\\MakeGif\\MKgif\\Debug\\ffmpeg\\ffmpeg -r 3 -i .\\Pictrue\\%d.jpg output.gif -y"));
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取工程目录，，debug下exe
		strPath += _T("\\ffmpeg\\");
		CDuiString strCMD(_T("/c "));
		strCMD += strPath;
		strCMD += _T("ffmpeg -r 2 -i ");
		strCMD += strPath;
		strCMD += _T(".\\pictrue\\%d.jpg ");
		strCMD += _T("output.gif -y");
		//给CMD发送命令
		SendMessage(strCMD);
	}

	void CutView()
	{
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取工程目录，，debug下exe
		strPath += _T("\\ffmpeg\\");
		CDuiString strCMD(_T("/c "));
		strCMD += strPath;
		strCMD += _T("ffmpeg -i ");
		strCMD += strPath;
		strCMD += _T("input.mkv");
		strCMD += _T(" -vcodec copy -acodec copy -ss ");

		//获取时间
		CDuiString strStartTime=((CEditUI*)m_PaintManager.FindControl(_T("edit_start")))->GetText();
		CDuiString strEndTime = ((CEditUI*)m_PaintManager.FindControl(_T("edit_end")))->GetText();
		strCMD += strStartTime;
		strCMD += _T(" -to ");
		strCMD += strEndTime;
		strCMD += _T(" ");
		//输出所在路径
		strCMD += strPath;
		strCMD += _T("11.mkv -y");

		//发送执行命令
		SendMessage(strCMD);
	}
	void GetSRTFile() //提取出字幕文件
	{
		//ffmpeg -i 11.mkv input.srt -y
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取工程目录，，debug下exe
		strPath += _T("\\ffmpeg\\");
		CDuiString strCMD(_T("/c "));
		strCMD += strPath;
		strCMD += _T("ffmpeg -i ");
		strCMD += strPath;
		strCMD += _T("11.mkv");
		strCMD += strPath;
		strCMD += _T("input.srt -y");

		SendMessage(strCMD);
	}

	void LoadSRT()  //加载字幕文件
	{
		//将srt格式的字幕文件，加载到界面中的list控件中
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取工程目录，，debug下exe
		strPath += _T("\\ffmpeg\\input.srt");

		std::ifstream fIn(strPath.GetData()); //输入流对象
		char strSRTCon[521] = { 0 };
		CListUI* pList=(CListUI*)m_PaintManager.FindControl(__T("list_srt"));
		pList->RemoveAll();
		while (!fIn.eof())
		{
			//读取字幕序号
			fIn.getline(strSRTCon,512);

			//读取时间轴
			CListTextElementUI* pListItem = new CListTextElementUI;
			pList->Add(pListItem);
			fIn.getline(strSRTCon, 512);
			pListItem->SetText(0, UTF8ToUnicode(strSRTCon)); //要将字符转化
			//读取字幕
			fIn.getline(strSRTCon, 512);
			pListItem->SetText(1, UTF8ToUnicode(strSRTCon));

			//读取空行
			fIn.getline(strSRTCon, 512);
		}
		fIn.close();
	}
	void WriteSRT()
	{
		//将srt格式的字幕文件，加载到界面中的list控件中
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取工程目录，，debug下exe
		strPath += _T("\\ffmpeg\\input.srt");
		std::ofstream fout(strPath.GetData()); //输入流对象

		//1.从list控件中获取文本内容
		CListUI* pList = (CListUI*)m_PaintManager.FindControl(_T("list_srt"));
		int szCount=pList->GetCount();
		for (int i = 0; i < szCount; ++i)
		{
			CListTextElementUI* pListItem = (CListTextElementUI*)pList->GetItemAt(i);
			//序号
			CDuiString strNo;
			strNo.Format(_T("%d"), i + 1);

			//时间轴
			CDuiString strTime = pListItem->GetText(0);
			//文本
			CDuiString strWord = pListItem->GetText(1);
			
			//
			string strNewLine = Unicode2ANSI(_T("\n"));
			//写入行号
			string itemNo = Unicode2ANSI(strNo);
			fout.write(itemNo.c_str(),itemNo.size());
			fout.write(strNewLine.c_str(), strNewLine.size());
			//写入时间轴
			string itemTime = Unicode2ANSI(strTime);
			fout.write(itemTime.c_str(), itemNo.size());
			fout.write(strNewLine.c_str(), strNewLine.size());

			//写文本
			string itemWord = Unicode2ANSI(strWord);
			fout.write(itemWord.c_str(), itemNo.size());
			fout.write(strNewLine.c_str(), strNewLine.size());

			//每组字幕之间的空白行
			fout.write(strNewLine.c_str(), strNewLine.size());

		}
		//2.将获取到的内容写到srt文件中
		fout.close();
	}

	void GenerateView() //提取视频裸流
	{
		//ffmpeg - i 11.mkv - vcodec copy - an - sn 22.mkv - y 
		//- an: 表示取消音频    - sn : 表示取消字幕
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取工程目录，，debug下exe
		strPath += _T("\\ffmpeg\\");
		CDuiString strCMD(_T("/c "));
		strCMD += strPath;
		strCMD += _T("ffmpeg -i ");
		strCMD += strPath; 
		strCMD += _T("11.mkv -vcodec copy -an -sn ");
		strCMD += strPath;
		strCMD += _T("22.mkv -y");

		SendMessage(strCMD);
	}

	void BornSRT2View()  //烧录
	{
		//ffmpeg -i 22.mkv -vf subtitles=11.srt 33.mkv -y
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取工程目录，，debug下exe
		strPath += _T("\\ffmpeg\\");
		CDuiString strCMD(_T("/c "));
		strCMD += strPath;
		strCMD += _T("ffmpeg -i ");
		strCMD += strPath;
		strCMD += _T("22.mkv -vf subtitles=11.srt ");
		strCMD += strPath;
		strCMD += _T("33.mkv -y");

		SendMessage(strCMD);
	}

	void GenerateGifWithView() //生成Gif
	{
		//ffmpeg -i 33.mkv -vf scale=iw/2:ih/2 -f gif output.gif -y
		CDuiString strPath = CPaintManagerUI::GetInstancePath();//获取工程目录，，debug下exe
		strPath += _T("\\ffmpeg\\");
		CDuiString strCMD(_T("/c "));
		strCMD += strPath;
		strCMD += _T("ffmpeg -i ");
		strCMD += strPath;
		strCMD += _T("33.mkv  -vf scale=iw/2:ih/2 -f gif ");
		strCMD += strPath;
		strCMD += _T("output.mkv -y");

		SendMessage(strCMD);
	}
	CDuiString UTF8ToUnicode(const char* str)
	{
		//第一次调用，求取目标字符串长度，，该函数需要调用两次
		int szLen = ::MultiByteToWideChar(CP_UTF8, 0, str, strlen(str), nullptr, 0);
		wchar_t* pContent = new wchar_t[szLen + 1];

		//第二次调用进行真正的转换
		::MultiByteToWideChar(CP_UTF8, 0, str, strlen(str), pContent, szLen);
		pContent[szLen] = '\0';
		CDuiString s(pContent);
		delete[] pContent;
		return s;
	}
	string Unicode2ANSI(CDuiString str) //unicode字符转到ascii字符
	{
		int szLen = ::WideCharToMultiByte(CP_ACP, 0, str.GetData(), -1, NULL, 0,NULL, FALSE);
		char* pBuff = new char[szLen + 1];
		::WideCharToMultiByte(CP_ACP, 0, str.GetData(), str.GetLength(), pBuff, szLen,NULL, FALSE);
		pBuff[szLen] = '\0';
		string s(pBuff);
		delete[] pBuff;
		return s;

	}
};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int
	nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	// 设置资源的默认路径（此处设置为和exe在同一目录） 
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CDuiFramWnd framWnd;
	// Cashier即在窗口右上角显式的名字
	// UI_WNDSTYLE_FRAME: duilib封装的宏，代表窗口可视，具有标题栏，最大化最小化，关闭功能等
	// WS_EX_WINDOWEDGE: Win32的窗口风格，带有边框
	framWnd.Create(NULL, _T("Cashier"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	framWnd.CenterWindow();
	//显示窗口，激活消息循环
	framWnd.ShowModal();
	return 0;
}

#if 0
class CDuiFramWnd : public CWindowWnd,public INotifyUI
{
public:
	// CWindowWnd类的纯虚函数，在该函数中必须返回用户所定义窗口的类名称，注册窗口时需要用到
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

			CDialogBuilder builder;     // duilib.xml需要放到exe目录下     
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
			MessageBox(m_hWnd, _T("按钮单击"), _T("test"), IDOK);
		}

	}
private:
	CPaintManagerUI m_PaintManager;
};
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int
	nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance); 
	// 设置资源的默认路径（此处设置为和exe在同一目录） 
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CDuiFramWnd framWnd;
	// Cashier即在窗口右上角显式的名字
	// UI_WNDSTYLE_FRAME: duilib封装的宏，代表窗口可视，具有标题栏，最大化最小化，关闭功能等
	// WS_EX_WINDOWEDGE: Win32的窗口风格，带有边框
	framWnd.Create(NULL, _T("Cashier"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	//显示窗口，激活消息循环
	framWnd.ShowModal();
	return 0;
}

# endif
#if 0
class CDuiFramWnd : public WindowImplBase            //继承于库中的基础类
{
protected:
	virtual CDuiString GetSkinFolder()
	{
		return _T("");
	}

	virtual CDuiString GetSkinFile()
	{
		return _T("Mgif.xml");                  //必须与exe层目录中的.xml文件名相同
	}

	virtual LPCTSTR GetWindowClassName(void)const
	{
		return _T("DuiWnd");
	}

	virtual void Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("click"))
		{
			MessageBox(m_hWnd, _T("按钮单击"), _T("test"), IDOK);
		}
	}
};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int
	nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	// 设置资源的默认路径（此处设置为和exe在同一目录）
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	//CPaintManagerUI::SetCurrentPath(CPaintManagerUI::GetInstancePath());
	CDuiFramWnd framWnd;
	// Cashier即在窗口右上角显式的名字
	// UI_WNDSTYLE_FRAME: duilib封装的宏，代表窗口可视，具有标题栏，最大化最小化，关闭功能等
	// WS_EX_WINDOWEDGE: Win32的窗口风格，带有边框
	framWnd.Create(NULL, _T("DuilibTest"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);

	framWnd.CenterWindow();

	//显示窗口，激活消息循环
	framWnd.ShowModal();
	return 0;
}

#endif












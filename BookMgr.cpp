// BookMgr.cpp : 实现文件
//

#include "stdafx.h"
#include "KZLib.h"
#include "BookMgr.h"
#include "afxdialogex.h"
#include "BookInfoInputDlg.h"
#include <locale>


// CBookMgr 对话框

IMPLEMENT_DYNAMIC(CBookMgr, CDialogEx)

CBookMgr::CBookMgr(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBookMgr::IDD, pParent)
{
	saveFilePath = _T("bookinfo.cdb");//文件路径
}

CBookMgr::~CBookMgr()
{
}

void CBookMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBookMgr, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DEL_BOOK, &CBookMgr::OnBnClickedButtonDelBook)
	ON_BN_CLICKED(IDC_BUTTON_ADDBOOK, &CBookMgr::OnBnClickedButtonAddbook)
	ON_NOTIFY(NM_DBLCLK, IDC_LC_BOOKINFO, &CBookMgr::OnDblclkLcBookinfo)
END_MESSAGE_MAP()


// CBookMgr 消息处理程序


BOOL CBookMgr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	mBookListCtrl = (CListCtrl *)GetDlgItem(IDC_LC_BOOKINFO);

	LONG styles=GetWindowLong(mBookListCtrl->m_hWnd, GWL_STYLE); 
    SetWindowLong(mBookListCtrl->m_hWnd, GWL_STYLE, styles|LVS_REPORT);
	mBookListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CRect rs;
	GetClientRect(&rs);
	int colWidth = rs.Width()/8;	//计算列宽

	mBookListCtrl->InsertColumn(0, _T("书号"), LVCFMT_LEFT, colWidth, 0);
	mBookListCtrl->InsertColumn(1, _T("书籍名称"), LVCFMT_CENTER, colWidth, 1);
	mBookListCtrl->InsertColumn(2, _T("作者"), LVCFMT_CENTER, colWidth, 2);
	mBookListCtrl->InsertColumn(3, _T("出版社"), LVCFMT_CENTER, colWidth, 3);
	mBookListCtrl->InsertColumn(4, _T("出版时间"), LVCFMT_CENTER, colWidth , 4);
	mBookListCtrl->InsertColumn(5, _T("书籍类型"), LVCFMT_CENTER, colWidth , 5);
	mBookListCtrl->InsertColumn(6, _T("持有量"), LVCFMT_CENTER, colWidth , 6);
	mBookListCtrl->InsertColumn(7, _T("价格"), LVCFMT_LEFT, colWidth, 7);

	CStdioFile file;	//read info from file

	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL)); 
	BOOL open = file.Open(saveFilePath, CFile::modeRead);//初始化时将txt中书籍信息读出(只读模式)
	file.Seek(0, CFile::begin);
	if(open)
	{
		setlocale( LC_CTYPE, ("chs")); 
		BOOL flag = TRUE;
		//int index = 0;
		while(flag)
		{
			CString buf;
			file.ReadString(buf);
			if(buf.GetLength() == 0)
			{
				flag = FALSE;
				break;
			}
			BookInfo info;
			info.mCode = buf;
			file.ReadString(info.mName);
			file.ReadString(info.mAuthor);
			file.ReadString(info.mPublish);
			file.ReadString(info.mPublishTime);
			file.ReadString(info.mType);
			file.ReadString(buf);
			info.mKeepCount = _ttoi(buf);
			info.mPrice = _ttoi(buf);
			AddBookInfo(info);
			break;
		}

		setlocale(LC_CTYPE, old_locale); 
		free(old_locale); 
		file.Close();
	}
	
	return TRUE; 
}


void CBookMgr::OnBnClickedButtonDelBook()//删除信息
{
	
	int clkMark = MessageBox(_T("确定要删除吗"), _T("提示"),MB_OKCANCEL);
	if(clkMark == IDOK)
	{
		int selMark = mBookListCtrl->GetSelectionMark();
		if(selMark<0){
			MessageBox(_T("请先选中要删除的条目！"), _T("提示"), MB_OK);
			return;
		}
		mBookListCtrl->DeleteItem(selMark);
		POSITION p = ((CKZLibApp *)AfxGetApp())->bookList.GetHeadPosition();
		for(int i=0; i<selMark; i++)
		{
			((CKZLibApp *)AfxGetApp())->bookList.GetNext(p);
		}
		((CKZLibApp *)AfxGetApp())->bookList.RemoveAt(p);
	}
}


void CBookMgr::OnBnClickedButtonAddbook()//添加信息按钮
{
	// TODO: 在此添加控件通知处理程序代码
	BookInfoInputDlg inputDlg;
	int addRes = inputDlg.DoModal();
	if(addRes == IDOK){
		AddBookInfo(inputDlg.mInfo);
	}
}


int CBookMgr::AddBookInfo(BookInfo bInfo)//添加信息函数
{
	int index = mBookListCtrl->GetItemCount();
	int addRow = mBookListCtrl->InsertItem(index, _T(""));

	mBookListCtrl->SetItemText(addRow, 0, bInfo.mCode);
	mBookListCtrl->SetItemText(addRow, 1, bInfo.mName);
	mBookListCtrl->SetItemText(addRow, 2, bInfo.mAuthor);
	mBookListCtrl->SetItemText(addRow, 3, bInfo.mPublish);
	mBookListCtrl->SetItemText(addRow, 4, bInfo.mPublishTime);
	mBookListCtrl->SetItemText(addRow, 5, bInfo.mType);
	CString countStr,priceStr;
	countStr.Format(_T("%d"), bInfo.mKeepCount);
	priceStr.Format(_T("%d"), bInfo.mPrice);
	mBookListCtrl->SetItemText(addRow, 6, countStr);
	mBookListCtrl->SetItemText(addRow, 7, priceStr);

	((CKZLibApp *)AfxGetApp())->bookList.AddTail(bInfo);

	return addRow;
}


void CBookMgr::OnDblclkLcBookinfo(NMHDR *pNMHDR, LRESULT *pResult)//双击listcontrol弹出书籍信息修改Dlg
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	NMLISTVIEW* p = (NMLISTVIEW *)pNMHDR;
	int itemRow = p->iItem;		//获取选中的行
	if(itemRow == -1){
		return;
	}

	BookInfoInputDlg editDlg;
	//set edit info
	editDlg.isEditMode = TRUE;
	editDlg.mInfo.mCode = mBookListCtrl->GetItemText(itemRow, 0);
	editDlg.mInfo.mName = mBookListCtrl->GetItemText(itemRow, 1);
	editDlg.mInfo.mAuthor = mBookListCtrl->GetItemText(itemRow, 2);
	editDlg.mInfo.mPublish = mBookListCtrl->GetItemText(itemRow, 3);
	editDlg.mInfo.mKeepCount = _ttoi( mBookListCtrl->GetItemText(itemRow, 4));
	
	int editRes = editDlg.DoModal();
	CString countStr, priceStr;
	if(editRes == IDOK) {
		mBookListCtrl->SetItemText(itemRow, 0, editDlg.mInfo.mCode);
		mBookListCtrl->SetItemText(itemRow, 1, editDlg.mInfo.mName);
		mBookListCtrl->SetItemText(itemRow, 2, editDlg.mInfo.mAuthor);
		mBookListCtrl->SetItemText(itemRow, 3, editDlg.mInfo.mPublish);
		mBookListCtrl->SetItemText(itemRow, 4, editDlg.mInfo.mPublishTime);
		mBookListCtrl->SetItemText(itemRow, 5, editDlg.mInfo.mType);
	
		countStr.Format(_T("%d"), editDlg.mInfo.mKeepCount);
		priceStr.Format(_T("%d"), editDlg.mInfo.mPrice);
		mBookListCtrl->SetItemText(itemRow, 6, countStr);
		mBookListCtrl->SetItemText(itemRow, 7, priceStr);
		

		POSITION p = ((CKZLibApp *)AfxGetApp())->bookList.GetHeadPosition();
		for(int i=0; i<itemRow; i++){
			((CKZLibApp *)AfxGetApp())->bookList.GetNext(p);
		}
		((CKZLibApp *)AfxGetApp())->bookList.SetAt(p, editDlg.mInfo);//将指定索引位置上的现有字符替换为其他字符 
	}
	*pResult = 0;
}



void CBookMgr::PostNcDestroy()//将增删改查操作只需在list中完成，最后将修改后的list数据写入txt
{
	//write to file
	CStdioFile file;//默认为Text模式，避免换行
	CString keepCount, Price;
	file.Open(saveFilePath, CFile::modeCreate|CFile::modeWrite);
	setlocale(LC_CTYPE, ("chs"));//序列化 book的结构体为cstring成员
	POSITION p = ((CKZLibApp *)AfxGetApp())->bookList.GetHeadPosition();//得到头索引
	while(p){
		BookInfo info = ((CKZLibApp *)AfxGetApp())->bookList.GetNext(p);//遍历booklist
		file.WriteString(info.mCode+"\n");
		file.WriteString(info.mName+"\n" );
		file.WriteString(info.mAuthor+ "\n");
		file.WriteString(info.mPublish+"\n");
		file.WriteString(info.mPublishTime+"\n" );
		file.WriteString(info.mType +"\n");
		keepCount.Format(_T("%d"), info.mKeepCount);
		Price.Format(_T("%d"), info.mPrice);
		file.WriteString(keepCount + "\n");
		file.WriteString(Price);
	}	
	file.Close();//关闭文件

	CDialogEx::PostNcDestroy();
}


void CBookMgr::RefreshBookInfo(void)//更新书籍信息函数
{
	POSITION p = ((CKZLibApp *)AfxGetApp())->bookList.GetHeadPosition();//得到头索引
	mBookListCtrl->DeleteAllItems();//先清空列表
	while(p)
	{
		BookInfo &bInfo = ((CKZLibApp *)AfxGetApp())->bookList.GetNext(p);//遍历booklist
		int index = mBookListCtrl->GetItemCount();
		int addRow = mBookListCtrl->InsertItem(index, _T(""));//set之前需先插入insertcolumn
		mBookListCtrl->SetItemText(addRow, 0, bInfo.mCode);
		mBookListCtrl->SetItemText(addRow, 1, bInfo.mName);
		mBookListCtrl->SetItemText(addRow, 2, bInfo.mAuthor);
		mBookListCtrl->SetItemText(addRow, 3, bInfo.mPublish);
		mBookListCtrl->SetItemText(addRow, 4, bInfo.mPublishTime);
		mBookListCtrl->SetItemText(addRow, 5, bInfo.mType);
		CString countStr,priceStr;
		countStr.Format(_T("%d"), bInfo.mKeepCount);
		priceStr.Format(_T("%d"), bInfo.mPrice);
		mBookListCtrl->SetItemText(addRow, 6, countStr);
		mBookListCtrl->SetItemText(addRow, 7, priceStr);
	}
}

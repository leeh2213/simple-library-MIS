// BookInfoInputDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KZLib.h"
#include "BookInfoInputDlg.h"
#include "afxdialogex.h"


// BookInfoInputDlg 对话框

IMPLEMENT_DYNAMIC(BookInfoInputDlg, CDialog)

BookInfoInputDlg::BookInfoInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(BookInfoInputDlg::IDD, pParent)
{
	isEditMode = FALSE;
}

BookInfoInputDlg::~BookInfoInputDlg()
{
}

void BookInfoInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(BookInfoInputDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BOOKINFO_ADD, &BookInfoInputDlg::OnBnClickedButtonBookinfoAdd)
	ON_BN_CLICKED(IDC_BUTTON_BOOKINFO_CANCEL, &BookInfoInputDlg::OnBnClickedButtonBookinfoCancel)
END_MESSAGE_MAP()


// BookInfoInputDlg 消息处理程序


void BookInfoInputDlg::OnBnClickedButtonBookinfoAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	bool nullFlag = false;

	GetDlgItemText(IDC_EDIT_BOOKINFO_CODE, mInfo.mCode);
	GetDlgItemText(IDC_EDIT_BOOKINFO_NAME, mInfo.mName);
	GetDlgItemText(IDC_EDIT_BOOKINFO_AUTHOR, mInfo.mAuthor);
	GetDlgItemText(IDC_EDIT_BOOKINFO_PUBLISH, mInfo.mPublish);
	GetDlgItemText(IDC_EDIT_BOOKINFO_PUBLISHTIME, mInfo.mPublishTime);
	GetDlgItemText(IDC_EDIT_BOOKINFO_TYPE,mInfo.mType);
	CString countStr,priceStr;
	GetDlgItemText(IDC_EDIT_BOOKINFO_COUNT, countStr);
	mInfo.mKeepCount = _ttoi(countStr);//将uint转换为cstring类型
	GetDlgItemText(IDC_EDIT_BOOKINFO_PRICE, priceStr);
	mInfo.mPrice = _ttoi(priceStr);

	nullFlag = mInfo.mCode.GetLength() == 0;
	nullFlag = mInfo.mName.GetLength() == 0;
	nullFlag = mInfo.mAuthor.GetLength() == 0;
	nullFlag = mInfo.mPublish.GetLength() == 0;
	nullFlag = mInfo.mPublishTime.GetLength() == 0;
	nullFlag = mInfo.mType.GetLength() == 0;
	nullFlag = priceStr.GetLength() == 0;
	nullFlag = countStr.GetLength() == 0;

	if(nullFlag){
		MessageBox(_T("必填信息不得为空"), _T("提示"), MB_OK);
		return;
	}

	CDialog::OnOK();
}


void BookInfoInputDlg::OnBnClickedButtonBookinfoCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


BOOL BookInfoInputDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN){
		if(pMsg->wParam == VK_RETURN){
			return TRUE; 
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL BookInfoInputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(isEditMode){
		SetDlgItemText(IDC_EDIT_BOOKINFO_CODE, mInfo.mCode);
		SetDlgItemText(IDC_EDIT_BOOKINFO_NAME, mInfo.mName);
		SetDlgItemText(IDC_EDIT_BOOKINFO_PUBLISH, mInfo.mPublish);
		SetDlgItemText(IDC_EDIT_BOOKINFO_AUTHOR, mInfo.mAuthor);
		SetDlgItemText(IDC_EDIT_BOOKINFO_TYPE, mInfo.mType);
		SetDlgItemText(IDC_EDIT_BOOKINFO_PUBLISHTIME, mInfo.mPublishTime);
		SetDlgItemInt(IDC_EDIT_BOOKINFO_COUNT, mInfo.mKeepCount);	
		SetDlgItemInt(IDC_EDIT_BOOKINFO_PRICE, mInfo.mPrice);	

		SetWindowText(_T("编辑图书信息"));
		SetDlgItemText(IDC_BUTTON_BOOKINFO_ADD, _T("修改图书信息"));
	}

	return TRUE; 
}

// LendRecInputDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KZLib.h"
#include "LendRecInputDlg.h"
#include "afxdialogex.h"


// LendRecInputDlg �Ի���

IMPLEMENT_DYNAMIC(LendRecInputDlg, CDialogEx)

LendRecInputDlg::LendRecInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(LendRecInputDlg::IDD, pParent)
{

}

LendRecInputDlg::~LendRecInputDlg()
{
}

void LendRecInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LendRecInputDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTION_RECDLG_ADD, &LendRecInputDlg::OnBnClickedButtionRecdlgAdd)
	ON_BN_CLICKED(IDC_BUTTON_RECDLG_CANCEL, &LendRecInputDlg::OnBnClickedButtonRecdlgCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_LENDBOOKNAME, &LendRecInputDlg::OnSelchangeComboLendbookname)
END_MESSAGE_MAP()
 

// LendRecInputDlg ��Ϣ�������


void LendRecInputDlg::OnBnClickedButtionRecdlgAdd()//��ӽ��ļ�¼dlg->add
{//��ȡ������Ϣ
	GetDlgItemText(IDC_COMBO_LENDERNAME, recInfo.mLenderName);
	GetDlgItemText(IDC_COMBO_LENDBOOKNAME, recInfo.mBookName);
	GetDlgItemText(IDC_EDIT_LEND_AUTHOR, recInfo.mBookAuthor);
	GetDlgItemText(IDC_EDIT_LENDDATE, recInfo.mDate);
	//������Ϣ����
	if(recInfo.mLenderName.GetLength() == 0 || recInfo.mBookAuthor.GetLength() == 0
		|| recInfo.mBookName.GetLength() == 0){
			MessageBox(_T("������Ϣ����Ϊ��"), _T("��ʾ"), MB_OK);
			return;
	}
	POSITION p = ((CKZLibApp *)AfxGetApp())->bookList.GetHeadPosition();
	BOOL addFlag = FALSE;
	while(p){
		BookInfo &info = ((CKZLibApp *)AfxGetApp())->bookList.GetNext(p);
		if(info.mName == recInfo.mBookName && info.mKeepCount > 0){
			addFlag = TRUE;
			info.mKeepCount--;
			break;
		}
	}

	if(addFlag)
		CDialogEx::OnOK();
	else
		MessageBox(_T("��Ӵ���ͼ�������Ϊ0��ͼ�鲻����"), _T("��ʾ"));
}


void LendRecInputDlg::OnBnClickedButtonRecdlgCancel()//ȡ��������Ϣdlg��ť
{
	CDialogEx::OnCancel();
}


BOOL LendRecInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��ȡϵͳʱ��
	CTime t = CTime::GetCurrentTime();
	recInfo.mDateInt = t.GetTime();
	SetDlgItemText(IDC_EDIT_LENDDATE, t.Format(_T("%Y-%m-%d %H:%M")));

	// ��ʼ�� BookName ComboBox
	CString strTemp;
	CKZLibApp* app = ((CKZLibApp *)AfxGetApp());

	CComboBox* readerNameCb = (CComboBox *)GetDlgItem(IDC_COMBO_LENDERNAME);
	
	int readerCount = app->readerList.GetCount();//��ȡ��������

	POSITION r_pos = app->readerList.GetHeadPosition();
	for(int i=0; i<readerCount; i++){
		//ѡ�������
		strTemp = app->readerList.GetNext(r_pos).mName;
		readerNameCb->AddString(strTemp);
	}
	if(readerNameCb->GetCount()>0){
		readerNameCb->SetCurSel(0);
	}

	// ��ʼ�� ReaderName ComboBox
	CComboBox* bookNameCb = (CComboBox *)GetDlgItem(IDC_COMBO_LENDBOOKNAME);
	
	int bookCount = app->bookList.GetCount();
	POSITION b_pos = app->bookList.GetHeadPosition();
	for(int i=0; i<bookCount; i++){//ѡ������鼮
		strTemp = app->bookList.GetNext(b_pos).mName;
		bookNameCb->AddString(strTemp);
	}
	if(bookNameCb->GetCount() > 0){
		bookNameCb->SetCurSel(0);
		SetDlgItemText(IDC_EDIT_LEND_AUTHOR, app->bookList.GetHead().mAuthor);
	}

	return TRUE;  
}


void LendRecInputDlg::OnSelchangeComboLendbookname()//�Զ�ѡ���鼮��Ӧ������
{
	CComboBox* bookNameCb = (CComboBox *)GetDlgItem(IDC_COMBO_LENDBOOKNAME);
	int curSel = bookNameCb->GetCurSel();
	CKZLibApp* app = ((CKZLibApp *)AfxGetApp());
	POSITION p = app->bookList.GetHeadPosition();

	CString s;
	for(int i=0; i<=curSel; i++){
		s = app->bookList.GetNext(p).mAuthor;
	}
	SetDlgItemText(IDC_EDIT_LEND_AUTHOR, s);
}

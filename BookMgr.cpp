// BookMgr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KZLib.h"
#include "BookMgr.h"
#include "afxdialogex.h"
#include "BookInfoInputDlg.h"
#include <locale>


// CBookMgr �Ի���

IMPLEMENT_DYNAMIC(CBookMgr, CDialogEx)

CBookMgr::CBookMgr(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBookMgr::IDD, pParent)
{
	saveFilePath = _T("bookinfo.cdb");//�ļ�·��
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


// CBookMgr ��Ϣ�������


BOOL CBookMgr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	mBookListCtrl = (CListCtrl *)GetDlgItem(IDC_LC_BOOKINFO);

	LONG styles=GetWindowLong(mBookListCtrl->m_hWnd, GWL_STYLE); 
    SetWindowLong(mBookListCtrl->m_hWnd, GWL_STYLE, styles|LVS_REPORT);
	mBookListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CRect rs;
	GetClientRect(&rs);
	int colWidth = rs.Width()/8;	//�����п�

	mBookListCtrl->InsertColumn(0, _T("���"), LVCFMT_LEFT, colWidth, 0);
	mBookListCtrl->InsertColumn(1, _T("�鼮����"), LVCFMT_CENTER, colWidth, 1);
	mBookListCtrl->InsertColumn(2, _T("����"), LVCFMT_CENTER, colWidth, 2);
	mBookListCtrl->InsertColumn(3, _T("������"), LVCFMT_CENTER, colWidth, 3);
	mBookListCtrl->InsertColumn(4, _T("����ʱ��"), LVCFMT_CENTER, colWidth , 4);
	mBookListCtrl->InsertColumn(5, _T("�鼮����"), LVCFMT_CENTER, colWidth , 5);
	mBookListCtrl->InsertColumn(6, _T("������"), LVCFMT_CENTER, colWidth , 6);
	mBookListCtrl->InsertColumn(7, _T("�۸�"), LVCFMT_LEFT, colWidth, 7);

	CStdioFile file;	//read info from file

	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL)); 
	BOOL open = file.Open(saveFilePath, CFile::modeRead);//��ʼ��ʱ��txt���鼮��Ϣ����(ֻ��ģʽ)
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


void CBookMgr::OnBnClickedButtonDelBook()//ɾ����Ϣ
{
	
	int clkMark = MessageBox(_T("ȷ��Ҫɾ����"), _T("��ʾ"),MB_OKCANCEL);
	if(clkMark == IDOK)
	{
		int selMark = mBookListCtrl->GetSelectionMark();
		if(selMark<0){
			MessageBox(_T("����ѡ��Ҫɾ������Ŀ��"), _T("��ʾ"), MB_OK);
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


void CBookMgr::OnBnClickedButtonAddbook()//�����Ϣ��ť
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BookInfoInputDlg inputDlg;
	int addRes = inputDlg.DoModal();
	if(addRes == IDOK){
		AddBookInfo(inputDlg.mInfo);
	}
}


int CBookMgr::AddBookInfo(BookInfo bInfo)//�����Ϣ����
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


void CBookMgr::OnDblclkLcBookinfo(NMHDR *pNMHDR, LRESULT *pResult)//˫��listcontrol�����鼮��Ϣ�޸�Dlg
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	NMLISTVIEW* p = (NMLISTVIEW *)pNMHDR;
	int itemRow = p->iItem;		//��ȡѡ�е���
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
		((CKZLibApp *)AfxGetApp())->bookList.SetAt(p, editDlg.mInfo);//��ָ������λ���ϵ������ַ��滻Ϊ�����ַ� 
	}
	*pResult = 0;
}



void CBookMgr::PostNcDestroy()//����ɾ�Ĳ����ֻ����list����ɣ�����޸ĺ��list����д��txt
{
	//write to file
	CStdioFile file;//Ĭ��ΪTextģʽ�����⻻��
	CString keepCount, Price;
	file.Open(saveFilePath, CFile::modeCreate|CFile::modeWrite);
	setlocale(LC_CTYPE, ("chs"));//���л� book�Ľṹ��Ϊcstring��Ա
	POSITION p = ((CKZLibApp *)AfxGetApp())->bookList.GetHeadPosition();//�õ�ͷ����
	while(p){
		BookInfo info = ((CKZLibApp *)AfxGetApp())->bookList.GetNext(p);//����booklist
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
	file.Close();//�ر��ļ�

	CDialogEx::PostNcDestroy();
}


void CBookMgr::RefreshBookInfo(void)//�����鼮��Ϣ����
{
	POSITION p = ((CKZLibApp *)AfxGetApp())->bookList.GetHeadPosition();//�õ�ͷ����
	mBookListCtrl->DeleteAllItems();//������б�
	while(p)
	{
		BookInfo &bInfo = ((CKZLibApp *)AfxGetApp())->bookList.GetNext(p);//����booklist
		int index = mBookListCtrl->GetItemCount();
		int addRow = mBookListCtrl->InsertItem(index, _T(""));//set֮ǰ���Ȳ���insertcolumn
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

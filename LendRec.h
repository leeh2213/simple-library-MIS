#pragma once
class LendRec
{
public:
	LendRec(void);
	~LendRec(void);

	CString mLenderName;//����������
	CString mBookName;//�����鼮����
	CString mBookAuthor;//����
	CString mDate;
	UINT mDateInt;

	void SetDateString(CString year, CString month, CString day, CString hour);//����ʱ���ֵ
};


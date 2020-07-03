#pragma once
class LendRec
{
public:
	LendRec(void);
	~LendRec(void);

	CString mLenderName;//借阅人名字
	CString mBookName;//借阅书籍名称
	CString mBookAuthor;//作者
	CString mDate;
	UINT mDateInt;

	void SetDateString(CString year, CString month, CString day, CString hour);//设置时间的值
};


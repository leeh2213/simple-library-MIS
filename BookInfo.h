#pragma once
class BookInfo
{
public:
	BookInfo(void);
	~BookInfo(void);
	//登录号、书名、作者名、分类号、出版单位、出版时间、价格
	CString mCode;//书号
	CString mName;//书籍名称
	CString mAuthor;//作者
	CString mPublish;//出版社
	CString mPublishTime;//出版时间
	CString mType;//书记类型
	UINT mKeepCount;//持有量
	UINT mPrice;//价格
};


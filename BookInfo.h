#pragma once
class BookInfo
{
public:
	BookInfo(void);
	~BookInfo(void);
	//��¼�š�������������������š����浥λ������ʱ�䡢�۸�
	CString mCode;//���
	CString mName;//�鼮����
	CString mAuthor;//����
	CString mPublish;//������
	CString mPublishTime;//����ʱ��
	CString mType;//�������
	UINT mKeepCount;//������
	UINT mPrice;//�۸�
};


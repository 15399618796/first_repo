//#pragma once
class XTask
{
public:
	// һ�ͻ���һ��base
	struct event_base *base = 0;

	// ���ӵ�sock
	int sock = 0;

	// �̳߳�id
	int thread_id = 0;

	// ��ʼ������
	virtual bool Init() = 0;
};


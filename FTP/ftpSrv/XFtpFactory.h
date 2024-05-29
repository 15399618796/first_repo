#pragma once
#include "XTask.h"

class XFtpFactory
{
public:
	// 获取单例实例
	static XFtpFactory *Get() {
		static XFtpFactory f;
		return &f;
	}

	XTask *CreateTask();

private:
	// 私有构造，防止外部直接实例化
	XFtpFactory();
};


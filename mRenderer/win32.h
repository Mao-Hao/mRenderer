#pragma once
#include "mBase.h"
#include <windows.h>
#include "mDevice.h"

// ����GDI��˫������Ʋ���
// ��һ�����ڣ������ݽ�������Ļ�ϻ���

// ָ��һ�� width * height * sizeof(uint) ��С���ڴ����׵�ַ
// ÿ��uint������һ�����ص���ɫ��0000 0000 RRRR RRRR GGGG GGGG BBBB BBBB

int mInitWindow();

void mUpdateWindow();

void mDestroyWindow();

bool mWindowShouldClose();

void mGetCursorPos(float & xpos, float & ypos);

double mGetNativeTime();
#include "apilib.h"

void HariMain(void)
{
	int win;
	char buf[150 * 50];
	win = api_openwin(buf, 150, 50, -1, "hello");
	api_boxfilwin(win,  8, 36, 141, 43, 3 /* ��ɫ */);
	api_putstrwin(win, 28, 28, 0 /* ��ɫ */, 12, "hello, world");
	for (;;) {
		if (api_getkey(1) == 0x0a) {
			break; /* ���»س�����break; */
		}
	}
	api_end();
}

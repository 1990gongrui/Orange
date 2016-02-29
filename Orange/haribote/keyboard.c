/* 僉乕儃乕僪娭學 */

#include "bootpack.h"

struct FIFO32 *keyfifo;
int keydata0;

/*来自PS/2键盘的中断处理*/
void inthandler21(int *esp)
{
	int data;
	io_out8(PIC0_OCW2, 0x61);	/* 通知PIC"IRQ-01已经受理完毕 */
	data = io_in8(PORT_KEYDAT);
	fifo32_put(keyfifo, data + keydata0);//采用fifo(队列)作为键盘输入的缓冲区，注意的是fifo32_put接收的第一个参数是内存地址（即指针）
	return;
}

#define PORT_KEYSTA				0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE		0x60
#define KBC_MODE				0x47

void wait_KBC_sendready(void)
{
	/* 等待键盘控制电路准备完毕 */
	for (;;) {
		if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
			break;
		}
	}
	return;
}

void init_keyboard(struct FIFO32 *fifo, int data0)
{
	/* 初始化鼠标 */
	keyfifo = fifo;
	keydata0 = data0;
	/* 初始化键盘控制电路 */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, KBC_MODE);
	return;
}

/* 儅僂僗娭學 */

#include "bootpack.h"

struct FIFO32 *mousefifo;
int mousedata0;

void inthandler2c(int *esp)
/* 来自PS/2鼠标的中断 */
{
	int data;
	io_out8(PIC1_OCW2, 0x64);	/* 通知PIC1 IRQ-12的受理已经完成 */
	io_out8(PIC0_OCW2, 0x62);	/* 通知PIC0 IRQ-02的受理已经完成 */
	data = io_in8(PORT_KEYDAT);
	fifo32_put(mousefifo, data + mousedata0);/* 把鼠标中断数据放到fifo队列当中*/
	return;
}

#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4

void enable_mouse(struct FIFO32 *fifo, int data0, struct MOUSE_DEC *mdec)
{
	/* 激活鼠标 */
	mousefifo = fifo;
	mousedata0 = data0;
	/* 等待键盘控制电路准备完毕 */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	/* 顺利的话，键盘控制其会返回ACK(0xfa) */
	mdec->phase = 0; /* 进入到等待鼠标的0xfa的状态 */
	return;
}

/* 对鼠标数据的解析 */
/*mouse_dec结构体的几个变量用于存放解读结果。x,y,btn, 分别用于存放移动信息和鼠标按键状态*/
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat)
{
	if (mdec->phase == 0) {
		/* 等待鼠标的0xfa的状态 */
		if (dat == 0xfa) {
			mdec->phase = 1;
		}
		return 0;
	}
	if (mdec->phase == 1) {
		/* 等待鼠标第一字节的阶段 */
		if ((dat & 0xc8) == 0x08) {
			/* 如果第一字节正确 */
			mdec->buf[0] = dat;
			mdec->phase = 2;
		}
		return 0;
	}
	if (mdec->phase == 2) {
		/* 等待鼠标第二字节的阶段 */
		mdec->buf[1] = dat;
		mdec->phase = 3;
		return 0;
	}
	if (mdec->phase == 3) {
		/* 等待鼠标第三字节的阶段 */
		mdec->buf[2] = dat;
		mdec->phase = 1;
		mdec->btn = mdec->buf[0] & 0x07;//通过和0x07(0000 0111)相与来取得buf[0]的低三位，而这三位里存放鼠标键的状态
		mdec->x = mdec->buf[1];
		mdec->y = mdec->buf[2];
		if ((mdec->buf[0] & 0x10) != 0) {
			mdec->x |= 0xffffff00;
		}
		if ((mdec->buf[0] & 0x20) != 0) {
			mdec->y |= 0xffffff00;
		}
		mdec->y = - mdec->y; /* 鼠标的y方向与画面符号相反 */
		return 1;
	}
	return -1; /* 应该不可能到这里来 */
}

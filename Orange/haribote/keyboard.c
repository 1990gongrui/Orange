/* キーボード関係 */

#include "bootpack.h"

struct FIFO32 *keyfifo;
int keydata0;

/*ﾀｴﾗﾔPS/2ｼ�ﾅﾌｵﾄﾖﾐｶﾏｴｦﾀ�*/
void inthandler21(int *esp)
{
	int data;
	io_out8(PIC0_OCW2, 0x61);	/* ﾍｨﾖｪPIC"IRQ-01ﾒﾑｾｭﾊﾜﾀ�ﾍ�ｱﾏ */
	data = io_in8(PORT_KEYDAT);
	fifo32_put(keyfifo, data + keydata0);//ｲﾉﾓﾃfifo(ｶﾓﾁﾐ)ﾗ�ﾎｪｼ�ﾅﾌﾊ菠�ｵﾄｻｺｳ衂�｣ｬﾗ｢ﾒ箏ﾄﾊﾇfifo32_putｽﾓﾊﾕｵﾄｵﾚﾒｻｸ�ｲﾎﾊ�ﾊﾇﾄﾚｴ豬ﾘﾖｷ｣ｨｼｴﾖｸﾕ�｣ｩ
	return;
}

#define PORT_KEYSTA				0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE		0x60
#define KBC_MODE				0x47

void wait_KBC_sendready(void)
{
	/* ｵﾈｴ�ｼ�ﾅﾌｿﾘﾖﾆｵ酊ｷﾗｼｱｸﾍ�ｱﾏ */
	for (;;) {
		if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
			break;
		}
	}
	return;
}

void init_keyboard(struct FIFO32 *fifo, int data0)
{
	/* ｳ�ﾊｼｻｯﾊ�ｱ� */
	keyfifo = fifo;
	keydata0 = data0;
	/* ｳ�ﾊｼｻｯｼ�ﾅﾌｿﾘﾖﾆｵ酊ｷ */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, KBC_MODE);
	return;
}

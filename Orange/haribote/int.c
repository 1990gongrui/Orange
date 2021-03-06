/* 割り込み関係 */
/* ﾊｹﾓﾃﾖﾐｶﾏﾒｪﾊﾗﾏﾈｳ�ﾊｼｻｯPIC｡｣PICﾊﾇ｡ｰprogrammmable interrupt controller｡ｱｵﾄﾋ�ﾐｴ｣ｬﾒ簍ｼﾊﾇｿﾉｱ犁ﾌﾖﾐｶﾏｿﾘﾖﾆﾆ�*/

#include "bootpack.h"
#include <stdio.h>

void init_pic(void)
/* PICｵﾄｳ�ﾊｼｻｯ */
{
	/*ｴﾓCPUｵﾄｽﾇｶﾈﾀｴｿｴ｣ｬPICﾊﾇﾍ箚ｿﾉ雎ｸ｣ｬCPUﾊｹﾓﾃOUTﾖｸﾁ�ｽ�ﾐﾐｲﾙﾗ�｡｣ｶ�PICﾄﾚｲｿﾓﾐｺﾜｶ狆ﾄｴ貳�｣ｬﾓﾃｶﾋｿﾚｺﾅｶﾔｱﾋｴﾋｽ�ﾐﾐﾇ�ｱ�｣ｬ*/
	/*ﾒﾔｾ�ｶｨﾊﾇﾐｴﾈ�ﾄﾄﾒｻｸ�ｼﾄｴ貳�*/
	io_out8(PIC0_IMR,  0xff  ); /* ｽ�ﾖｹﾋ�ﾓﾐﾖﾐｶﾏ */
	io_out8(PIC1_IMR,  0xff  ); /* ｽ�ﾖｹﾋ�ﾓﾐﾖﾐｶﾏ */

	io_out8(PIC0_ICW1, 0x11  ); /* ｱﾟﾑﾘｴ･ｷ｢ﾄ｣ﾊｽ｣ｨedge trigger mode｣ｩ */
	io_out8(PIC0_ICW2, 0x20  ); /* IRQ0-7ﾓﾉINT20-27ｽﾓﾊﾕ */
	io_out8(PIC0_ICW3, 1 << 2); /* PIC1ﾓﾉIRQ2ｽﾓﾊﾕ */
	io_out8(PIC0_ICW4, 0x01  ); /* ﾎﾞｻｺｳ衂�ﾄ｣ﾊｽ */

	io_out8(PIC1_ICW1, 0x11  ); /* ｱﾟﾑﾘｴ･ｷ｢ﾄ｣ﾊｽ｣ｨedge trigger mode｣ｩ */
	io_out8(PIC1_ICW2, 0x28  ); /* IRQ8-15ﾓﾉINT28-2fｽﾓﾊﾕ */
	io_out8(PIC1_ICW3, 2     ); /* PIC1ﾓﾉIRQ2ｽﾓﾊﾕ */
	io_out8(PIC1_ICW4, 0x01  ); /* ﾎﾞｻｺｳ衂�ﾄ｣ﾊｽ */

	io_out8(PIC0_IMR,  0xfb  ); /* 11111011 PIC1ﾒﾔﾍ篳ｫｲｿｽ�ﾖｹ */
	io_out8(PIC1_IMR,  0xff  ); /* 11111111 ｽ�ﾖｹﾋ�ﾓﾐﾖﾐｶﾏ */

	return;
}

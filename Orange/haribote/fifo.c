/* FIFOライブラリ */

#include "bootpack.h"

#define FLAGS_OVERRUN		0x0001

void fifo32_init(struct FIFO32 *fifo, int size, int *buf, struct TASK *task)
/* ｳ�ﾊｼｻｯFIFOｻｺｳ衂� */
{
	fifo->size = size;
	fifo->buf = buf;
	fifo->free = size; /* ﾊ｣ﾓ狒ﾕｼ莊ﾄｴ�ﾐ｡ */
	fifo->flags = 0;
	fifo->p = 0; /* ﾏﾂﾒｻｸ�ﾊ�ｾﾝﾐｴﾈ�ﾎｻﾖﾃ */
	fifo->q = 0; /* ﾏﾂﾒｻｸ�ﾊ�ｾﾝｶﾁｳ�ﾎｻﾖﾃ */
	fifo->task = task; /* データが入ったときに起こすタスク */
	return;
}

int fifo32_put(struct FIFO32 *fifo, int data)
/* ﾏ�FIFOｴｫﾋﾍﾊ�ｾﾝｲ｢ｱ｣ｴ� */
{
	if (fifo->free == 0) {
		/* ｿﾕﾓ狹ｻﾓﾐﾁﾋ｣ｬﾒ邉� */
		fifo->flags |= FLAGS_OVERRUN;
		return -1;
	}
	fifo->buf[fifo->p] = data;
	fifo->p++;
	if (fifo->p == fifo->size) {
		fifo->p = 0;
	}
	fifo->free--;
	if (fifo->task != 0) {
		if (fifo->task->flags != 2) { /* ﾈ郢�ﾈﾎﾎ�ｴｦﾓﾚﾐﾝﾃﾟﾗｴﾌｬ */
			task_run(fifo->task, -1, 0); /* ｽｫﾈﾎﾎ�ｻｽﾐﾑ */
		}
	}
	return 0;
}

int fifo32_get(struct FIFO32 *fifo)
/* ｴﾓFIFOﾈ｡ｵﾃﾒｻｸ�ﾊ�ｾﾝ */
{
	int data;
	if (fifo->free == fifo->size) {
		/* ﾈ郢�ｻｺｳ衂�ﾎｪｿﾕ｣ｬﾔ�ｷｵｻﾘ -1 */
		return -1;
	}
	data = fifo->buf[fifo->q];
	fifo->q++;
	if (fifo->q == fifo->size) {
		fifo->q = 0;
	}
	fifo->free++;
	return data;
}

int fifo32_status(struct FIFO32 *fifo)
/* ｱｨｸ贅ｻﾏﾂｵｽｵﾗｻ�ﾔﾜﾁﾋｶ猖ﾙﾊ�ｾﾝ */
{
	return fifo->size - fifo->free;
}

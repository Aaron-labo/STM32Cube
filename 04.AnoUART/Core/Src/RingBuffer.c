/*
 * RingBuffer.c
 *
 *  Created on: 2021年4月1日
 *      Author: Francis
 */
#include "RingBuffer.h"

/************************************************
 *  该函数为循环缓冲区初始化函数
 * 参数：head - 读的首地址
 *      tail - 写的首地址
 * 		读取的范围:head~tail
 * 		写入的范围:tail~head
 * 		head == tail 表示缓冲区空
 * 		head == tail+1 表示缓冲区满
 * 返回：读取是否正常
 ***********************************************/
uint8_t rb_init(rb_t *rb, uint8_t *pbuff, uint32_t size) {
	rb->size = size;
	rb->buff = pbuff;
	rb->tail = pbuff;
	rb->head = pbuff;

	return RET_RB_OK;
}

/************************************************
 * 该函数用于输出循环缓冲器中的可读数量
 * 参数：rb为被操作的结构体指针
 * 返回：RingBuffer中现存的可读数
 ***********************************************/
uint32_t rb_readable(rb_t *rb) {
	if (rb->head <= rb->tail) {
		return (rb->tail - rb->head);
	} else {
		return (rb->size - (rb->head - rb->tail));
	}
}

/************************************************
 * 该函数用于输出循环缓冲器中的可写数量
 * rb - 被操作的结构体指针
 * 返回：RingBuffer中现存的可写数
 ***********************************************/
uint32_t rb_writeable(rb_t *rb) {
	//buffer中总有一个字节不用
	return rb->size - rb_readable(rb) - 1;
}

/************************************************
 * 用于读入RingBuffer中的一个字节数据
 * 参数：rb --- 被操作的结构体指针
 * 	    data - 读出的数据存放与此
 * 返回：符合枚举rb_ret_t类型的数据
 ***********************************************/
uint8_t rb_readbyte(rb_t *rb, uint8_t *data) {
	if (rb == NULL) {
		return RET_RB_BUFF_ERR;
	}
	if (rb_writeable(rb) == 0) {
		return RET_RB_NODATA;
	}

	*data = *rb->head;
	rb->head++;

	//如果head超过缓存区的范围，则返回缓存区的起始位置
	if (rb->head >= (rb->buff + rb->size)) {
		rb->head = rb->buff;
	}

	return RET_RB_OK;
}

/************************************************
 * 用于读入RingBuffer中的多个字节数据
 * 参数：rb --- 被操作的结构体指针
 * 	    data - 读出的数据存放与此
 * 	    len -- 需要读取的数据大小
 * 返回：符合枚举rb_ret_t类型的数据
 ***********************************************/
uint8_t rb_readbytes(rb_t *rb, uint8_t *data, uint32_t len) {
	uint8_t *pdata = data;
	while (len) {
		if (rb_readbyte(rb, pdata++) == RET_RB_NODATA) {
			return RET_RB_RDWARN;
		}
		len--;
	}
	return RET_RB_OK;
}

/************************************************
 * 用于写入RingBuffer中的一个字节数据
 * 参数：rb --- 被操作的结构体指针
 * 	    data - 被写入的数据
 * 返回：符合枚举rb_ret_t类型的数据
 ***********************************************/
uint8_t rb_writebyte(rb_t *rb, uint8_t data) {
	if (rb == NULL) {
		return RET_RB_BUFF_ERR;
	}
	if (rb_writeable(rb) == 0) {
		return RET_RB_NOSPACE;
	}

	*rb->tail = data;
	rb->tail++;

	//如果tail超过缓存区的范围，则返回缓存区的起始位置
	if (rb->tail >= (rb->buff + rb->size)) {
		rb->tail = rb->buff;
	}

	return RET_RB_OK;
}

/************************************************
 * 用于写入RingBuffer中的多个字节数据
 * 参数：rb --- 被操作的结构体指针
 * 	    data - 被写入的数据的指针
 * 	    len -- 需要写入的数据大小
 * 返回：符合枚举rb_ret_t类型的数据
 ***********************************************/
uint8_t rb_writebytes(rb_t *rb, uint8_t *data, uint32_t len) {
	uint8_t *pdata = data;
	while (len) {
		if (rb_writebyte(rb, *pdata++) == RET_RB_NOSPACE) {
			return RET_RB_WAWARN;
		}
		len--;
	}
	return RET_RB_OK;
}

/************************************************
 * 用于查看可读数据中第offset个数，但不改变head
 * 参数：rb --- 被操作的结构体指针
 * 	    data - 查看得到的数据保存地址
 * 	    offset - 读取偏移量
 * 返回：符合枚举rb_ret_t类型的数据
 ***********************************************/
uint8_t rb_peekbyte(rb_t *rb, uint8_t *data, uint32_t offset) {
	if (rb == NULL) {
		return RET_RB_BUFF_ERR;
	}
	if (offset > rb_readable(rb)) {
		return RET_RB_NODATA;
	}

	if ((rb->head + offset) > (rb->buff + rb->size)) {
		*data = *((rb->head + offset) - (rb->buff + rb->size) + rb->buff);
	} else {
		*data = *(rb->head + offset);
	}

	return RET_RB_OK;
}

/************************************************
 * 用于查看指定数据在缓存区中的位置
 * 参数：rb --- 被操作的结构体指针
 * 	    data - 需要查找的数据
 * 	    addr - 存放查找到的数据位置
 * 返回：符合枚举rb_ret_t类型的数据
 ***********************************************/
uint8_t rb_peedbyteadder(rb_t *rb, uint8_t data, uint32_t *addr) {
	uint8_t i = 0;
	uint32_t readable = rb_readable(rb);
	uint8_t dirt;

	if (rb == NULL) {
		return RET_RB_BUFF_ERR;
	}
	if (readable == 0) {
		return RET_RB_NODATA;
	}
	while (i < readable) {
		if ((rb->head + i) > (rb->buff + rb->size)) {
			dirt = *((rb->head + i) - (rb->buff + rb->size) + rb->buff);
		} else {
			dirt = rb->head[i];
		}
		if (dirt == data) {
			*addr = (uint32_t) &rb->head[i];
			return RET_RB_OK;
		}
		i++;
	}

	*addr = (uint32_t) NULL;
	return RET_RB_NODATA;
}

/************************************************
 * 用于复位清空
 * 参数：rb --- 被操作的结构体指针
 * 返回：符合枚举rb_ret_t类型的数据
 ***********************************************/
uint8_t rb_reset(rb_t *rb) {
	if (rb == NULL) {
		return RET_RB_BUFF_ERR;
	}

	rb->tail = rb->head;
	memset(rb->buff, 0, rb->size);

	return RET_RB_OK;
}


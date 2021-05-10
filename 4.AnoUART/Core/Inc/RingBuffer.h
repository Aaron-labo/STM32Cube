/*
 * RingBuffer.h
 *
 *  Created on: 2021年4月1日
 *      Author: Francis
 */

#ifndef INC_RINGBUFFER_H_
#define INC_RINGBUFFER_H_

#include "stm32f1xx.h"
#include "string.h"
#include "stdio.h"

//宏定义用于反映函数是否正常实现
enum rb_ret_t {
	RET_RB_OK,
	RET_RB_NOSPACE,
	RET_RB_WAWARN,
	RET_RB_NODATA,
	RET_RB_BUFF_ERR,
	RET_RB_RDWARN
};

//循环缓存区结构体
typedef struct {
	uint32_t size;
	uint8_t *head;
	uint8_t *tail;
	uint8_t *buff;
} rb_t;

uint8_t rb_init(rb_t *rb, uint8_t *pbuff, uint32_t size);
uint32_t rb_readable(rb_t *rb);
uint32_t rb_writeable(rb_t *rb);
uint8_t rb_readbyte(rb_t *rb, uint8_t *data);
uint8_t rb_readbytes(rb_t *rb, uint8_t *data, uint32_t len);
uint8_t rb_writebyte(rb_t *rb, uint8_t data);
uint8_t rb_writebytes(rb_t *rb, uint8_t *data, uint32_t len);
uint8_t rb_peekbyte(rb_t *rb, uint8_t *data, uint32_t offset);
uint8_t rb_peedbyteadder(rb_t *rb, uint8_t data, uint32_t *addr);
uint8_t rb_reset(rb_t *rb);

#endif /* INC_RINGBUFFER_H_ */

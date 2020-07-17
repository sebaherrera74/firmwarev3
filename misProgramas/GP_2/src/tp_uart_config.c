/*============================================================================
 * Copyright (c) 2020, TP RTOS II - CESE -UBA
 * Miguel del Valle <m.e.delvallecamino@ieee.org>
 * Leandro Torrent <leandrotorrent92@gmail.com>
 * Ezequiel Urdaniz <urdanizezequiel@gmail.com>
 * 
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2020/04/24
 * Version: 2.1.2
 *===========================================================================*/
/*=====[Inclusion of own header]=============================================*/
#include "tp_uart_config.h"

#include <stdio.h>

#include "qmpool.h"
#include "FreeRTOS.h"

/*=====[Definition macros of private constants]==============================*/
/*=====[Private function-like macros]========================================*/
/*=====[Definitions of private data types]===================================*/
/*=====[Definitions of external public global variables]=====================*/
/*=====[Definitions of public global variables]==============================*/
/*=====[Definitions of private global variables]=============================*/
/*=====[Prototypes (declarations) of private functions]======================*/
uint32_t CriticalEnter(uint8_t intoIsr);
void CriticalExit(uint8_t intoIsr, uint32_t basepri);
/*=====[Implementations of public functions]=================================*/
//GetTimer ( const uint8_t *, const timerName, const uint32_t msTime,
//	void * const pvTimerID, TimerCallbackFunction_t pxCallbackFunction );
//	me->xTimerCreate ( "TX Time Out", PROTOCOL_TIMEOUT,pdFALSE,
//		( void* ) selectedUart, onTxTimeOutCallback );
//uint8_t * GetMemory( uint8_t intoIsr ) {
//	if ( QMPOOL_TEST ) {
//		uint8_t *block1;
//		block1 = QMPool_get(&rxMemPool, 0, 0 );
//		return block1;
//	} else {
//		uint32_t basepri = 0;
//		basepri = CriticalEnter ( intoIsr );
//		uint8_t *block1;
//		block1 = pvPortMalloc ( MAX_INC_FRAME );
//		CriticalExit ( intoIsr, basepri );
//		return block1;
//	}
//}
//void ClearMemory ( uint8_t *block, uint8_t intoIsr ) {
//	uint32_t result = 0;
//	if ( QMPOOL_TEST ) {
//		QMPool_put ( &txMemPool, block, 0 );
//	} else{
//		uint32_t basepri = 0;
//		basepri = CriticalEnter ( intoIsr );
//		vPortFree ( block );
//		CriticalExit ( intoIsr, basepri );
//	}
//	}
void QMPool_init(QMPool * const me, void * const poolSto,
		uint_fast32_t poolSize, uint_fast16_t blockSize) {
	QFreeBlock *fb;
	uint_fast16_t nblocks;
	me->free_head = poolSto;
	/* round up the blockSize to fit an integer free blocks, no division */
	me->blockSize = (QMPoolSize) sizeof(QFreeBlock);
	nblocks = (uint_fast16_t) 1; /* start with just one */
	while (me->blockSize < (QMPoolSize) blockSize) {
		/* #free blocks that fit in one memory block */
		me->blockSize += (QMPoolSize) sizeof(QFreeBlock);
		++nblocks;
	}
	/* round-up to nearest block */
	blockSize = (uint_fast16_t) me->blockSize;
	/* don't count the last block */
	poolSize -= (uint_fast32_t) blockSize;
	/* the last block already in the pool */
	me->nTot = (QMPoolCtr) 1;
	/* start at the head of the free list */
	fb = (QFreeBlock *) me->free_head;
	while (poolSize >= (uint_fast32_t) blockSize) {
		/* chain all blocks together in a free-list... */
		fb->next = &fb[nblocks]; /* point next link to next block */
		fb = fb->next; /* advance to the next block */
		/* reduce available pool size */
		poolSize -= (uint_fast32_t) blockSize;
		++me->nTot; /* increment the number of blocks so far */
	}
	fb->next = (QFreeBlock *) 0; /* the last link points to NULL */
	me->nFree = me->nTot; /* all blocks are free */
	me->nMin = me->nTot; /* the minimum number of free blocks */
	me->start = poolSto; /* the original start this pool buffer */
	me->end = fb; /* the last block in this pool */
}
void QMPool_put(QMPool * const me, void *b, uint8_t intoIsr) {
	uint32_t basepri = 0;
	basepri = CriticalEnter(intoIsr);
	/* link into list */
	((QFreeBlock *) b)->next = (QFreeBlock *) me->free_head;
	me->free_head = b; /* set as new head of the free list */
	++me->nFree; /* one more free block in this pool */
	CriticalExit(intoIsr, basepri);
}
void *QMPool_get(QMPool * const me, uint_fast16_t const margin, uint8_t intoIsr) {
	QFreeBlock *fb;
	uint32_t basepri = 0;
	basepri = CriticalEnter(intoIsr);
	/* have more free blocks than the requested margin? */
	if (me->nFree > (QMPoolCtr) margin) {
		fb = (QFreeBlock *) me->free_head; /* get a free block */
		/* set the head to the next free block */
		me->free_head = fb->next;
		--me->nFree; /* one less free block */
		if (me->nMin > me->nFree)
			me->nMin = me->nFree; /* remember the new minimum */
	} else
		fb = (QFreeBlock *) 0;
	CriticalExit(intoIsr, basepri);
	return fb; /* return the block or NULL pointer to the caller */
}
uint_fast16_t QMPool_getMin(QMPool * const me, uint8_t intoIsr) {
	uint_fast16_t min;
	uint32_t basepri = 0;
	basepri = CriticalEnter(intoIsr);
	min = me->nMin;
	CriticalExit(intoIsr, basepri);
	return min;
}
char* Print_Qmpool_Struct(QMPool* Q, char* S) {
	sprintf(S, "mem pool\r\n----------\r\n"
			"free_head   =%d\r\n"
			"start       =%d\r\n"
			"end         =%d\r\n"
			"blockSize   =%d\r\n"
			"nTot        =%d\r\n"
			"nFree       =%d\r\n"
			"nMin        =%d\r\n", Q->free_head, Q->start, Q->end, Q->blockSize,
			Q->nTot, Q->nFree, Q->nMin);
	return S;
}
uint32_t CriticalEnter(uint8_t intoIsr) {
	uint32_t basepri = 0;
	if (intoIsr) {
		basepri = taskENTER_CRITICAL_FROM_ISR ( );
	} else {
		portENTER_CRITICAL ( );
	}
	return basepri;
}
void CriticalExit(uint8_t intoIsr, uint32_t basepri) {
	if (intoIsr) {
		taskEXIT_CRITICAL_FROM_ISR(basepri);
	} else {
		portEXIT_CRITICAL();
	}
}
/*=====[Implementations of interrupt functions]==============================*/
/*=====[Implementations of private functions]================================*/

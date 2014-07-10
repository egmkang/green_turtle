#pragma once

#include <stdint.h>

struct pq_time_tick_t
{
	uint64_t expire_time;
	uint64_t data;

	void (*callback)(pq_time_tick_t*, uint64_t);
};


void pq_timer_update(uint64_t now, uint64_t tick);
pq_time_tick_t* pq_timer_create(uint64_t time, void (*callback)(pq_time_tick_t*, uint64_t), uint64_t data);

void pq_timer_destroy(pq_time_tick_t* timer);

#define timer_update pq_timer_update
#define timer_create pq_timer_create
//#define timer_add pq_timer_add
#define timer_destroy pq_timer_destroy

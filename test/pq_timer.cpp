#include "pq_timer.h"
#include <memory>
#include <queue>
#include <vector>
#include <stdlib.h>

class compare_timer
{
public:
	bool operator()(pq_time_tick_t* left, pq_time_tick_t* right){
		return left->expire_time > right->expire_time;
	}
		
};
std::priority_queue<pq_time_tick_t*, std::vector<pq_time_tick_t*>, compare_timer> pq;

void pq_timer_update(uint64_t now, uint64_t tick)
{
    (void)tick;
	for (pq_time_tick_t* top = pq.empty() ? NULL : pq.top();
		top && (top->expire_time <= now);
		top = pq.empty() ? NULL : pq.top())
	{
		pq.pop();
		top->callback(top, top->data);
	}
}

pq_time_tick_t* pq_timer_create(uint64_t time, void (*callback)(pq_time_tick_t*, uint64_t), uint64_t data)
{
	pq_time_tick_t* ret = (pq_time_tick_t*)malloc(sizeof(pq_time_tick_t));
	ret->expire_time = time;
	ret->callback = callback;
	ret->data = data;
	pq.push(ret);

	return ret;
}

void pq_timer_destroy(pq_time_tick_t* timer)
{
	free(timer);
}

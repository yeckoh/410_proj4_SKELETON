/*
 * auth: wchang 00960978
 * prof: K. Perkins
 * proj: sp20 cs410 - p4: condition_variables, threads and stuff
 * date: 8 Apr. 2k20
 * desc: define what a waiter does and how to take orders
 */
#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/externs.h"

using namespace std;

Waiter::Waiter(int id,std::string filename):id(id),myIO(filename) {}

Waiter::~Waiter() {}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder) {
	int return_code;
	if((return_code = myIO.getNext(anOrder)) != SUCCESS)
		b_WaiterIsFinished = true;
	return return_code;
}

//contains a loop that will get orders from filename one at a time
void Waiter::beWaiter() {
	ORDER next_order;
	while (getNext(next_order) == SUCCESS) {
		lock_guard<mutex> using_pos_system(mutex_order_inQ);
		order_in_Q.push(next_order);
	}
	cv_order_inQ.notify_all();
}


#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/externs.h"

using namespace std;

Waiter::Waiter(int id,std::string filename):id(id),myIO(filename) {
}

Waiter::~Waiter() {}


//gets next Order from file_IO
//if return == SUCCESS then anOrder
//contains new order
//otherwise return contains fileIO error

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder) {
	if(myIO.getNext(anOrder))
		return SUCCESS;
	b_WaiterIsFinished = true;
	return NO_ORDERS;
}



//contains a loop that will get orders from filename one at a time
//then puts them in order_in_Q then signals baker(s) using cv_order_inQ
//so they can be consumed by baker(s)
//when finished exits loop and signals baker(s) using cv_order_inQ that
//it is done using b_WaiterIsFinished
void Waiter::beWaiter() {
	ORDER next_order;
	while (getNext(next_order)) {
		lock_guard<mutex> using_pos_system(mutex_order_inQ);
		order_in_Q.push(next_order);
	} // failed getnext should set waiterisfinished
	cv_order_inQ.notify_all();
}


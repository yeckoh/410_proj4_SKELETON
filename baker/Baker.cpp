/*
 * auth: wchang 00960978
 * prof: K. Perkins
 * proj: sp20 cs410 - p4: condition_variables, threads and stuff
 * date: 8 Apr. 2k20
 * desc: define what a baker does and how to box donuts
 */
#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"
#include "../includes/constants.h"

using namespace std;


Baker::Baker(int id):id(id) {}

Baker::~Baker() {}

// given an order, pack GLAZED donuts (as is directed by constants.h) into boxes holding 12 or less
void Baker::bake_and_box(ORDER &anOrder) {
	Box box_of_donuts;
	DONUT fresh_donut;
	fresh_donut.myType = GLAZED;
	anOrder.boxes.push_back(box_of_donuts);
	int donuts_left = anOrder.number_donuts;
	while(donuts_left > 0) {
		if(anOrder.boxes.back().addDonut(fresh_donut))
			--donuts_left;
		if(anOrder.boxes.back().size() == 12) // rest in rip baker's dozen
			anOrder.boxes.push_back(box_of_donuts);
	}
}

// sleep on the job until there's something on the ticket rack. Then panic and lock waiter out of the pos_system.
void Baker::beBaker() {
	do {
		unique_lock<mutex> ticket_was_printed(mutex_order_inQ);
		while (!b_WaiterIsFinished && order_in_Q.empty())
			cv_order_inQ.wait(ticket_was_printed);

		ORDER current_order;
		if (!order_in_Q.empty()) {
			current_order = order_in_Q.front();
			order_in_Q.pop();
			if (current_order.number_donuts < 0)
				continue; // no free boxes for dumb orders
			bake_and_box(current_order);
			lock_guard<mutex> sending_ticket_to_window(mutex_order_outQ);
			order_out_Vector.push_back(current_order);
		}
	}
	while (!b_WaiterIsFinished || !order_in_Q.empty());
}

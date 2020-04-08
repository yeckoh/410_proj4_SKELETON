#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"

using namespace std;


//id is just a number used to identify this particular baker
//(used with PRINT statements)
Baker::Baker(int id):id(id) {}

Baker::~Baker() {}


//bake, box and append to anOrder.boxes vector
//if order has 13 donuts there should be 2 boxes
//1 with 12 donuts, 1 with 1 donut
void Baker::bake_and_box(ORDER &anOrder) {
	Box box_of_donuts;
	DONUT fresh_donut;
	fresh_donut.myType = GLAZED;

	for(int i = anOrder.number_donuts; i > 0; --i) {
		if (!box_of_donuts.addDonut(fresh_donut)) // missed opportunity to use baker's dozens
			anOrder.boxes.push_back(box_of_donuts);
		box_of_donuts.clear();
	}
	// package last nonfull. Can be empty like this, oof!
	anOrder.boxes.push_back(box_of_donuts);
}

//as long as there are orders in order_in_Q then
//for each order:
//	create box(es) filled with number of donuts in the order
//  then place finished order on order_outvector
//  if waiter is finished (b_WaiterIsFinished) then
//  finish up remaining orders in order_in_Q and exit
//
//You will use cv_order_inQ to be notified by waiter
//when either order_in_Q.size() > 0 or b_WaiterIsFinished == true
//hint: wait for something to be in order_in_Q or b_WaiterIsFinished == true
void Baker::beBaker() {
	while(!b_WaiterIsFinished && !order_in_Q.empty()) {
		unique_lock<mutex> ticket_rack_is_full(mutex_order_inQ);
		// pretend to work until the waiters put 5million tickets in at once
		// then panic and tell the front the pos_sys is dead now
		while (!b_WaiterIsFinished || order_in_Q.empty())
			cv_order_inQ.wait(ticket_rack_is_full);

		ORDER current_order;
		while (!order_in_Q.empty) {
			lock_guard<mutex> working_on_this_ticket(mutex_order_outQ); // "don't mess me up, i got this ticket"
			current_order = order_in_Q.front();
			order_in_Q.pop(); // wewlad no data is returned, 2 operations for this
			// ticket_rack_is_full.unlock(); // let waiters put more orders in
			bake_and_box(current_order);
			order_out_Vector.push_back(current_order);
		}
	}
}


/*
 * BOX
 *
 * //holds up to 12 donuts (no more)
private:
	//holds the donuts
	std::vector<DONUT> donuts;

	//clear it out
	void clear();

	//how many in the box
	int size();

	//returns true if there is space in box and donut was added
	//otherwise false (cant fit more than DOZEN)
	bool addDonut(DONUT &adonut);
 *
 */


/*
 * ORDER struct
 * 	std::vector<Box> boxes;
	int order_number = UNINITIALIZED;
	int number_donuts = UNINITIALIZED;
 *
 */

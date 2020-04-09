/*
 * 		 00960978
 * prof: K. Perkins
 * proj: sp20 cs410 - p4: condition_variables, threads and stuff
 * date: 8 Apr. 2k20
 */
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>

#include "../includes/box.h"
#include "../includes/constants.h"
#include "../includes/waiter.h"
#include "../includes/baker.h"
#include "../includes/datastructs.h"
#include "../includes/PRINT.h"

using namespace std;

//*************************************************
//NOTE:  most of these globals are needed
//by both bakers and waiters, so be sure to include
//externs.h in appropriate cpp implementations
//*************************************************
//the mutexes
mutex mutex_order_inQ;
mutex mutex_order_outQ;

//the condition variable
condition_variable cv_order_inQ;

//when true its time for the baker to quit
bool b_WaiterIsFinished = false;	

//where orders are stored
queue<ORDER> order_in_Q;
vector<ORDER> order_out_Vector;

//used to make PRINT statements work properly
mutex printMutex;

//*************************************************
//runs waiter until orders all read and placed
//on order_in_Q then exits
void doWaiter(int id,string fn) {
	Waiter myWaiter(id,fn);
	myWaiter.beWaiter();
}

//takes orders from order_in_Q, processes
//them and then puts them on order_out_Vector
void doBaker(int id) {
	Baker myBaker(id);
	myBaker.beBaker();
}

//prints what is in order_out_Vector
//DO NOT CALL THIS WHEN MULTIPLE THREADS ARE ACCESSING order_out_Vector
void audit_results() {
	std::vector<ORDER>::iterator itOrder;
	std::vector<Box>::iterator itBox;

	int total_donuts = 0;
	int total_orders = 0;
	int total_boxes  = 0;

	//first go through every order
	for (itOrder = order_out_Vector.begin(); itOrder != order_out_Vector.end(); itOrder++) {
		int numDonuts = 0;
		total_orders++;

		//for each order go through all the boxes and add up all the donuts
		for (itBox = (itOrder->boxes).begin(); itBox != (itOrder->boxes).end(); ++itBox) {
			total_boxes++;
			numDonuts += itBox->size();
			total_donuts += itBox->size();
		}

		//if one order was screwed up say so
		if (numDonuts != itOrder->number_donuts) 
			PRINT6("ERROR Order", itOrder->order_number, " Expected ", itOrder->number_donuts, " found ", numDonuts);
	}

	PRINT2("Total donuts made   = ", total_donuts);
	PRINT2("Total number boxes  = ", total_boxes);
	PRINT2("Total orders filled = ", total_orders);
}

int main()
{
//#define WEEKDAY
#ifdef WEEKDAY
	thread t1(doWaiter, 1, "in1.txt");
	thread t2(doBaker, 1);
	thread t3(doBaker, 2);

	t1.join();
	t2.join();
	t3.join();
#endif
#ifndef WEEKDAY
//	thread t3(doWaiter, 1, "in1.txt");
//	thread t10(doWaiter, 2, "in2.txt");
	thread t1000(doWaiter, 3, "in3.txt");
	thread t4(doBaker, 4);
	thread t5(doBaker, 5);
	thread t6(doBaker, 6);
	//thread t600(doBaker, 123);

//	t3.join();
//	t10.join();
	t1000.join();
	//t600.join();
	t4.join();
	t5.join();
	t6.join();
#endif

	audit_results();
	return SUCCESS;
}


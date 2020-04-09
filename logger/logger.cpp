/*
 *		 00960978
 * prof: K. Perkins
 * proj: sp20 cs410 - p4: condition_variables, threads and stuff
 * date: 8 Apr. 2k20
 * desc: define how to clear and write to the logfile
 */

#include "../includes/logger.h"
using namespace std;

Logger::Logger(std::string filename) {
	this->filename = filename;
}

Logger::~Logger() {
}

//open close and clear the log file
void Logger::clearlogfile() {
	lock_guard<mutex> scope_lock(m);
	myFile.open(filename, std::fstream::trunc);

	//close file
	if (myFile.is_open())
		myFile.close();
}

// write to the logfile, or do nothing if we can't open the file
void Logger::log(std::string data) {
	lock_guard<mutex> scope_lock(m);
	myFile.open(filename, std::fstream::app);
	if (!myFile.is_open())
		return;

	std::string myline;

	myFile << data;

	//close file
	if (myFile.is_open())
		myFile.close();
}

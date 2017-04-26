//============================================================================
// Name        : Semprarrolar.cpp
// Author      : Mariana Aguiar, T5E201605904
// Version     :
// Copyright   : Your copyright notice
// Description : Projeto Semprarrolar
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Line{
	int id_num;
	int freq;
	vector<string> stops;
	vector<int> times;
};

struct Driver{
	int id_num;
	string name;
	int max_day;
	int max_week;
	int min_rest;
};

struct Clock{
	int h;
	int min;
};

typedef void  (*MenuOption)();

// In memory data structures
static vector<Driver> drivers;
static vector<Line> lines;
static map<string,MenuOption> menu;
const int day_start = 8;
const int day_end = 20;
string linesFilepath;
string driversFilepath;
static bool linesChanged = false;
static bool driversChanged = false;

// Helpers for string processing
void trimstring(string &s){
	s = s.substr(s.find_first_not_of(" "));
	s = s.substr(0, s.find_last_not_of(" ")+1);
}

void normalize(string &s){
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	trimstring(s);
}

// Generic functions to process files
void next(string &piece, string &line, string separator){
	int temp = line.find_first_of(separator);
	if(temp == string::npos){
		piece = line;
		line = "";
	}else{
		piece = line.substr(0,temp);
		line=line.substr(temp+1, line.length() - 1);
	}
	trimstring(piece);
}

void next(string &piece, string &line){
	next(piece,line,";");
}

void next(int &elem, string &piece, string separator){
	string elemstring;

	next(elemstring,piece,separator);
	elem = stoi(elemstring,nullptr);
}

// Functions to process lines' file
Line readLine(string &l){
	Line newline;
	string piece;

	next(newline.id_num,l,";");
	next(newline.freq,l,";");

	next(piece,l);
	while(piece!=""){
		string elem;
		next(elem,piece,",");
		newline.stops.push_back(elem);
	}
	next(piece,l);
	while(piece!=""){
		int elem;
		next(elem,piece,",");
		newline.times.push_back(elem);
	}
	return newline;
}

Driver readDriver(string &d){
	Driver newdriver;

	next(newdriver.id_num,d,";");
	next(newdriver.name,d);
	next(newdriver.max_day,d,";");
	next(newdriver.max_week,d,";");
	next(newdriver.min_rest,d,";");
	return newdriver;
}

void loadFiles(){
	ifstream linesfile;
	ifstream driversfile;
	linesFilepath = "lines.txt";
	driversFilepath = "drivers.txt";
	string l;
	string d;

	//	cout << "Insert lines's file name" << endl;
	//	getline(cin,linespath);
	linesfile.open(linesFilepath);

	while(linesfile.fail()){
		cout << "Error opening file. Try again.";
		cout << "Insert lines's file name" << endl;
		getline(cin,linesFilepath);
		linesfile.open(linesFilepath);
	}
	while(!linesfile.eof()) {
		getline(linesfile,l);
		lines.push_back(readLine(l));
	}
	linesfile.close();

	//	cout << "Insert drivers' file name" << endl;
	//	getline(cin,driverspath);
	driversfile.open(driversFilepath);

	while(driversfile.fail()){
		cout << "Error opening file. Try again.";
		cout << "Insert drivers' file name" << endl;
		getline(cin,driversFilepath);
		driversfile.open(driversFilepath);
	}

	while(!driversfile.eof()) {
		getline(driversfile,d);
		drivers.push_back(readDriver(d));
	}
	driversfile.close();
}

void saveLines() {
	ofstream linesfile;
	if (linesChanged) {
		linesfile.open(linesFilepath, ofstream::out | ofstream::trunc);
		for (int i = 0; i < lines.size(); i++) {
			linesfile << lines.at(i).id_num << " ; " << lines.at(i).freq
					<< " ; ";
			for (int t = 0; t < lines.at(i).stops.size() - 1; t++) {
				linesfile << lines.at(i).stops.at(t) << ", ";
			}
			linesfile << lines.at(i).stops.at(lines.at(i).stops.size() - 1)
					<< "; ";
			for (int t = 0; t < lines.at(i).times.size() - 1; t++) {
				linesfile << lines.at(i).times.at(t) << ", ";
			}
			linesfile << lines.at(i).times.at(lines.at(i).times.size() - 1);
			if (i < lines.size() - 1)
				linesfile << endl;
		}
	}
	linesfile.close();
}

void saveDrivers() {
	ofstream driversFile;
	if (driversChanged) {
		driversFile.open(driversFilepath, ofstream::out | ofstream::trunc);
		for (int i = 0; i < drivers.size(); i++) {
			driversFile << drivers.at(i).id_num << " ; " << drivers.at(i).name
					<< " ; ";
			driversFile << drivers.at(i).max_day << " ; "
					<< drivers.at(i).max_week << " ; ";
			driversFile << drivers.at(i).min_rest;
			if (i < drivers.size() - 1)
				driversFile << endl;
		}
	}
	driversFile.close();
}

void saveFiles(){
	saveLines();
	saveDrivers();
}

/*
 * Functions called from the menu
 */
void linesShow(){
	for(int i=0; i<lines.size(); i++){
		cout << "id_number: " << lines.at(i).id_num << "   " << "frequency: " << lines.at(i).freq << "   " << "stops: ";
		for(int t=0; t<lines.at(i).stops.size()-1; t++){
			cout << lines.at(i).stops.at(t) << ", ";
		}
		cout << lines.at(i).stops.at(lines.at(i).stops.size()-1);
		cout << "   "<< "times: ";
		for(int t=0; t<lines.at(i).times.size()-1; t++){
			cout << lines.at(i).times.at(t) << ", ";
		}
		cout << lines.at(i).times.at(lines.at(i).times.size()-1);
		cout << endl;
	}
}

void validArg(int &variable){
	variable = 0;
	string foo;
	while(true){
		getline(cin,foo);
		try{
			variable = stoi(foo,nullptr);
		}
		catch(const std::invalid_argument& ia){
			cout << "Invalid. Reenter." << endl;
		}
		if(variable) break;
	}
}

int promptLineNumber(int &id_number) {
	int lineindex = -1;
	while (lineindex == -1) {
		cout << "Insert line's id_number: " << endl;
		validArg(id_number);
		cout << endl;
		for (int i = 0; i < lines.size(); i++) {
			if (lines.at(i).id_num == id_number) {
				lineindex = i;
				break;
			}
		}
		if (lineindex == -1)
			cout << "Not found!" << endl;
	}
	cout << "| ";
	return lineindex;
}

void linesCreate(){
	Line newline;
	string foo;
	int id_number;
	int freq;
	string stop;
	int time;
	vector<string> stops;
	vector<int> times;

	cout << "Enter id_number: "; validArg(id_number);
	cout << "Enter frequency: "; validArg(freq);
	cout << "Insert the stops:(Press enter to stop) ";
	while(true){
		getline(cin,stop);
		if(stop == "") break;
		stops.push_back(stop);
	}
	cout << "Insert the times:(Enter enter to stop) ";
	while(true){
		time = 0;
		getline(cin,foo);
		if(foo == "") break;
		while(true){
			time = 0;
			try{
				time = stoi(foo, nullptr);
			}
			catch(const std::invalid_argument& ia){
				cout << "Invalid. Reenter." << endl; getline(cin,foo);
			}
			if(time) break;
		}
		times.push_back(time);
	}
	newline.id_num = id_number;
	newline.freq = freq;
	newline.stops = stops;
	newline.times = times;
	lines.push_back(newline);
	linesChanged = true;
}

void linesUpdate(){
	string foo;
	int lineindex;
	int id_number;
	int newid_number;
	int newfreq;
	string newstop;
	int newtime;
	vector<string> newstops;
	vector<int> newtimes;
	lineindex = promptLineNumber(id_number);
	cout << "Line " << id_number << ": ";
	cout << "id_number: " << lines.at(lineindex).id_num << "   " << "frequency: " << lines.at(lineindex).freq << "   " << "stops: ";
	for(int t=0; t<lines.at(lineindex).stops.size()-1; t++){
		cout << lines.at(lineindex).stops.at(t) << ", ";
	}
	cout << lines.at(lineindex).stops.at(lines.at(lineindex).stops.size()-1);
	cout << "   "<< "times: ";
	for(int t=0; t<lines.at(lineindex).times.size()-1; t++){
		cout << lines.at(lineindex).times.at(t) << ", ";
	}
	cout << lines.at(lineindex).times.at(lines.at(lineindex).times.size()-1);
	cout << endl;
	cout << "Enter id_number: "; validArg(newid_number);
	cout << "Enter frequency";	validArg(newfreq);
	cout << "Insert the stops:(Press enter to stop)";
	while(true){
		getline(cin,newstop);
		if(newstop == "") break;
		newstops.push_back(newstop);
	}
	cout << "Insert the times:(Enter enter to stop)";
	while(true){
		newtime = 0;
		getline(cin,foo);
		if(foo == "") break;
		while(true){
			newtime = 0;
			try{
				newtime = stoi(foo, nullptr);
			}
			catch(const std::invalid_argument& ia){
				cout << "Invalid. Reenter." << endl; getline(cin,foo);
			}
			if(newtime) break;
		}
		newtimes.push_back(newtime);
	}
	lines.at(lineindex).id_num = newid_number;
	lines.at(lineindex).freq = newfreq;
	lines.at(lineindex).stops = newstops;
	lines.at(lineindex).times = newtimes;
	linesChanged = true;
}

void linesDelete(){
	int len = lines.size();
	int id_number;
	int lineindex;
	lineindex = promptLineNumber(id_number);
	for(int i=lineindex; i<len-1; i++){
		lines.at(i) = lines.at(i+1);
	}
	lines.resize(len-1);
	cout << "Line deleted!" << endl;
	linesChanged = true;
}


Clock addTime(int min, Clock time){
	Clock result;
	result.h = time.h;
	result.min= time.min + min;
	while(result.min >= 60){
		result.min = result.min - 60;
		result.h = result.h + 1;
	}
	return result ;
}

void printStops(int id_number, Line& line) {
	for (int i = 1; i <= line.stops.size(); i++) {
		cout << id_number << "." << i << " " << line.stops.at(i - 1) << " | ";
	}
	cout << endl << endl;
}

void printStopsHeader(const Line& line, int id_number, int direction) {
	string s;

	int start, end;
	if (direction == 1) {
		start = 1;
		end = line.stops.size()+1;
	} else {
		start = line.stops.size();
		end = 0;
	}
	for (int i = start; i != end; i=i+direction) {
		s = to_string(id_number) + "." + to_string(i);
		cout << setw(12) << s;
	}
	cout << endl;
	for(int i=1; i<=line.stops.size(); i++){
		cout << setw(12) << "-----";
	}
	cout << endl;
}

void printTimes(vector<Clock> time, int freq) {
	bool end = false;
	while (!end) {
		for (int i = 0; i < time.size(); i++) {
			string v;
			if (time.at(i).h < 10) {
				v = "0" + to_string(time.at(i).h) + ":";
			} else
				v = to_string(time.at(i).h) + ":";

			if (time.at(i).min < 10) {
				v = v + "0" + to_string(time.at(i).min);
			} else
				v = v + to_string(time.at(i).min);

			cout << setw(12) << v;
		}
		cout << endl;
		for (int i = 0; i < time.size(); i++) {
			time.at(i) = addTime(freq, time.at(i));
			if (time.at(i).h >= day_end)
				end = true;
		}
	}
}

void linesSchedules(){
	int sum;
	Clock start_time;
	start_time.h = day_start;
	start_time.min = 0;
	int id_number;
	string s;
	vector<Clock> time;
	vector<Clock> time2;

	Line line = lines.at(promptLineNumber(id_number));
	printStops(id_number, line);
	int freq = line.freq;

	cout << "Direction: " << line.stops.at(0);
	cout << " to " << line.stops.at(line.stops.size()-1) << endl << endl;
	printStopsHeader(line, id_number, 1);

	time.push_back(start_time);
	sum = 0;
	for(int i=0; i<line.times.size(); i++){
		sum = sum + line.times.at(i);
		time.push_back(addTime(sum , start_time));
	}
	printTimes(time, freq);
	cout << endl << endl;



	cout << "Direction: " << line.stops.at(line.stops.size()-1);
	cout << " to " << line.stops.at(0) << endl << endl;
	printStopsHeader(line, id_number, -1);

	time2.push_back(start_time);
	sum = 0;
	for(int i=line.times.size()-1 ; i>=0; i--){
		sum = sum + line.times.at(i);
		time2.push_back(addTime(sum , start_time));
	}
	printTimes(time2, freq);
}

void linesTravelTime(){
	string stop1, stop2;
	int linesindex = -1;
	int stop1index, stop2index;
	int duration = 0;
	bool found = false;
	vector<string> path;
	cout << "Enter first stop: "; getline(cin,stop1);
	cout << "Enter last stop: ";getline(cin,stop2);
	for(int i=0; i<lines.size(); i++){
		vector<string> stops = lines.at(i).stops;
		if(find(stops.begin(), stops.end(), stop1) != stops.end() &&
				find(stops.begin(), stops.end(), stop2) != stops.end()){
			linesindex = i;
			found = true;
			for(int t=0; t<stops.size(); t++){
				if(stops.at(t) == stop1){
					stop1index = t;
				}
				if(stops.at(t) == stop2){
					stop2index = t;
				}
			}
			break;
		}
	}
	if(found){
		if(stop1index < stop2index){
			for(int i=stop1index; i<=stop2index; i++){
				path.push_back(lines.at(linesindex).stops.at(i));
			}
			for(int i=stop1index; i<stop2index; i++){
				duration+= lines.at(linesindex).times.at(i);
			}
			cout << "Path:";
			for(int i=0; i<path.size(); i++){
				cout << "  " << path.at(i);
			}
			cout << endl;
			cout << "Duration: " << duration << endl;
		}else{
			for(int i=stop1index; i>=stop2index; i--){
				path.push_back(lines.at(linesindex).stops.at(i));
			}
			for(int i=stop1index-1; i>=stop2index; i--){
				duration+= lines.at(linesindex).times.at(i);
			}
			cout << "Path:";
			for(int i=0; i<path.size(); i++){
				cout << "  " << path.at(i);
			}
			cout << endl;
			cout << "Duration: " << duration << endl;
		}
	}
}

void linesStopLines(){
	string stop;
	bool found = false;
	vector<int> linesInStop;
	cout << "Insert name of the stop: "; getline(cin,stop);
	for(int i=0; i<lines.size(); i++){
		if(find(lines.at(i).stops.begin(), lines.at(i).stops.end(), stop) != lines.at(i).stops.end()){
			linesInStop.push_back(lines.at(i).id_num);
			found = true;
		}
	}
	if(found){
		cout << "Line(s):";
		for(int i = 0; i<linesInStop.size(); i++){
			cout << "  " << linesInStop.at(i);
		}
		cout << endl;
	}else cout << "Stop not found!" << endl;
}

void printTable(int travel, int duration, int index, string stop, string begin, string end){
	Clock time;
	int freq = lines.at(index).freq;
	vector <string> stops = lines.at(index).stops;
	vector<Clock> empty;
	vector<Clock> temp;
	time.h = day_start;
	time.min = 0;
	vector < vector < Clock > > hours;
	time = addTime(travel, time);
	temp.push_back(time);
	while(true){
		int foo = time.h;
		time = addTime(freq,time);
		Clock foo2 = addTime(duration-travel,time);
		if(foo2.h >= day_end){
			hours.push_back(temp);
			hours.push_back(empty);
			break;
		}
		else if(time.h > foo){
			hours.push_back(temp);
			temp.resize(0);
			temp.push_back(time);
		}else temp.push_back(time);
	}

	cout << "Line " << lines.at(index).id_num << " - " << stop << endl;
	cout << "Direction: " << begin;
	cout << " to " << end << endl << endl;
	for(int i=day_start; i<day_end; i++){
		cout << setw(6) << i;
	}
	cout << endl;
	for(int i=day_start; i<day_end; i++){
		cout << setw(6) << "--";
	}
	int size = 0;
	for(int i = 0; i < hours.size(); i++){
		size = size + hours.at(i).size();
	}
	cout << endl;
	int t=0;
	while(size>0){
		for(int i=0; i<(day_end-day_start); i++){
			string s;
			if(t>=hours.at(i).size()){
				cout << setw(6) << " ";
			}
			else if(hours.at(i).at(t).min < 10){
				s = "0" + to_string(hours.at(i).at(t).min);
				cout << setw(6) << s;
				size--;
			}else{
				s = to_string(hours.at(i).at(t).min);
				cout << setw(6) << s;
				size--;
			}
		}
		cout << endl;
		t++;
	}
	cout << endl;
}

void linesStopTimetable(){
	vector < vector < Clock > > hours;
	string stop;
	int travel = 0;
	int duration = 0;
	vector<Clock> temp;
	vector<Clock> empty;
	vector<int> indexs;
	while(indexs.size() == 0){
		cout << "Enter stop: ";
		getline(cin, stop);
		for(int i=0; i<lines.size(); i++){
			vector<string> stops = lines.at(i).stops;
			if(find(stops.begin(), stops.end(), stop) != stops.end()){
				indexs.push_back(i);
			}
		}
		if(indexs.size()==0) cout << "Not found!" << endl;
	}
	cout << endl << endl;
	for(int u = 0; u<indexs.size(); u++){
		vector <string> stops = lines.at(indexs.at(u)).stops;
		for(int i=0; i<lines.at(indexs.at(u)).times.size(); i++){
			if(stops.at(i) == stop) break;
			else{
				travel = travel + lines.at(indexs.at(u)).times.at(i);
			}
		}
		for(int i=0; i<lines.at(indexs.at(u)).times.size(); i++){
			duration = duration + lines.at(indexs.at(u)).times.at(i);
		}
		printTable(travel,duration, indexs.at(u), stop,stops.at(0),stops.at(stops.size()-1));
		travel = 0;
		for(int i= lines.at(indexs.at(u)).times.size() -1 ; i>=0; i--){
			if(stops.at(i+1) == stop) break;
			else{
				travel = travel + lines.at(indexs.at(u)).times.at(i);
			}
		}
		printTable(travel,duration,indexs.at(u), stop, stops.at(stops.size()-1), stops.at(0));
	}
}

void driversShow(){
	for(int i=0; i<drivers.size(); i++){
		cout << "id number: " << drivers.at(i).id_num << "    ";
		cout << "name: " << drivers.at(i).name << "    ";
		cout << "max hours per day: " << drivers.at(i).max_day << "    ";
		cout << "max hours per week: " << drivers.at(i).max_week << "    ";
		cout << "min hours of rest: " << drivers.at(i).min_rest<< endl;
	}

}

void driversCreate(){
	Driver newdriver;
	string foo;
	cout << "Enter id_number: "; validArg(newdriver.id_num);
	cout << "Enter name: "; getline(cin, newdriver.name);
	cout << "Enter max hours per day: "; validArg(newdriver.max_day);
	cout << "Enter max hours per week: "; validArg(newdriver.max_week);
	cout << "Enter min hours of rest: "; validArg(newdriver.min_rest);
	drivers.push_back(newdriver);
	driversChanged = true;
}

int promptDriverNumber(int &id_number) {
	int driverindex = -1;
	while (driverindex == -1) {
		cout << "Insert driver's id_number: " << endl;
		validArg(id_number);
		for (int i = 0; i < drivers.size(); i++) {
			if (drivers.at(i).id_num == id_number) {
				driverindex = i;
				break;
			}
		}
		if (driverindex == -1)
			cout << "Not found!" << endl;
	}
	return driverindex;
}

void driversUpdate(){
	int id_number;
	string newname;
	int driverindex = promptDriverNumber(id_number);
	Driver driver = drivers.at(driverindex);
	cout << "Driver " << id_number << ": ";
	cout << "id number: " << driver.id_num << "    ";
	cout << "name: " << driver.name << "    ";
	cout << "max hours per day: " << driver.max_day << "    ";
	cout << "max hours per week: " << driver.max_week << "    ";
	cout << "min hours of rest: " << driver.min_rest<< endl;
	cout << "Enter id_number: "; validArg(driver.id_num);
	cout << "Enter name: "; getline(cin, driver.name);
	cout << "Enter max hours per day: "; validArg(driver.max_day);
	cout << "Enter max hours per week: "; validArg(driver.max_week);
	cout << "Enter min hours of rest: "; validArg(driver.min_rest);
	driversChanged = true;
}

void driversDelete(){
	int len = drivers.size();
	int id_number;
	int driverindex = promptDriverNumber(id_number);
	for(int i=driverindex; i<len-1; i++){
		drivers.at(i) = drivers.at(i+1);
	}
	drivers.resize(len-1);
	cout << "Driver deleted!" << endl;
	driversChanged = true;
}

void exitMenu(){
	cout << "Bye bye";
	saveFiles();
	exit(0);
}

/*
 * Add functions to menu
 * */

void setupMenu(){
	menu["lines show"] = linesShow;
	menu["lines create"] = linesCreate;
	menu["lines update"] = linesUpdate;
	menu["lines delete"] = linesDelete;
	menu["lines schedules"] = linesSchedules;
	menu["lines travel time"] = linesTravelTime;
	menu["lines stop lines"] = linesStopLines;
	menu["lines stop timetable"] = linesStopTimetable;
	menu["drivers show"] = driversShow;
	menu["drivers create"] = driversCreate;
	menu["drivers update"] = driversUpdate;
	menu["drivers delete"] = driversDelete;
	menu["exit"] = exitMenu;
}

void setupMenuShortcuts(){
	menu["ls"] = linesShow;
	menu["lc"] = linesCreate;
	menu["lu"] = linesUpdate;
	menu["ld"] = linesDelete;
	menu["lsch"] = linesSchedules;
	menu["ltt"] = linesTravelTime;
	menu["lsl"] = linesStopLines;
	menu["lst"] = linesStopTimetable;
	menu["ds"] = driversShow;
	menu["dc"] = driversCreate;
	menu["du"] = driversUpdate;
	menu["dd"] = driversDelete;
	menu["e"] = exitMenu;
}

void displayMenu(){
	cout << "\n";
	cout << "Lines" << endl;
	cout << "     Show, Create, Update, Delete" << endl;
	cout << "     Schedules, Travel time" << endl;
	cout << "     Stop Lines, Stop Timetable" << endl;
	cout << "Drivers" << endl;
	cout << "     Show, Create, Update, Delete" << endl;
	cout << "Exit" << endl;
}

int main(int argc, char* argv[]) {
	loadFiles();
	setupMenu();
	setupMenuShortcuts();
	string command;
	string foo;
	while(true){
		displayMenu();
		cout << "Command:";
		getline(cin,command);
		normalize(command);
		if(menu.find(command) != menu.end()){
			menu[command]();
		}else cout << "Invalid";
		cout << "\nPress enter to continue";
		getline(cin,foo);
	}
}

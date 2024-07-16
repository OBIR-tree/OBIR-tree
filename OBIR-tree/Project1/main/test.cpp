#include "include/OBIR-tree.h"
#include <chrono>

using namespace std;

int main()
{
	auto _start = chrono::high_resolution_clock::now();
	OBIRtree* obirtree = new OBIRtree("../Data/output.txt","../Data/10.txt");
	auto _end = chrono::high_resolution_clock::now();
	auto runtime = chrono::duration_cast<chrono::nanoseconds>(_end - _start);
	double total_time2 = double(runtime.count()) * chrono::nanoseconds::period::num / (chrono::nanoseconds::period::den);
    cout << "build tree:"<<total_time2 << endl;
	vector<chrono::nanoseconds> duration_time;
	chrono::nanoseconds total_duration = chrono::nanoseconds::zero();
    cout << "build tree:"<<total_time2 << endl;
	fstream query_file;
    query_file.open("../Data/query.txt");
	string line;
	double x = 0, y = 0;
	string text;
	while (getline(query_file, line))
	{
		istringstream iss(line);
		iss >> text >> x >> y;
		string new_string;
		if (text.size() > 5)
		{
			new_string = text.substr(0, 5);
		}
		if (text.size() < 5)
		{
			new_string = text + string(5 - text.size(), 'X');
		}
		if (text.size() == 5)
		{
			new_string = text;
		}
		chrono::nanoseconds temp_time = obirtree->getRunTime(x, y, new_string);
		duration_time.push_back(temp_time);
	}
	for (auto& duration : duration_time)
	{
		total_duration += duration;
	}

	double total_time = double(total_duration.count()) * chrono::nanoseconds::period::num / (chrono::nanoseconds::period::den);
	double total = (total_time)/50;
    double interaction = (obirtree->stash_num*50)/50;
    cout << "Time of initialization:"<<total_time2 << endl;
	cout << "K:" << search_K << ' ' << "Query time:" << ' ' << total+ (interaction /1000)<< endl;
//	cout << "Time of interaction£º" << interaction << endl;
//    cout << "´óÐ¡£º" << sizeof(irtree)<< endl;
}
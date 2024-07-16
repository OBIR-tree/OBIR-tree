#include "Ptree.h"
#include <chrono>

using namespace std;

int main()
{
	Ptree* irtree = new Ptree();
	vector<chrono::nanoseconds> duration_time;
	chrono::nanoseconds total_duration = chrono::nanoseconds::zero();
	
	fstream query_file;
	query_file.open("/home/hhhead/桌面/Project1/test1/data/query4.txt");
	string line;
	double x = 0, y = 0;
	string text;
	while (getline(query_file, line))
	{
		istringstream iss(line);
		iss >> x >> y >> text;
		chrono::nanoseconds temp_time = irtree->getRunTime(x, y, text);
		duration_time.push_back(temp_time);
	}
	for (auto& duration : duration_time)
	{
		total_duration += duration;
	}

	double total_time = double(total_duration.count()) * chrono::nanoseconds::period::num / (chrono::nanoseconds::period::den);
	double total = (1999 / total_time) * 60;
	cout << "查询K为" << search_K << ' ' << "平均每分钟处理查询条目为" << ' ' << total;
	
}
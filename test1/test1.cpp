#include <iostream>
#include<thread>
#include<chrono>
#include <mutex>
#include <map>
#include <algorithm>
#include <vector>
static std::mutex mtx_cout;

namespace task_1
{
	std::mutex mutex;


	class pcout : public std::ostream {
	public:
		pcout(std::streambuf* string) : std::ios(string), std::ostream(string) {
			std::lock_guard<std::mutex> lock(mutex);
		}
	} pcout(std::cout.rdbuf());



	void start_pcout1()
	{
		pcout << "thread_id: " << std::this_thread::get_id() << std::endl;
		pcout << "thread_stop_id: " << std::this_thread::get_id() << std::endl;
		for (size_t i = 0; i < 100; i++)
		{
			pcout << "rrrrrrrr";
		}
		pcout << std::endl;
	}

	void start_pcout2()
	{
		pcout << "thread_id: " << std::this_thread::get_id() << std::endl;
		pcout << "thread_stop_id: " << std::this_thread::get_id() << std::endl;
		for (size_t i = 0; i < 100; i++)
		{
			pcout << "ppppp";
		}
		pcout << std::endl;
	}

	void start()
	{
		std::thread thread_num_1(start_pcout1);
		std::thread thread_num_2(start_pcout2);
		thread_num_1.join();
		thread_num_2.join();
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		system("cls");
	}
}

namespace task_2
{
	long find_simple(unsigned long& sim, unsigned long& nn) {
		unsigned long n = 4;
		unsigned long lpr = 0;
		bool simple;
		while (sim != nn) {
			simple = true;
			for (int i = 2; i <= sqrt(n); i++) {
				if ((n % i) == 0) { simple = false; break; }
			}
			if (simple) { ++sim; lpr = n; }
			++n;
		}

		return lpr;
	}
	void start()
	{
		long simple;
		unsigned long sim = 3;
		unsigned long start_vaule = 1000000;
		std::thread thread_num_1([&]() { simple = find_simple(sim, start_vaule); });
		thread_num_1.detach();
		while (sim < start_vaule) {
			std::cout << (start_vaule - sim) << std::endl;
		}
		system("cls");
		task_1::pcout << std::endl << "simple: " << simple;

		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		system("cls");
	}
}
namespace task_3
{
	static std::mutex m;
	void add_item(std::vector<std::pair<size_t, char>>& house)
	{
		while (!house.empty())
		{
			srand(time(0));
			size_t cost_element = (rand() % 90 + 10); // cost
			char name_element = (rand() % 25 + 65); // A-Z 
			{
				std::lock_guard<std::mutex> lg(m);
				house.push_back({ cost_element, name_element });
				std::cout << "+++ owner +++ | brought: "
					<< " | name: "
					<< name_element
					<< " | price: "
					<< cost_element
					<< " | count: "
					<< house.size()
					<< std::endl;
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	};

	void steal_item(std::vector<std::pair<size_t, char>>& house)
	{
		while (!house.empty())
		{
			auto dell_element = std::max_element(house.begin(), house.end());
			std::pair<size_t, char> temp = *dell_element;
			{
				std::lock_guard<std::mutex> lg(m);

				std::cout << "---  thief   --- | stole : "
					<< " | name: "
					<< temp.second
					<< " | price: "
					<< temp.first
					<< " | count: "
					<< house.size()
					<< std::endl;

				house.erase(dell_element);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}

	void start()
	{
		std::vector<std::pair<size_t, char>> house;
		house.reserve(100);
		house.push_back({ 10,'A' });
		house.push_back({ 20,'B' });
		house.push_back({ 30,'C' });
		house.push_back({ 40,'D' });
		house.push_back({ 50,'E' });
		house.push_back({ 60,'F' });
		house.push_back({ 70,'G' });
		house.push_back({ 80,'H' });
		house.push_back({ 90,'I' });
		house.push_back({ 55,'J' });

		std::thread owner(add_item, ref(house));
		std::thread thief(steal_item, ref(house));

		owner.join();
		thief.join();
	}
}



int main()
{
	//task_1::start();
	//task_2::start();
	task_3::start();
}
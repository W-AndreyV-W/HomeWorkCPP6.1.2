#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <vector>
#include <windows.h>

//  Заполнение вектора случайными числами.
void vector_random(std::vector<int>& arr, int size_vector) {

	std::random_device rv;
	std::mt19937 gen(rv());
	std::uniform_int_distribution<> dist(0, size_vector);

	for (int i = 0; i < size_vector; i++) {

		arr.at(i) = dist(gen);
	}
}

void summ(std::vector<int>& arr, int num_1, int num_2, int i) {

	arr.at(i) = num_1 + num_2;
}

//  Параллельное суммирование.
void addition(std::vector<int>& vect_1, std::vector <int>& vect_2, std::vector<std::thread>& flow, int size_vector, int flow_num) {

	std::vector <int> arr(size_vector);
	int j = 0;

	for (int i = 0; i < size_vector; i++) {
		
		if (i >= flow_num) {

			flow.at(j).join();
		}

		flow.at(j) = std::thread(summ, std::ref(arr), vect_1.at(i), vect_2.at(i), i);
		
		j++;
		
		if (j == flow_num) {

			j = 0;
		}
	}

	for (int i = 0; i < flow_num; i++) {

		if (flow.at(i).joinable()) {

			flow.at(i).join();
		}
	}
}

//  Вычисление времени суммирования векторов.
double execution_time(int& best, double& min_time, std::vector<int>& vect_1, std::vector <int>& vect_2, std::vector<std::thread>& flow, int size_vector, int flow_num) {
	
	auto start = std::chrono::steady_clock::now();
	
	addition(vect_1, vect_2, flow, size_vector, flow_num);

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> exe_time = end - start;

	if (min_time == 0 || min_time > exe_time.count() / size_vector) {

		min_time = exe_time.count() / size_vector;
		best = flow_num;
	}

	return exe_time.count();
}

int main()
{
	setlocale(LC_ALL, "Russian");

	double min_time = 0;
	int best = 0;
	
	auto num_flow = std::thread::hardware_concurrency();

	int size_1 = 1000;
	int size_2 = 10000;
	int size_3 = 100000;
	int size_4 = 1000000;

	int flow_1 = 1;
	int flow_2 = 2;
	int flow_3 = 4;
	int flow_4 = 8;
	int flow_5 = 16;

	std::vector<std::thread> flow(flow_5);
	std::vector<int> vect_1(size_4);
	std::vector<int> vect_2(size_4);

	vector_random(vect_1, size_4);
	vector_random(vect_2, size_4);

	std::cout << "Количество аппаратных ядер: " << num_flow << "\n" << std::endl;
	std::cout << "\t\t" << size_1 << "\t\t" << size_2 << "\t\t" << size_3 << "\t\t" << size_4 << std::endl;
	std::cout << flow_1 << " потоков \t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_1, flow_1) << " s\t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_2, flow_1) << " s\t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_3, flow_1) << " s\t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_4, flow_1) << " s\t";

	std::cout << std::endl;

	std::cout << flow_2 << " потоков \t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_1, flow_2) << " s\t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_2, flow_2) << " s\t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_3, flow_2) << " s\t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_4, flow_2) << " s\t";

	std::cout << std::endl;

	std::cout << flow_3 << " потоков \t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_1, flow_3) << " s\t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_2, flow_3) << " s\t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_3, flow_3) << " s\t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_4, flow_3) << " s\t";

	std::cout << std::endl;

	std::cout << flow_4 << " потоков \t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_1, flow_4) << " s\t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_2, flow_4) << " s\t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_3, flow_4) << " s\t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_4, flow_4) << " s\t";

	std::cout << std::endl;

	std::cout << flow_5 << " потоков \t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_1, flow_5) << " s\t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_2, flow_5) << " s\t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_3, flow_5) << " s\t";

	std::cout << execution_time(best, min_time, vect_1, vect_2, flow, size_4, flow_5) << " s\t";

	std::cout << std::endl;

	std::cout << "Лучший результат: " << best << " потоков, время / на 1 поток - " << min_time << std::endl;
}
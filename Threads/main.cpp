#include<iostream>
#include<thread>
using namespace std;
using namespace std::chrono_literals;

bool finish = false;

void Hello()
{
	while (!finish)
	{
		cout << " Hello ";
	    std:this_thread::sleep_for(1s);
	}

}
void World()
{
	while (!finish)
	{
		cout << " World ";
		std::this_thread::sleep_for(1s);
	}
}
void main()
{
	setlocale(LC_ALL, "");
	/*std::thread hello_thread(Hello); //инициализация потоков при объявлении
	std::thread world_thread(World);*/

	std::thread hello_thread;//инициализация потоков после объявления
	std::thread world_thread;

	hello_thread = std::thread(Hello);
	world_thread = std::thread(World);
	cin.get();//ожидает нажатия энтер
	finish=true;

	hello_thread.join();//синхронизируют потоки
	world_thread.join();
}
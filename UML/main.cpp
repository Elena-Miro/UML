#include <iostream>
#include<conio.h>
using namespace std;
#define MIN_TANK_VAUME 20
#define MAX_TANK_VALUME 80
#define MIN_ENGINE_CONSUMPTION 3
#define MAX_ENGINE_CONSUMPTION 25
#define Escape 27
#define Enter 13
class Tank
{
	const unsigned int volume;//характеристика бака
	double fuel_level;
public:
	unsigned int get_volume()const
	{
		return this->volume;
	}
	double get_fuel_level()const
	{
		return fuel_level;
	}
	void fill(double fuel_level)
	{
		if (fuel_level <= 0)return;
		if (this->fuel_level + fuel_level <= volume)this->fuel_level += fuel_level;
		else this->fuel_level = volume;
	}
	Tank(unsigned int volume) :volume(volume >= MIN_TANK_VAUME && volume <= MAX_TANK_VALUME ? volume : 60)
	{//константный член класса можно проинициализироыать только в списке реализации конструктора(только в заголовке
		this->fuel_level = 0;
		cout << "Tank is resdy" << endl;

	}
	~Tank()
	{
		cout << "Tank is over" << endl;

	}
	void info()const
	{
		cout << "Tank volume:" << volume << " liters\n";
		cout << "Fuel level:" << fuel_level << " liters\n";
	}
};
class Engine
{
	double consumption;//расход на 100км
	double consumtion_per_second;//расход за секунду
	bool is_started;//заведен/заглушен
public:
	double get_consumption()const
	{
		return consumption;

	}
	double get_consumtion_per_second()const
	{
		return consumtion_per_second;
	}
	void set_consumtion(double consumption)
	{
		if (consumption >= MIN_ENGINE_CONSUMPTION && consumption <= MAX_ENGINE_CONSUMPTION)
			this->consumption = consumption;
		else
			this->consumption = 10;
		this->consumtion_per_second = this->consumption*3e-5;
	}
	void start()
	{
		is_started = true;
	}
	void stop()
	{
		is_started = false;
	}
	bool started()const
	{
		return is_started;
	}
	void info()const
	{
		cout << "Consumption per 100km: " << consumption << " liters\n";
		cout << "Consumption per second: " << consumtion_per_second << " liters\n";
	}
	Engine(double consumtion)
	{
		set_consumtion(consumtion);
		is_started = false;
		cout << "Engine is ready" << endl;
	}
	~Engine()
	{
		cout<< "Engine is over" << endl;
    }
};
class Car
{
	Engine engine;
	Tank tank;
	bool driver_inside;

public:
	Car(double engine_consumption, unsigned int tank_volume) :engine(engine_consumption), tank(tank_volume)
	{
		driver_inside = false;
		cout << "Your car is ready" << endl;
	}
	~Car()
	{
		cout << "Car is over" << endl;
	}
	void get_in()
	{
		
		driver_inside = true;
		panel();
	}
	void get_out()
	{
		driver_inside = false;
		system("CLS");
		cout << "You are out of your car" << endl;
	}
	void control_car()
	{
		cout << "Press Enter to get in" << endl;
		char key;
		do
		{
		  key = _getch();
		  switch (key)
		  {
		  case Enter:
			  if (driver_inside)get_out();
			  else get_in() ;//вход.выход из машины
			  break;
		  }
		} while (key != Escape);

	}
	void panel()const
	{
		system("CLS");
		cout << "Fuel level: " << tank.get_fuel_level() << " liters\n";
		cout << "Engine is " << (engine.started() ? "started" : "stopped") << endl;
	}
	void info()const
	{
		engine.info();
		tank.info();
	}
};
//#define TANK_CHECK
//#define ENGINE_CHECK
void main()
{
	setlocale(LC_ALL, "");
#ifdef TANK_CHECK
	Tank tank(40);
	tank.info();
	int fuel;
	while (true)
	{
		cout << "Введите объем топлива:"; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	}
#endif // TANK_CHECK

#ifdef ENGINE_CHECK
	Engine engine(20);
	engine.info();
#endif // ENGINE_CHECK
	Car bmw(12,60);
	//bmw.info();
	bmw.control_car();

}
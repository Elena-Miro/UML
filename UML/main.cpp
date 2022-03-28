#include <iostream>
#include<conio.h>
#include<thread>
#include<Windows.h>
using namespace std;
using namespace std::chrono_literals;
#define MIN_TANK_VAUME 20
#define MAX_TANK_VALUME 80
#define MIN_ENGINE_CONSUMPTION 3
#define MAX_ENGINE_CONSUMPTION 25
#define DEFOLT_ENGINE_CONSUMPTION 10
#define Escape 27
#define Enter 13
#define MAX_SPEED_LOW 150
#define MAX_SPEED_HIGH_LIMIT 300
#define DEFAULT_MAX_SPEED 250
#define MIN_ACCELLERATION 5
#define MAX_ACCELLERATION 25
#define DEFAULT_AACCELLERATION 12
class Tank //бак
{
	const unsigned int volume;//характеристика бака
	double fuel_level;//уровень топлива
public:
	unsigned int get_volume()const
	{
		return this->volume;
	}
	double get_fuel_level()const
	{
		return fuel_level;
	}
	void fill(double fuel_level)//заправивть бака
	{
		if (fuel_level <= 0)return;
		if (this->fuel_level + fuel_level <= volume)this->fuel_level += fuel_level;
		//if (fuel_level < 5)cout << "LOW FUEL";
		else this->fuel_level = volume;
	}
	double give_fuel(double amount)//оттдает топливо в движок
	{
		if (fuel_level - amount > 0)fuel_level -= amount;
		else
		{
			fuel_level = 0;
		}
		return fuel_level;
	}
	Tank(unsigned int volume) :volume(volume >= MIN_TANK_VAUME && volume <= MAX_TANK_VALUME ? volume : 60)
	{//константный член класса можно проинициализироыать только в списке реализации конструктора(только в заголовке
		this->fuel_level = 0;
		cout << "Tank is ready" << endl;

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
	void set_consumtion(double consumption)//расход топлива
	{
		if (consumption >= MIN_ENGINE_CONSUMPTION && consumption <= MAX_ENGINE_CONSUMPTION)
			this->consumption = consumption;
		else
			this->consumption = DEFOLT_ENGINE_CONSUMPTION;

		this->consumtion_per_second = this->consumption*3e-5;
		
	}
	void start()//запуск
	{
		is_started = true;
	}
	void stop()//остановка
	{
		is_started = false;
	}
	bool started()const//проверяет состояние двигателя
	{
		return is_started;
	}
	void correct_consumtion(int speed)
	{
		if (speed > 0 && speed <= 60 || speed >= 101 && speed <= 140)
			consumtion_per_second = 0.002;
		if(speed>=61 && speed<=100)consumtion_per_second = 0.0014;
		if (speed >= 141 && speed <= 200)consumtion_per_second = 0.0025;
		if (speed >= 201 && speed <= 250)consumtion_per_second = 0.003;
		if (speed == 0)consumtion_per_second = 0.003;
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
	Engine engine; //двигатель
	Tank tank;//бак
	bool driver_inside;//водитель внутри
	int speed;//скорость
	const int MAX_SPEED;//макс скорость
	int accelleration;//ускорение
	struct Control
	{
		std::thread panel_thread;//отображ панели
		std::thread engine_idle_thread;//холостой ход
		std::thread free_wheeling_thread;//замедляет скорость автомобиля
	}control;

public:
	Car(double engine_consumption, unsigned int tank_volume, int max_speed=DEFAULT_MAX_SPEED,int accelleration=10)
		:engine(engine_consumption), tank(tank_volume),
		MAX_SPEED(max_speed>=MAX_SPEED_LOW && max_speed<=MAX_SPEED_HIGH_LIMIT?max_speed:DEFAULT_MAX_SPEED)
	{
		driver_inside = false;
		speed = 0;
		//MAX_SPEED = max_speed;
		this->accelleration = accelleration >= 5 && accelleration <= 25 ? accelleration : 12;
		
		cout << "Your car is ready" << endl;
	}
	~Car()
	{
		
		cout << "Car is over" << endl;
	}
	void get_in()//залазит внутрь
	{
		
		driver_inside = true;
		//panel();
		control.panel_thread = std::thread(&Car::panel, this);
	}
	void get_out()//вылазит из машины
	{
		if (speed > 0)
		{
			cout << "Сейчас нельзя выйти из машины" << endl;
			return;
		}
		driver_inside = false;
		if(control.panel_thread.joinable())control.panel_thread.join();
		system("CLS");//очищает экран
		cout << "You are out of your car" << endl;
	}
	void start_engine()//запуск двигателя
	{
		if (driver_inside && tank.get_fuel_level())
		{
			engine.start();
			control.engine_idle_thread = std::thread(&Car::engine_idle, this);
		}
	}
	
	void stop_engine()//глушит двигатель
	{
		engine.stop();
		if (control.engine_idle_thread.joinable())control.engine_idle_thread.join();
	}
	
	void control_car()//управление машиной
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
			  
		  case 'I'://зажигание
		  case'i':
			  if (engine.started())stop_engine();
			  else
			  {
				  start_engine();
			  }
			  break;
		  case 'f'://заправка
		  case'F':
			  
			  if (driver_inside)
			  {
				  cout << "Для того, чтобы заправиться, нужно выйти из машины" << endl;
				  
			  }
			  else
			  {
				  double amount;
				  cout << "Введите литраж: "; cin >> amount;
				  tank.fill(amount);
				  cout << "Заправка успешно завершена! " << endl;
			  }
			  break;
		  case 'w'://газ
		  case'W':
			  if (driver_inside && engine.started() )
			  {
				 if (speed < MAX_SPEED)speed += accelleration;
				 if(speed>MAX_SPEED) speed =MAX_SPEED;
				 if (!control.free_wheeling_thread.joinable())control.free_wheeling_thread = std::thread(&Car::free_wheeling, this);
				  std::this_thread::sleep_for(1s);
			  }
			  
			  break;
		  case's'://тормоз
		  case 'S':
			  if (driver_inside && speed > 0)
			  {
				  speed -= accelleration;
				  if (speed < 0)speed = 0;
				  std::this_thread::sleep_for(1s);
			  }
			  break;
		  case Escape://выход
			  speed = 0;
			  stop_engine();
			  get_out();
			  break;
		  }
		  if (speed == 0 && control.free_wheeling_thread.joinable())//проверяет скорость и проверяет поток можно ли синхронизировать(остановить)
			  control.free_wheeling_thread.join();
		} while (key != Escape);

	}
	void panel()const//отображает панель приборов
	{
		while (driver_inside)
		{
			system("CLS");
			for (int i = 0; i < speed/3; i++)cout << "|";
			cout << endl;
			cout << "Speed:\t" << speed << " km/h\n";
			cout << "Fuel level: " << tank.get_fuel_level() << " liters\t";
			if (tank.get_fuel_level() < 5)
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//позволяет идентификатор объекта позволяет обратиться к любому объекту виндоус
				SetConsoleTextAttribute(hConsole, 0xCF);//позволяет поменять цвет и фон текста
				cout << "LOW FUEL";
				SetConsoleTextAttribute(hConsole, 0x07);
			}
			cout << endl;
			cout << "Engine is " << (engine.started() ? "started" : "stopped") << endl;
			if(engine.started())cout << "Consumption per second: " << engine.get_consumtion_per_second() << endl;
			std::this_thread::sleep_for(1s);
		}
	}
	void engine_idle()//сжигает бензин
	{
		while (engine.started()&&tank.give_fuel(engine.get_consumtion_per_second()))
		{
			std::this_thread::sleep_for(1s);
		}
	}
	void free_wheeling()//реализует силу трения
	{
		while (speed > 0)
		{
			speed--;
			if (speed < 0)speed = 0;
			std::this_thread::sleep_for(1s);
			engine.correct_consumtion(speed);

		}
	}
	
	void info()const//информация бак и движок
	{
		engine.info();
		tank.info();
	}
};
//#define TANK_CHECK
//#define ENGINE_CHECK
void main()
{
	//throw std::exception();
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
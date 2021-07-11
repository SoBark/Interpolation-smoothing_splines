#include "split.h"

namespace Com_Methods 
{

	void RegularSplit(std::vector<Com_Methods::Point>& Points, double a, double b, int n)
	{
		//if (a > b) return;
		double h = (b - a) / n; //определение равномерного шага
		//создание равномерного сеточного разбиения
		for (int i = 0; i <= n - 1; i++)
			Points.push_back(Point{ a + h * i,0,0 });
		Points.push_back(Point{ b,0,0 });
	}

	void RegularSplit(std::vector<Com_Methods::Point>& Points, const std::string PATH)
	{
		std::ifstream input(PATH);
		double a, b;
		int n;
		if (!input.is_open())
			throw std::exception("Input file has not found...");
		input >> a >> b >> n;
		double h = (b - a) / n; //определение равномерного шага
		//создание равномерного сеточного разбиения
		for (int i = 0; i < n; i++)
			Points.push_back(Point{ a + h * i,0,0 });
		Points.push_back(Point{ b,0,0 });
	}

	void AdaptiveSplit(std::vector<Com_Methods::Point>& Points, double a, double b, int n, double r)
	{
		//поиск начального шага
		// h1 = h, h2 = rh1 = rh, h3 = rh2 = hr^2... hn = hr^(n-1)
		double power = 1, sum = 0;
		for (int i = 0; i < n; i++)
		{
			sum += power;
			power *= r;
		}
		double h = (b - a) / sum;
		//создание адаптивной сетки
		power = 1;
		double curr = a;
		Points.push_back(Point{ curr,0,0 });
		for (int i = 1; i < n; i++)
		{
			curr += h * power;
			Points.push_back(Point{ curr, 0 ,0 });
			power *= r;
		}
		Points.push_back({ b,0,0 });
	}

	void AdaptiveSplit(std::vector<Com_Methods::Point>& Points, const std::string PATH)
	{
		std::ifstream input(PATH);
		double a, b, r;
		int n;
		if (!input.is_open())
			throw std::exception("Input file has not found...");
		input >> a >> b >> n >> r;
		//поиск начального шага
		// h1 = h, h2 = rh1 = rh, h3 = rh2 = hr^2... hn = hr^(n-1)
		double power = 1, sum = 0;
		for (int i = 0; i < n; i++)
		{
			sum += power;
			power *= r;
		}
		double h = (b - a) / sum;
		//создание адаптивной сетки
		power = 1;
		double curr = a;
		Points.push_back(Point{ curr,0,0 });
		for (int i = 1; i < n; i++)
		{
			curr += h * power;
			Points.push_back(Point{ curr, 0 ,0 });
			power *= r;
		}
		Points.push_back({ b,0,0 });
	}
}
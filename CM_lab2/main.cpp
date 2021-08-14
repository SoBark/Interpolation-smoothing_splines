#include "split.h"
#include "Cubic_Interpolation_Spline_1D.h"
#include "Smoothing_Spline_1D.h"
#include "Spline.h"
#include <iostream>
#include <functional>

//подпрограммы вывода значений сплайна на заданном промежутке [a,b] с шагом step
void arbitrary_points(Com_Methods::Spline& Spline, double a, double b, int n, double h, double step, const std::string PATH);

void arbitrary_points_adpt(Com_Methods::Spline& Spline, double a, double b, double r, int n, double step, const std::string PATH);


int main()
{
	//»сследование кубического интерпол€ционного сплайна
	Com_Methods::Cubic_Interpolation_Spline_1D cubic_spline;
	double a = 0, b = 1;
	int n = 10;			//h1 = (b-a)/n = 0.1
	std::vector<double> F_value;
	std::string name_files_x = "x1_h1_output.txt", path = "results/";
	std::vector<Com_Methods::Point> Mesh;
	//x,x^2,x^3,x^4 на отрезке [0,1] с шагами h1=0.1, h2=h1/2, h3=h1/4
	for (int k = 0; k < 3; k++) {
		Com_Methods::RegularSplit(Mesh, a, b, n);
		name_files_x.replace(4, 1, std::to_string(k + 1));
		for (int j = 1; j <= 4; j++)
		{
			for (int i = 0; i <= n; i++)
				F_value.push_back(std::pow(Mesh[i].x(), j));
			cubic_spline.Update_Spline(Mesh, F_value);
			name_files_x.replace(1, 1, std::to_string(j));
			arbitrary_points(cubic_spline, a, b, n, (b-a)/n, 0.01, path+name_files_x);
			F_value.clear();
		}
		Mesh.clear();
		n *= 2;
	}
	//sin(x) на отрезке [0, 7] c шагами h1=0.5, h2=h1/2, h3=h1/4
	a = 0; b = 7; n = 14;
	std::string name_files_sin = "sinx_h1_output.txt";
	for (int k = 0; k < 3; k++) {
		Com_Methods::RegularSplit(Mesh, a, b, n);
		name_files_sin.replace(6, 1, std::to_string(k + 1));
		for (int i = 0; i <= n; i++)
			F_value.push_back(std::sin(Mesh[i].x()));
		cubic_spline.Update_Spline(Mesh, F_value);
		arbitrary_points(cubic_spline, a, b, n, (b-a)/n, 0.01, path + name_files_sin);
		F_value.clear();
		Mesh.clear();
		n *= 2;
	}
	//иследование второй производной на сгущенной сетке к концам отрезка
	//дл€ функций x,x2,x3,x4
	a = 0; b = 1; n = 20;
	double r = 3.5;

	Com_Methods::AdaptiveSplit(Mesh, a, (a+b) / 2, n/2, r);
	Mesh.pop_back();
	Com_Methods::AdaptiveSplit(Mesh, (a+b) / 2, b, n / 2, 1 / r);
	name_files_x = "x1_adpt_output.txt";
	for (int j = 1; j <= 4; j++)
	{
		for (int i = 0; i <= n; i++)
			F_value.push_back(std::pow(Mesh[i].x(), j));
		cubic_spline.Update_Spline(Mesh, F_value);
		name_files_x.replace(1, 1, std::to_string(j));
		arbitrary_points_adpt(cubic_spline, Mesh[n-5].x(), b, r, n, 1e-6, path + name_files_x);
		F_value.clear();
	}
	Mesh.clear();
	//дл€ sinx
	a = 0; b = 1; n = 20; r = 3.5;
	name_files_sin = "sinx_adpt_output.txt";
	Com_Methods::AdaptiveSplit(Mesh, a, b / 2, n / 2, r);
	Mesh.pop_back();
	Com_Methods::AdaptiveSplit(Mesh, b / 2, b, n / 2, 1 / r);
	for (int i = 0; i <= n; i++)
		F_value.push_back(std::sin(Mesh[i].x()));
	cubic_spline.Update_Spline(Mesh, F_value);
	arbitrary_points_adpt(cubic_spline, Mesh[n-5].x(), b, r, n, 1e-6, path + name_files_sin);
	F_value.clear();
	Mesh.clear();


	std::function<int(int)> fac = [&](int n){ return (n < 2) ? 1 : n*fac(n-1); };
	//»сследование сглаживающего сплайна
	//вспомогательна€ функци€ дл€ построени€ сплайна нужной гладкости
	std::function<void(std::vector<Com_Methods::Point>& Mesh, std::vector<double>& F_Value, double smooth, const std::string PATH)>
		research_smooth_spline = [&](std::vector<Com_Methods::Point>& Mesh, std::vector<double>& F_Value, double smooth,const std::string PATH)
	{
		Com_Methods::Smoothing_Spline_1D smooth_spline(smooth);
		smooth_spline.Update_Spline(Mesh, F_Value);
		arbitrary_points(smooth_spline, a, b, n, (b - a) / n, 0.001, PATH);
	};
	//—глаживающие сплайны функции x|sin(10000x)|
	//с коэффициентам гладкости smooth = 0.1, 0.5, 0.99
	//с шагами сетки h=0.05, 0,025
	//веса w = 1, 0.5
	a = -1; b = 1; n = 40;
	double sm[] = { 0, 0.1, 0.5, 0.99 };
	for (int k = 0; k < 2; k++) {
		Com_Methods::RegularSplit(Mesh, a, b, n);
		for (int i = 0; i <= n; i++)
			F_value.push_back(Mesh[i].x()*std::abs(std::sin(10000*Mesh[i].x())));
		for (int j = 0; j < 4; j++)
		{
			research_smooth_spline(Mesh,F_value,sm[j], path + "osc_w2h" + std::to_string(k + 1) + "sm" + std::to_string(j) + ".txt");
		}
		F_value.clear();
		Mesh.clear();
		n *= 2;
	}
	return 0;



}

void arbitrary_points(Com_Methods::Spline& Spline, double a, double b, int n, double h, double step, const std::string PATH)
{
	double x = a, res[3]/*, eps=Com_Methods::__EPS__*/;
	std::ofstream Writer(PATH);
	Writer.precision(6);
	Writer.setf(std::ios::fixed | std::ios::showpos);
	Writer << "a=\t" << a << "\tb=\t" << b << "\tn=\t" << n << "\th=\t" << h << std::endl;
	Writer << "x\tg(x)\tg'(x)\tg''(x)" << std::endl;
	while (std::abs(x - b) > 1e-6)
	{
		Spline.Get_Value(Com_Methods::Point{ x,0,0 }, res);
		Writer << x << "\t" << res[0] << "\t" << res[1] << "\t" << res[2] << std::endl;
		x += step;
	}
	Spline.Get_Value(Com_Methods::Point{ b,0,0 }, res);
	Writer << b << "\t" << res[0] << "\t" << res[1] << "\t" << res[2] << std::endl;
	Writer.close();
}

void arbitrary_points_adpt(Com_Methods::Spline& Spline, double a, double b, double r, int n, double step, const std::string PATH)
{
	double x = a, res[3]/*, eps = Com_Methods::__EPS__*/;
	std::ofstream Writer(PATH);
	Writer.precision(6);
	Writer.setf(std::ios::fixed | std::ios::showpos);
	Writer << "a=\t" << a << "\tb=\t" << b << "\tn=\t" << n << "\tr=\t" << r << std::endl;
	Writer << "x\tg(x)\tg'(x)\tg''(x)" << std::endl;
	while (std::abs(x - b) > 1e-6)
	{
		Spline.Get_Value(Com_Methods::Point{ x,0,0 }, res);
		Writer << x << "\t" << res[0] << "\t" << res[1] << "\t" << res[2] << std::endl;
		x += step;
	}
	Spline.Get_Value(Com_Methods::Point{ b,0,0 }, res);
	Writer << b << "\t" << res[0] << "\t" << res[1] << "\t" << res[2] << std::endl;
	Writer.close();
}

#include "split.h"
#include "Cubic_Interpolation_Spline_1D.h"
#include <iostream>
void arbitrary_points(Com_Methods::Cubic_Interpolation_Spline_1D& Spline, double a, double b, int n, double h, double step, const std::string PATH);


int main()
{
	Com_Methods::Cubic_Interpolation_Spline_1D spline;
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
			spline.Update_Spline(Mesh, F_value);
			name_files_x.replace(1, 1, std::to_string(j));
			arbitrary_points(spline, a, b, n, (b-a)/n, 0.01, path+name_files_x);
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
		spline.Update_Spline(Mesh, F_value);
		arbitrary_points(spline, a, b, n, (b-a)/n, 0.1, path + name_files_sin);
		F_value.clear();
		Mesh.clear();
		n *= 2;
	}
	//иследование второй производной на сгущенной сетке к концам отрезка
	//для функций x,x2,x3,x4
	a = 0; b = 1; n = 20;
	double r = 10;

	Com_Methods::AdaptiveSplit(Mesh, a, b / 2, n/2, r);
	Mesh.pop_back();
	Com_Methods::AdaptiveSplit(Mesh, b / 2, b, n / 2, 1 / r);
	name_files_x = "x1_adpt_output.txt";
	for (int j = 1; j <= 4; j++)
	{
		for (int i = 0; i <= n; i++)
			F_value.push_back(std::pow(Mesh[i].x(), j));
		spline.Update_Spline(Mesh, F_value);
		name_files_x.replace(1, 1, std::to_string(j));
		arbitrary_points(spline, a, b, n, Mesh[1].x()-Mesh[0].x(), 0.01, path + name_files_x);
		F_value.clear();
	}
	Mesh.clear();
	//для sinx
	a = 0; b = 7; n = 28; r = 10;
	name_files_sin = "sinx_adpt_output.txt";
	Com_Methods::AdaptiveSplit(Mesh, a, b / 2, n / 2, r);
	Mesh.pop_back();
	Com_Methods::AdaptiveSplit(Mesh, b / 2, b, n / 2, 1 / r);
	for (int i = 0; i <= n; i++)
		F_value.push_back(std::sin(Mesh[i].x()));
	spline.Update_Spline(Mesh, F_value);
	arbitrary_points(spline, a, b, n, Mesh[1].x() - Mesh[0].x(), 0.1, path + name_files_sin);
	F_value.clear();
	Mesh.clear();

	return 0;

}

void arbitrary_points(Com_Methods::Cubic_Interpolation_Spline_1D& Spline, double a, double b, int n, double h, double step, const std::string PATH)
{
	double x = a, res[3], eps=1e-7;
	std::ofstream Writer(PATH);
	Writer.precision(6);
	Writer.setf(std::ios::fixed | std::ios::showpos);
	Writer << "a=" << a << ";b=" << b << ";n=" << n << ";h=" << h << std::endl;
	Writer << "x\tf(x)\tf'(x)\tf''(x)" << std::endl;
	while (x <= b)
	{
		Spline.Get_Value(Com_Methods::Point{ x,0,0 }, res);
		Writer << x << "\t" << res[0] << "\t" << res[1] << "\t" << res[2] << std::endl;
		x += step;
	}
	Writer.close();
}

#define _USE_MATH_DEFINES
#include <cmath>

#include "OptimizerNM.h"
#include "ObjectiveA4.h"
#include <iostream>

using namespace std;
using namespace Eigen;

OptimizerNM::OptimizerNM() :
	tol(1e-3),
	iterMax(100),
	iter(0)
{
	
}

OptimizerNM::~OptimizerNM()
{
	
}

VectorXd OptimizerNM::optimize(const shared_ptr<ObjectiveA4> objective, const VectorXd& thetaInit, const VectorXd& pTar, bool finiteDiff)
{
	int n = thetaInit.rows();
	VectorXd theta = thetaInit;
	VectorXd g(n);
	MatrixXd H(n, n);

	for (iter = 1; iter <= iterMax; iter++)
	{
		double f = objective->evalObjective(theta, pTar, g, H);

		if (finiteDiff)
		{
			double e = 1e-7;
			MatrixXd H_(n, n);
			for (int i = 0; i < n; ++i) {
				VectorXd theta_ = theta;
				theta_(i) += e;
				VectorXd g_(n);
				objective->evalObjective(theta_, pTar, g_);
				H_.col(i) = (g_ - g) / e;
			}
			cout << "H: " << (H_ - H).norm() << endl;
		}

		VectorXd dTheta = -H.ldlt().solve(g);

		theta += dTheta;
		if (dTheta.norm() < tol)
		{
			break;
		}
	}

	for (int i = 0; i < n; i++)
	{
		while (theta(i) > M_PI)
		{
			theta(i) -= (2 * M_PI);
		}
		while (theta(i) < -M_PI)
		{
			theta(i) += (2 * M_PI);
		}
	}
	
	return theta;
}

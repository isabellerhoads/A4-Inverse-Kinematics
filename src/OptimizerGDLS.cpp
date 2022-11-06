#define _USE_MATH_DEFINES
#include <cmath>

#include "OptimizerGDLS.h"
#include "ObjectiveA4.h"
#include <iostream>

using namespace std;
using namespace Eigen;

OptimizerGDLS::OptimizerGDLS() :
	alphaInit(1e-4),
	gamma(0.1),
	tol(1e-3),
	iterMax(100),
	iter(0)
{
	
}

OptimizerGDLS::~OptimizerGDLS()
{
	
}

VectorXd OptimizerGDLS::optimize(const shared_ptr<ObjectiveA4> objective, const VectorXd& thetaInit, const VectorXd& pTar, bool finiteDiff)
{
	VectorXd theta = thetaInit;
	int n = thetaInit.rows();
	VectorXd g(n);

	for (iter = 1; iter < iterMax; iter++)
	{
		double f = objective->evalObjective(theta, pTar, g);
		double alpha = alphaInit;
		VectorXd dTheta(n);

		if (finiteDiff)
		{
			double e = 1e-7;
			VectorXd g_(n);
			for (int i = 0; i < n; ++i)
			{
				VectorXd theta_ = theta;
				theta_(i) += e;
				double f_ = objective->evalObjective(theta_, pTar);
				g_(i) = (f_ - f) / e; 
			}

			cout << "g: " << (g_ - g).norm() << endl;
		}

		for (int iterLS = 1; iterLS < iterMax; iterLS++)
		{
			dTheta = -alpha * g;
			double fNew = objective->evalObjective(theta + dTheta, pTar);

			if (fNew < f)
			{
				break;
			}
			alpha *= gamma;
		}

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

#include "ObjectiveA4.h"
#include <cmath>
#include <memory>
#include <vector>

#include "Link.h"
#include <iostream>

using namespace Eigen;
using namespace std;

ObjectiveA4::ObjectiveA4()
{

}

ObjectiveA4::~ObjectiveA4()
{
	
}

Vector3d ObjectiveA4::calculateP()
{
	Matrix3d calculatedPs;
	calculatedPs.setIdentity();

	Vector3d r;
	r << 1.0, 0.0, 1.0;

	for (int i = 0; i < pVals.size(); i++)
	{
		calculatedPs *= pVals[i];
	}

	return calculatedPs * r;
}

Matrix3d ObjectiveA4::calculatePTransforms(double theta, bool headLink)
{
	MatrixXd R(3, 3);
	R << cos(theta), -sin(theta), 0,
		sin(theta), cos(theta), 0,
		0, 0, 1;

	Matrix3d T;
	T.setIdentity();
	if (!headLink)
	{
		T << 1.0, 0, 1.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;
	}
	return T * R;
}

Matrix3d ObjectiveA4::calculatePPrime(double theta, bool headLink)
{
	MatrixXd R1(3, 3);
	R1 << -sin(theta), -cos(theta), 0,
		cos(theta), -sin(theta), 0,
		0, 0, 0;

	Matrix3d T;
	T.setIdentity();
	if (!headLink)
	{
		T << 1.0, 0, 1.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;
	}
	return T * R1;
}

Matrix3d ObjectiveA4::calculatePDoublePrime(double theta, bool headLink)
{
	MatrixXd R2(3, 3);
	R2 << -cos(theta), sin(theta), 0,
		-sin(theta), -cos(theta), 0,
		0, 0, 0;

	Matrix3d T;
	T.setIdentity();
	if (!headLink)
	{
		T << 1.0, 0, 1.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0;
	}
	return T * R2;
}

void ObjectiveA4::calculateTransformations(VectorXd theta)
{
	pVals.clear();
	pPrimes.clear();
	pDoublePrimes.clear();

	bool headLink = true;
	for (int i = 0; i < theta.size(); i++)
	{
		if (i != 0) 
		{
			headLink = false;
		}
		pVals.push_back(this->calculatePTransforms(theta(i), headLink));
		pPrimes.push_back(this->calculatePPrime(theta(i), headLink));
		pDoublePrimes.push_back(this->calculatePDoublePrime(theta(i), headLink));
	}

	VectorXd rest(theta.size());
	rest.setZero();
	this->thetaRest = rest;
}

std::vector<Vector3d> ObjectiveA4::calculatePPrimes(int n)
{
	std::vector<Vector3d> primes;
	VectorXd r(3);
	r << 1.0, 0.0, 1.0;

	for (int i = 0; i < n; i++)
	{
		Matrix3d PPrime_ij;
		PPrime_ij.setIdentity();
		for (int j = 0; j < n; j++)
		{
			if (i == j)
			{
				PPrime_ij *= pPrimes[j];
			}
			else
			{
				PPrime_ij *= pVals[j];
			}
		}
		primes.push_back(PPrime_ij * r);
	}

	return primes;
}

std::vector<std::vector<Vector3d>> ObjectiveA4::calculatePDoublePrimes(int n)
{
	std::vector<std::vector<Vector3d>> doublePrimes(n);
	VectorXd r(3);
	r << 1.0, 0.0, 1.0;

	for (int i = 0; i < n; i++)
	{
		doublePrimes[i] = std::vector<Vector3d>(n);
		for (int j = 0; j < n; j++)
		{
			Matrix3d PDoublePrime_ij;
			PDoublePrime_ij.setIdentity();
			for (int k = 0; k < n; k++)
			{
				if (j == i && k == j)
				{
					PDoublePrime_ij *= pDoublePrimes[k];
				}
				else if (k == i || k == j)
				{
					PDoublePrime_ij *= pPrimes[k];
				}
				else {
					PDoublePrime_ij *= pVals[k];
				}
			}
			doublePrimes[i][j] = PDoublePrime_ij * r;
		}
	}
	return doublePrimes;
}


double ObjectiveA4::evalObjective(const VectorXd& theta, const VectorXd &pTar)
{
	this->calculateTransformations(theta);

	Vector3d p = this->calculateP();
	Vector3d deltaP = p - pTar;
	VectorXd deltaTheta = theta - thetaRest;

	return (0.5 * wTar * deltaP.dot(deltaP)) + (0.5 * wReg * deltaTheta.dot(deltaTheta));
}

double ObjectiveA4::evalObjective(const VectorXd& theta, const VectorXd& pTar, VectorXd &g)
{
	this->calculateTransformations(theta);

	Vector3d p = this->calculateP();
	int nLinks = theta.size();

	Vector3d deltaP = p - pTar;
	VectorXd deltaTheta = theta - thetaRest;

	std::vector<Vector3d> primes = this->calculatePPrimes(nLinks);

	for (int i = 0; i < nLinks; i++)
	{
		g(i) = (wTar * deltaP.transpose() * primes[i]) + (wReg * deltaTheta(i));
	}

	return (0.5 * wTar * deltaP.dot(deltaP)) + (0.5 * wReg * deltaTheta.dot(deltaTheta));
} 

double ObjectiveA4::evalObjective(const VectorXd& theta, const VectorXd& pTar, VectorXd &g, MatrixXd &H) 
{
	this->calculateTransformations(theta);

	Vector3d p = this->calculateP();        
	int nLinks = pVals.size();

	Vector3d deltaP = p - pTar;
	VectorXd deltaTheta = theta - thetaRest;

	std::vector<Vector3d> primes = this->calculatePPrimes(nLinks);
	std::vector<std::vector<Vector3d>> doublePrimes = this->calculatePDoublePrimes(nLinks);

	for (int i = 0; i < nLinks; i++)
	{
		g(i) = (wTar * deltaP.transpose() * primes[i]) + (wReg * deltaTheta(i));
	}

	MatrixXd I(nLinks, nLinks);
	I.setIdentity();
	for (int i = 0; i < nLinks; i++)
	{
		for (int j = 0; j < nLinks; j++)
		{
			H(i, j) = wTar * (primes[i].dot(primes[j]) + deltaP.dot(doublePrimes[i][j])) + wReg * I(i, j);
		}
	}

	return (0.5 * wTar * deltaP.dot(deltaP)) + (0.5 * wReg * deltaTheta.dot(deltaTheta));
}

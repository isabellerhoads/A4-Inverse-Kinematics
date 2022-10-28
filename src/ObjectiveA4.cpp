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

	for (auto pIt : this->transformations)
	{
		calculatedPs *= pIt;
	}

	return calculatedPs * r;
}

void ObjectiveA4::calculateTransformations(std::shared_ptr<Link> headLink)
{
	transformations.clear();
	pPrimes.clear();
	pDoublePrimes.clear();

	int count = 0;
	auto link = headLink;
	while (link) 
	{
		count++;
		transformations.push_back(link->calculateP());
		pPrimes.push_back(link->calculatePPrime());
		pDoublePrimes.push_back(link->calculatePDoublePrime());
		link = link->getChild();
		
	}

	VectorXd rest(count);
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
				PPrime_ij *= transformations[j];
			}
		}
		primes.push_back(PPrime_ij * r);
	}

	return primes;
}

std::vector<Vector3d> ObjectiveA4::calculatePDoublePrimes(int n)
{
	std::vector<Vector3d> doublePrimes;
	VectorXd r(3);
	r << 1.0, 0.0, 1.0;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			Matrix3d PDoublePrime_ij;
			PDoublePrime_ij.setIdentity();
			for (int k = 0; k < n; k++)
			{
				if (k == i && k == j)
				{
					PDoublePrime_ij *= pDoublePrimes[k];
				}
				else if (k == i || k == j)
				{
					PDoublePrime_ij *= pPrimes[k];
				}
				else {
					PDoublePrime_ij *= transformations[k];
				}
			}
			doublePrimes.push_back(PDoublePrime_ij * r);
		}
	}

	return doublePrimes;
}


double ObjectiveA4::evalObjective(const VectorXd& theta, const VectorXd &pTar)
{
	Vector3d p = this->calculateP();

	Vector3d deltaP = p - pTar;
	VectorXd deltaTheta = theta - thetaRest;

	return (0.5 * wTar * deltaP.dot(deltaP)) + (0.5 * wReg * deltaTheta.dot(deltaTheta));
}

double ObjectiveA4::evalObjective(const VectorXd& theta, const VectorXd& pTar, VectorXd &g)
{
	Vector3d p = this->calculateP();
	int nLinks = transformations.size();
	cout << "p: " << p << endl;

	Vector3d deltaP = p - pTar;
	VectorXd deltaTheta = theta - thetaRest;

	std::vector<Vector3d> primes = this->calculatePPrimes(nLinks);
	std::vector<Vector3d> doubles = this->calculatePDoublePrimes(nLinks);

	for (int i = 0; i < nLinks * nLinks; i++)
	{
		std::cout << "double p: " << doubles[i] << endl;;
	}

	for (int i = 0; i < nLinks; i++)
	{
		g(i) = (wTar * deltaP.transpose() * primes[i]) + (wReg * deltaTheta(i));
		std::cout << "g - " << g(i) << std::endl;
		std::cout << "primes - " << primes[i] << std::endl;
	}

	return (0.5 * wTar * deltaP.dot(deltaP)) + (0.5 * wReg * deltaTheta.dot(deltaTheta));
}

double ObjectiveA4::evalObjective(const VectorXd& theta, const VectorXd& pTar, VectorXd &g, MatrixXd &H) 
{
	Vector3d p = this->calculateP();                  
	int nLinks = transformations.size();

	Vector3d deltaP = p - pTar;
	VectorXd deltaTheta = theta - thetaRest;

	std::vector<Vector3d> primes = this->calculatePPrimes(nLinks);
	std::vector<Vector3d> doublePrimes = this->calculatePDoublePrimes(nLinks);

	for (int i = 0; i < nLinks; i++)
	{
		g(i) = (wTar * deltaP.transpose() * primes[i]) + (wReg * deltaTheta(i));
	}

	return (0.5 * wTar * deltaP.dot(deltaP)) + (0.5 * wReg * deltaTheta.dot(deltaTheta));
}

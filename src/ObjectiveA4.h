#pragma once
#ifndef OBJECTIVEA4_H
#define OBJECTIVEA4_H

#include <Eigen/Dense>
#include <memory>
#include <vector>

#include "Link.h"
#include "Objective.h"

class ObjectiveA4 : public Objective
{
public:
	ObjectiveA4();
	virtual ~ObjectiveA4();
	virtual double evalObjective(const Eigen::VectorXd& theta, const Eigen::VectorXd& pTar);
	virtual double evalObjective(const Eigen::VectorXd& theta, const Eigen::VectorXd& pTar, Eigen::VectorXd &g);
	virtual double evalObjective(const Eigen::VectorXd& theta, const Eigen::VectorXd& pTar, Eigen::VectorXd &g, Eigen::MatrixXd &H);

	void calculateTransformations(Eigen::VectorXd theta);
	Eigen::Vector3d calculateP();
	std::vector<Eigen::Vector3d> calculatePPrimes(int n);
	std::vector<std::vector<Eigen::Vector3d>> calculatePDoublePrimes(int n);

	Eigen::Matrix3d calculatePTransforms(double theta, bool headLink);
	Eigen::Matrix3d calculatePPrime(double theta, bool headLink);
	Eigen::Matrix3d calculatePDoublePrime(double theta, bool headLink);
private:
	const double wTar = 1e3;
	const double wReg = 1e0;
	Eigen::VectorXd thetaRest;

	std::vector<Eigen::Matrix3d> pVals;
	std::vector<Eigen::Matrix3d> pPrimes;
	std::vector<Eigen::Matrix3d> pDoublePrimes;
};

#endif

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

	void calculateTransformations(std::shared_ptr<Link> headLink);
	Eigen::Vector3d calculateP();
	std::vector<Eigen::Vector3d> calculatePPrimes(int n);
	std::vector<Eigen::Vector3d> calculatePDoublePrimes(int n);
private:
	const double wTar = 1e3;
	const double wReg = 1e0;
	Eigen::VectorXd thetaRest;

	std::vector<Eigen::Matrix3d> transformations;
	std::vector<Eigen::Matrix3d> pPrimes;
	std::vector<Eigen::Matrix3d> pDoublePrimes;
};

#endif

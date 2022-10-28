#pragma once
#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include <Eigen/Dense>

class Objective
{
public:
	Objective() {};
	virtual ~Objective() {};
	virtual double evalObjective(const Eigen::VectorXd& theta, const Eigen::VectorXd& pTar) = 0;
	virtual double evalObjective(const Eigen::VectorXd& theta, const Eigen::VectorXd& pTar, Eigen::VectorXd& g) = 0;
	virtual double evalObjective(const Eigen::VectorXd& theta, const Eigen::VectorXd& pTar, Eigen::VectorXd& g, Eigen::MatrixXd& H) = 0;
};

#endif

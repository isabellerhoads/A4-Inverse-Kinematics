#pragma once
#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <memory>
#include <Eigen/Dense>

class ObjectiveA4;

class Optimizer
{
public:
	Optimizer() {};
	virtual ~Optimizer() {};
	virtual Eigen::VectorXd optimize(const std::shared_ptr<ObjectiveA4> objective, const Eigen::VectorXd& thetaInit, const Eigen::VectorXd& pTar) = 0;
};

#endif

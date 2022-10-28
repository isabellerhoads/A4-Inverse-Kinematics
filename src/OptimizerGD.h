#pragma once
#ifndef OPTIMIZER_GD_H
#define OPTIMIZER_GD_H

#include "Optimizer.h"

class Objective;

class OptimizerGD : public Optimizer
{
public:
	OptimizerGD();
	virtual ~OptimizerGD();
	virtual Eigen::VectorXd optimize(const std::shared_ptr<Objective> objective, const Eigen::VectorXd &xInit);
	
	void setAlpha(double alpha) { this->alpha = alpha; }
	void setTol(double tol) { this->tol = tol; }
	void setIterMax(int iterMax) { this->iterMax = iterMax; }
	int getIter() const { return iter; }
	
private:
	double alpha;
	double tol;
	int iterMax;
	int iter;
};

#endif

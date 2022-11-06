#pragma once
#ifndef OPTIMIZER_NM_H
#define OPTIMIZER_NM_H

#include "Optimizer.h"

class Objective;

class OptimizerNM : public Optimizer
{
public:
	OptimizerNM();
	virtual ~OptimizerNM();
	virtual Eigen::VectorXd optimize(const std::shared_ptr<ObjectiveA4> objective, const Eigen::VectorXd& thetaInit, const Eigen::VectorXd& pTar, bool finiteDiff);
	
	void setTol(double tol) { this->tol = tol; }
	void setIterMax(int iterMax) { this->iterMax = iterMax; }
	int getIter() const { return iter; }
	
private:
	double tol;
	int iterMax; 
	int iter;
};

#endif

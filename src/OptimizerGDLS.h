#pragma once
#ifndef OPTIMIZER_GDLS_H
#define OPTIMIZER_GDLS_H

#include "Optimizer.h"

class ObjectiveA4;

class OptimizerGDLS : public Optimizer
{
public:
	OptimizerGDLS();
	virtual ~OptimizerGDLS();
	virtual Eigen::VectorXd optimize(const std::shared_ptr<ObjectiveA4> objective, const Eigen::VectorXd& thetaInit, const Eigen::VectorXd& pTar);
	
	void setAlphaInit(double alphaInit) { this->alphaInit = alphaInit; }
	void setGamma(double gamma) { this->gamma = gamma; }
	void setTol(double tol) { this->tol = tol; }
	void setIterMax(int iterMax) { this->iterMax = iterMax; }
	int getIter() const { return iter; }
	
private:
	double alphaInit;
	double gamma;
	double tol;
	int iterMax;
	int iter;
};

#endif

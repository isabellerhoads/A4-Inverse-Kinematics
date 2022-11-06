#define _USE_MATH_DEFINES
#include <cmath>

#include <assert.h>
#include <iostream>
#include <memory>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>

#include "Link.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Optimizer.h"
#include "OptimizerGDLS.h"
#include "OptimizerNM.h"
#include "ObjectiveA4.h"

using namespace std;
using namespace Eigen;

Link::Link() :
	parent(NULL),
	child(NULL),
	depth(0),
	position(0.0, 0.0),
	angle(0.0),
	meshMat(Matrix4d::Identity())
{
	
}

Link::~Link()
{
	
}

void Link::addChild(shared_ptr<Link> child)
{
	child->parent = shared_from_this();
	child->depth = depth + 1;
	this->child = child;
}

Eigen::VectorXd Link::updateAngles(Eigen::VectorXd thetaInit, const Eigen::Vector3d pTar, std::shared_ptr<ObjectiveA4> objective, std::shared_ptr<OptimizerGDLS> optimizerGDLS, std::shared_ptr<OptimizerNM> optimizerNM, bool finiteDiff)
{
	int n = thetaInit.rows();
	VectorXd theta = thetaInit;
	VectorXd g(n);
	MatrixXd H(n, n);

	double f = objective->evalObjective(thetaInit, pTar, g, H);

	VectorXd thetaGDLS = optimizerGDLS->optimize(objective, thetaInit, pTar, finiteDiff);
	cout << "Number of iterations for GDLS: " << optimizerGDLS->getIter() << endl;

	VectorXd thetaNM = optimizerNM->optimize(objective, thetaGDLS, pTar, finiteDiff);
	cout << "Number of iterations for NM: " << optimizerNM->getIter() << endl;

	double fNew = objective->evalObjective(thetaNM, pTar);

	if (fNew < f)
	{
		return thetaGDLS;
	}

	return thetaGDLS;
}

void Link::draw(const shared_ptr<Program> prog, shared_ptr<MatrixStack> MV, const shared_ptr<Shape> shape) const
{
	assert(prog);
	assert(MV);
	assert(shape);
	
	if (this == NULL) { return; }

	Vector3d axis(0.0, 0.0, 1.0);
	axis.normalize();
	Matrix3d R(AngleAxis(this->angle, axis));
	Matrix4d jointMat;
	jointMat.setIdentity();
	jointMat.block<3, 3>(0.0, 0.0) = R;
	jointMat.col(3) = Vector4d(this->position[0], this->position[1], 0.0, 1.0);

	Matrix4d transMat;
	transMat.setIdentity();
	transMat.col(3) = Vector4d(0.5, 0.0, 0.0, 1.0);

	MV->pushMatrix();
		MV->multMatrix(jointMat);
		MV->pushMatrix(); 
			MV->multMatrix(meshMat * transMat);
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			shape->setProgram(prog);
			shape->draw();
		MV->popMatrix();
		this->child->draw(prog, MV, shape);
	MV->popMatrix();
}
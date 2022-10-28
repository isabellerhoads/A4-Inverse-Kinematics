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

MatrixXd Link::calculateP() 
{

	MatrixXd R(3, 3);
	R << cos(angle), -sin(angle), 0,
		sin(angle), cos(angle), 0,
		0, 0, 1;

	Matrix3d T;
	T.setIdentity();
	T.col(2) = Vector3d(this->position[0], this->position[1], 1.0);

	return T * R;
}

MatrixXd Link::calculatePPrime() 
{
	MatrixXd R1(3, 3);
	R1 << -sin(angle), -cos(angle), 0,
		cos(angle), sin(angle), 0,
		0, 0, 0;

	Matrix3d T;
	T.setIdentity();
	T.col(2) = Vector3d(this->position[0], this->position[1], 1.0);

	return T * R1;
}

MatrixXd Link::calculatePDoublePrime() 
{
	MatrixXd R2(3, 3);
	R2 << -cos(angle), sin(angle), 0,
		-sin(angle), -cos(angle), 0,
		0, 0, 0;

	Matrix3d T;
	T.setIdentity();
	T.col(2) = Vector3d(this->position[0], this->position[1], 1.0);

	return T * R2;
}

void Link::calculateTransformations(const VectorXd& pTar)
{

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
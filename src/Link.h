#pragma once
#ifndef Link_H
#define Link_H

#include <memory>

#include <Eigen/Dense>

class ObjectiveA4;
class OptimizerGDLS;
class OptimizerNM;
class Shape;
class MatrixStack;
class Program;

class Link : public std::enable_shared_from_this<Link>
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	
	Link();
	virtual ~Link();

	void addChild(std::shared_ptr<Link> child);
	std::shared_ptr<Link> getChild() const { return child; }
	std::shared_ptr<Link> getParent() const { return parent; }
	int getDepth() const { return depth; }

	void setPosition(double x, double y) { position << x, y; }
	const Eigen::Vector2d &getPosition() const { return position; }

	void setAngle(double a) { angle = a; }
	double getAngle() const { return angle; }

	void setMeshMatrix(const Eigen::Matrix4d &M) { meshMat = M; }

	static Eigen::VectorXd updateAngles(Eigen::VectorXd thetaInit, const Eigen::Vector3d pTar, std::shared_ptr<ObjectiveA4> objective, std::shared_ptr<OptimizerGDLS> optimizerGDLS, std::shared_ptr<OptimizerNM> optimizerNM, bool finiteDiff);
	
	void draw(const std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack> MV, const std::shared_ptr<Shape> shape) const;
	
private:
	std::shared_ptr<Link> parent;
	std::shared_ptr<Link> child;
	int depth;
	Eigen::Vector2d position;
	double angle;
	Eigen::Matrix4d meshMat;
};

#endif

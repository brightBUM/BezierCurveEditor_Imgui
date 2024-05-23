#pragma once
#include"Point2f.h"
#include<vector>
#include<iostream>
class BezierCurve
{
	std::vector<Point3f> CasteljauPoint(std::vector<Point3f> points, float t);
public:
	BezierCurve(int sampleSize);
	int controlPointsCount;
	int curveSampleSize = 10; //more samplePoints - better Curve
	std::vector<Point3f> controlPoints;
	std::vector<Point3f> curvePoints;

	Point3f BezPoint(float);
	void AddControlPoint(Point3f);
	std::vector<float> DrawCurve();
	std::vector<float> DrawCurveBW(float,float);
	std::vector<float> GetControlPointArray();
	std::vector<BezierCurve> DeCasteljauSubDivision(float t);

};


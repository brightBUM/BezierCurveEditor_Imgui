#include "BezierCurve.h"
using namespace std;

BezierCurve::BezierCurve(int sampleSize)
{
	this->curveSampleSize = sampleSize;
	this->controlPointsCount = 3;
}

void BezierCurve::AddControlPoint(Point3f controlPoint)
{
	controlPoints.push_back(controlPoint);

}

std::vector<float> BezierCurve::DrawCurve()
{
	float t = 0.0f;
	std::vector<float> vertices;

	for (int i = 0; i <= curveSampleSize; i++)
	{
		std::vector<Point3f> curvePoint = CasteljauPoint(controlPoints,t);
		vertices.push_back(curvePoint[0].X);
		vertices.push_back(curvePoint[0].Y);
		vertices.push_back(curvePoint[0].Z);
		t += (float)1.0f/(float)curveSampleSize;
	}
	return vertices;
}
std::vector<float> BezierCurve::DrawCurveBW(float from,float to)
{
	float t = from;
	std::vector<float> vertices;

	while (t <= to)
	{
		std::vector<Point3f> curvePoint = CasteljauPoint(controlPoints, t);
		vertices.push_back(curvePoint[0].X);
		vertices.push_back(curvePoint[0].Y);
		vertices.push_back(curvePoint[0].Z);
		t += (float)1.0f / (float)curveSampleSize;
	}
	return vertices;
}

std::vector<float> BezierCurve::GetControlPointArray()
{
	std::vector<float> controlPointArray;

	for (int i = 0; i < controlPoints.size(); i++)
	{
		controlPointArray.push_back(controlPoints[i].X);
		controlPointArray.push_back(controlPoints[i].Y);
		controlPointArray.push_back(controlPoints[i].Z);
	}
	return controlPointArray;
}

Point3f BezierCurve::BezPoint(float t)
{
	return CasteljauPoint(controlPoints, t)[0];
}

// Recurvse algorithm that lerps all the controlpoints till we get a single curvePoint
std::vector<Point3f> BezierCurve::CasteljauPoint(std::vector<Point3f> points,float t)
{
	
	std::vector<Point3f> nextPoints;
	for (int i = 0; i < points.size() - 1; i++)
	{
		Point3f curvePoint = Point3f::LerpPoint(points[i], points[i + 1], t);
		nextPoints.push_back(curvePoint);
	}

	if (nextPoints.size() > 1)
	{
		nextPoints = CasteljauPoint(nextPoints,t);
	}
	return nextPoints;
	
}

std::vector<BezierCurve> BezierCurve::DeCasteljauSubDivision(float t)
{
	BezierCurve subCurve1(10), subCurve2(10);
	std::vector<Point3f> points,nextPoints;
	points = this->controlPoints;
	while (nextPoints.size()!=1)
	{
		subCurve1.AddControlPoint(points[0]);
		subCurve2.controlPoints.insert(subCurve2.controlPoints.begin(), points[points.size()-1]);

		nextPoints.clear();
		for (int i = 0; i < points.size()-1; i++)
		{
			Point3f curvePoint = Point3f::LerpPoint(points[i], points[i + 1], t);
			nextPoints.push_back(curvePoint);
		}
		points = nextPoints;
	}
	subCurve1.AddControlPoint(nextPoints[0]);
	subCurve2.controlPoints.insert(subCurve2.controlPoints.begin(), nextPoints[0]);
	std::vector<BezierCurve> subCurves = { subCurve1,subCurve2 };
	
	
	return subCurves;
}



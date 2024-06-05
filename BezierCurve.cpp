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
	return PointToFloatVector(controlPoints);
}

std::vector<float> BezierCurve::GetBezVisualPointArray()
{
	return PointToFloatVector(bezVisualizePoints);
}

std::vector<float> BezierCurve::GetBezSubPointArray()
{
	return PointToFloatVector(subPoints);
}

void BezierCurve::BezPoint(float t)
{
	bezVisualizePoints.clear();
	subPoints.clear();
	//return CasteljauPoint(controlPoints, t)[0];
	subPoints = controlPoints;
	std::vector<Point3f> points = this->controlPoints;
	std::vector<Point3f> nextPoints;

	while (nextPoints.size() != 1)
	{
		if (nextPoints.size() != 0)
		{
			points = nextPoints;
			nextPoints.clear();
		}
		//cout << "points size - " << points.size() << endl;

		for (int i = 0; i < points.size() - 1; i++)
		{
			bezVisualizePoints.push_back(points[i]);
			bezVisualizePoints.push_back(points[i + 1]);
			Point3f curvePoint = Point3f::LerpPoint(points[i], points[i + 1], t);
			nextPoints.push_back(curvePoint);
			subPoints.push_back(curvePoint);
		}
	}
	//cout << "next point size- " << nextPoints.size() <<endl;
	bezVisualizePoints.push_back(nextPoints[0]);
	/*for (int i = 0; i < bezVisualizePoints.size(); i++)
	{
		cout << "x - " << bezVisualizePoints[i].X << ", y - " << bezVisualizePoints[i].Y << " , z - " << bezVisualizePoints[i].Z << endl;
	}*/
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

std::vector<float> BezierCurve::PointToFloatVector(std::vector<Point3f> points)
{
	std::vector<float> floatPoint;

	for (int i = 0; i < points.size(); i++)
	{
		floatPoint.push_back(points[i].X);
		floatPoint.push_back(points[i].Y);
		floatPoint.push_back(points[i].Z);
	}
	return floatPoint;
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



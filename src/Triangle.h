#pragma once
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_
#include "Vertex.h"
#include <cmath>
#include <algorithm>
#include <math.h>

class Triangle {

private:
	
public:

	Vertex A;
	Vertex B;
	Vertex C;

	double xmin;
	double xmax;
	double ymin;
	double ymax;

	Triangle(Vertex _A, Vertex _B, Vertex _C): A(_A), B(_B), C(_C) {
		xmin = std::min({ _A.x, _B.x, _C.x });
		xmax = std::max({ _A.x, _B.x, _C.x });
		ymin = std::min({ _A.y, _B.y, _C.y });
		ymax = std::max({ _A.y, _B.y, _C.y });
	}
	~Triangle() {};

	void printTriangle() {
		std::cout << "Vertex A: " << A.x << " " << A.y << " " << A.z << std::endl;
		std::cout << "Vertex B: " << B.x << " " << B.y << " " << B.z << std::endl;
		std::cout << "Vertex C: " << C.x << " " << C.y << " " << C.z << std::endl;
		std::cout << "xmin, xmax " << xmin << ", " << xmax << std::endl;
		std::cout << "ymin, ymax " << ymin << ", " << ymax << std::endl;
	}
};
#endif 


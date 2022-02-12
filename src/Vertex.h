#pragma once
#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_


class Vertex {

public:
	double x;
	double y;
	double z;

	int r;
	int g;
	int b;

	double xn;
	double yn;
	double zn;

	Vertex(double _x, double _y, double _z): x(_x), y(_y), z(_z) {}
	~Vertex(){}

	void setRBG(int _r, int _g, int _b) {
		r = _r;
		g = _g;
		b = _b;
	}
	void setXYZ(double _x, double _y, double _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	void setNormal(double _xn, double _yn, double _zn) {
		xn = _xn;
		yn = _yn;
		zn = _zn;
	}
	void printVertex() {
		std::cout << "x,y,z: " << x << ", " << y << "," << z << std::endl;
		std::cout << "xn,yn,zn: " << xn << ", " << yn << "," << zn << std::endl;
		std::cout << "r,g,b: " << r << ", " << g << "," << b << std::endl;
		std::cout << "xn,yn,zn: " << xn << ", " << yn << "," << zn << std::endl;



	}
};

#endif

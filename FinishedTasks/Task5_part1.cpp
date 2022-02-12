/*

Task 5 - First part only with triangles finished
Need to change to accept input arguments before submitting

*/


#include <iostream>
#include <string>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Image.h"
#include "Vertex.h"
#include "Triangle.h"
using namespace std;

struct BC {
	double a;
	double b;
	double c;
};
BC barycentric(double Pi, double Px, Triangle tri);

double RANDOM_COLORS[7][3] = {
	{0.0000,    0.4470,    0.7410},
	{0.8500,    0.3250,    0.0980},
	{0.9290,    0.6940,    0.1250},
	{0.4940,    0.1840,    0.5560},
	{0.4660,    0.6740,    0.1880},
	{0.3010,    0.7450,    0.9330},
	{0.6350,    0.0780,    0.1840},
};

int main() {
	/* int argc, char **argv
	if(argc < 2) {
		cout << "Usage: A1 meshfile" << endl;
		return 0;
	}
	string meshName(argv[1]);
	*/

	// Hard coded while testing
	string meshName = "../resources/tri2.obj";
	string outfile = "tri2_task5_456.png";
	int height = 512;
	int width = 512;
	int task = 5;

	// Create the image
	auto image = make_shared<Image>(width, height);

	// Load geometry
	vector<float> posBuf; // list of vertex positions
	vector<float> norBuf; // list of vertex normals
	vector<float> texBuf; // list of vertex texture coords
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if (!rc) {
		cerr << errStr << endl;
	}
	else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
					posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
					posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
					if (!attrib.normals.empty()) {
						norBuf.push_back(attrib.normals[3 * idx.normal_index + 0]);
						norBuf.push_back(attrib.normals[3 * idx.normal_index + 1]);
						norBuf.push_back(attrib.normals[3 * idx.normal_index + 2]);
					}
					if (!attrib.texcoords.empty()) {
						texBuf.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
						texBuf.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
	cout << "Number of vertices: " << posBuf.size() / 3 << endl;


	// Holds all vertices
	vector<Vertex> vertices;

	// For mesh bounding box													 
	double xmin = 1.0, xmax = 0, ymin = 1.0, ymax = 0;

	// Loop through posBuf, create vertices, and track minsand maxes
	for (int i = 0; i < posBuf.size() / 3; ++i) {

		// Get x,y,z
		double x = posBuf.at(3 * i);
		double y = posBuf.at(3 * i + 1);
		double z = posBuf.at(3 * i + 2);

		// Track mins and maxes for entire mesh
		if (x < xmin)
			xmin = x;
		if (x > xmax)
			xmax = x;
		if (y < ymin)
			ymin = y;
		if (y > ymax)
			ymax = y;

		// Create vertex and add to vector
		Vertex V(x, y, z);
		vertices.push_back(V);
	}

	// Compute translation

	// Compute Sx and and Sy (scale)
	double Sy = height / (ymax - ymin);
	double Sx = width / (xmax - xmin);

	double scale = Sy;
	if (Sx < Sy) {
		scale = Sx;
	}

	// Middle of world
	double midxw = 0.5 * (xmin + xmax);
	double midyw = 0.5 * (ymin + ymax);

	// Calculate translations tx and ty
	double tx = (0.5 * width) - (scale * midxw);
	double ty = (0.5 * height) - (scale * midyw);

	// Create vector of triangles
	unsigned char r;
	unsigned char g;
	unsigned char b;
	vector<Triangle> triangles;

	for (int i = 0; i < vertices.size(); i++) {

		// Translate each vertex assign random rgb then create triangle

		Vertex At(scale * vertices.at(i).x + tx, scale * vertices.at(i).y + ty, vertices.at(i).z);
		r = (unsigned char)(RANDOM_COLORS[i % 7][0] * 255);
		g = (unsigned char)(RANDOM_COLORS[i % 7][1] * 255);
		b = (unsigned char)(RANDOM_COLORS[i % 7][2] * 255);
		At.setRBG(r, g, b);

		i++;
		Vertex Bt(scale * vertices.at(i).x + tx, scale * vertices.at(i).y + ty, vertices.at(i).z);
		r = (unsigned char)(RANDOM_COLORS[i % 7][0] * 255);
		g = (unsigned char)(RANDOM_COLORS[i % 7][1] * 255);
		b = (unsigned char)(RANDOM_COLORS[i % 7][2] * 255);
		Bt.setRBG(r, g, b);

		i++;
		Vertex Ct(scale * vertices.at(i).x + tx, scale * vertices.at(i).y + ty, vertices.at(i).z);
		r = (unsigned char)(RANDOM_COLORS[i % 7][0] * 255);
		g = (unsigned char)(RANDOM_COLORS[i % 7][1] * 255);
		b = (unsigned char)(RANDOM_COLORS[i % 7][2] * 255);
		Ct.setRBG(r, g, b);

		Triangle tri(At, Bt, Ct);
		triangles.push_back(tri);
	}


	// Create 2-D array for z-buffer
	double** zbuffer = new double* [height];
	for (int i = 0; i < height; ++i) {
		zbuffer[i] = new double[width];
	}

	// Initialize z-buffer 
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			zbuffer[y][x] = -999999999999.0;
		}
	}

	// Loop through all triangles and compute translation and set pixels
	for (int i = 0; i < triangles.size(); ++i) {

		// Compute translation per triangle
		Triangle tri = triangles.at(i);

		tri.printTriangle();
		cout << endl;

		// Color triangles within the triangles own bounded box
		for (int y = tri.ymin; y < tri.ymax; ++y) {
			for (int x = tri.xmin; x < tri.xmax; ++x) {

				// Compute barrycentric coordinates
				BC abc = barycentric(x, y, tri);

				// If in triangle
				if (abc.a >= 0 && abc.b >= 0 && abc.c >= 0) {

					// Compute pixels z
					double z = (abc.a * tri.A.z) + (abc.b * tri.B.z) + (abc.c * tri.C.z);
					// if z > curr z write to image 
					if (z > zbuffer[y][x]) {
						r = ((abc.a * tri.A.r) + (abc.b * tri.B.r) + (abc.c * tri.C.r));
						g = ((abc.a * tri.A.g) + (abc.b * tri.B.g) + (abc.c * tri.C.g));
						b = ((abc.a * tri.A.b) + (abc.b * tri.B.b) + (abc.c * tri.C.b));
						image->setPixel(x, y, r, g, b);
						zbuffer[y][x] = z;
					}
				}
			}
		}
	}

	// Write image to file
	image->writeToFile(outfile);

	return 0;
}

BC barycentric(double Px, double Py, Triangle tri) {
	// How to go CCW?

	BC abc;
	abc.a = 0;
	abc.b = 0;
	abc.c = 0;

	// Find total area of triangle
	// Area(v1, v2, v3) = 0.5*[ (X1-X3)*(Y2-Y3) - (X2-X3)*(Y1-Y3)  ]
	float arg1 = (tri.A.x - tri.C.x) * (tri.B.y - tri.C.y);
	float arg2 = (tri.B.x - tri.C.x) * (tri.A.y - tri.C.y);
	float totalArea = 0.5 * (arg1 - arg2);

	// areaA = area(P1, V2, V3)
	//arg1 = (tri.A.x - Px) * (tri.B.y - Py);
	//arg2 = (tri.B.x - Px) * (tri.A.y - Py);
	arg1 = (Px - tri.C.x) * (tri.B.y - tri.C.y);
	arg2 = (tri.B.x - tri.C.x) * (Py - tri.C.y);
	float areaV1 = (0.5 * (arg1 - arg2));

	// areaB = area(P2, V3, V1)
	//arg1 = (Px - tri.C.x) * (tri.B.y - tri.C.y);
	//arg2 = (tri.B.x - tri.C.x) * (Py - tri.C.y);
	arg1 = (tri.A.x - tri.C.x) * (Py - tri.C.y);
	arg2 = (Px - tri.C.x) * (tri.A.y - tri.C.y);
	float areaV2 = (0.5 * (arg1 - arg2));

	// areaC = area(P3, V1, V2)
	//arg1 = (tri.A.x - tri.C.x) * (Py - tri.C.y);
	//arg2 = (Px - tri.C.x) * (tri.A.y - tri.C.y);
	arg1 = (tri.A.x - Px) * (tri.B.y - Py);
	arg2 = (tri.B.x - Px) * (tri.A.y - Py);

	float areaV3 = (0.5 * (arg1 - arg2));

	abc.a = areaV1 / totalArea;
	abc.b = areaV2 / totalArea;
	abc.c = areaV3 / totalArea;

	return abc;
}

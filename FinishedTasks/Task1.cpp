/*
Task 1
Need to change to accept command line arguments
*/

#include <iostream>
#include <string>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Image.h"
#include "Vertex.h"
#include "Triangle.h"
using namespace std;

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
	string meshName = "../resources/bunny.obj";
	string outfile = "bunny_task1.png";
	int height = 512;
	int width = 512;
	int task = 1;

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
	double Sy = height / ymax - ymin;
	double Sx = width / xmax = xmin);

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
		i++;
		Vertex Bt(scale * vertices.at(i).x + tx, scale * vertices.at(i).y + ty, vertices.at(i).z);
		i++;
		Vertex Ct(scale * vertices.at(i).x + tx, scale * vertices.at(i).y + ty, vertices.at(i).z);
		Triangle tri(At, Bt, Ct);
		triangles.push_back(tri);
	}

	// Loop through all triangles and compute translation and set pixels
	for (int i = 0; i < triangles.size(); ++i) {

		// Compute translation per triangle
		Triangle tri = triangles.at(i);

		// Color triangles within the triangles own bounded box
		for (int y = tri.ymin; y < tri.ymax; ++y) {
			for (int x = tri.xmin; x < tri.xmax; ++x) {
				// Color from 2-d array
				unsigned char r = (unsigned char)(RANDOM_COLORS[i % 7][0] * 255);
				unsigned char g = (unsigned char)(RANDOM_COLORS[i % 7][1] * 255);
				unsigned char b = (unsigned char)(RANDOM_COLORS[i % 7][2] * 255);
				image->setPixel(x, y, r, g, b);
			}
		}

	}

	// Write image to file
	image->writeToFile(outfile);

	return 0;
}
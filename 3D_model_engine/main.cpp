#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <strstream>
//include <utility>
#include <algorithm>
#include<iostream>
using namespace std;


void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, sf::RenderWindow& window)
{
	sf::VertexArray triangle(sf::Lines, 6);
	triangle[0].position = { x1, y1 };
	triangle[1].position = { x2, y2 };

	triangle[2].position = { x2, y2 };
	triangle[3].position = { x3, y3 };

	triangle[4].position = { x3, y3 };
	triangle[5].position = { x1, y1 };
	for (int i = 0; i < 6; i++) {
		triangle[i].color = sf::Color::Green;
	}

	window.draw(triangle);
}

void DrawFilledTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float grayvalue, sf::RenderWindow& window)
{
	sf::VertexArray triangle(sf::Triangles, 3);
	triangle[0].position = { x1, y1 };
	triangle[1].position = { x2, y2 };
	triangle[2].position = { x3, y3 };
	float mappedValue = (grayvalue + 1.0f) / 2.0f;
	sf::Uint8 grayscale = static_cast<sf::Uint8>(mappedValue * 255);
	for (int i = 0; i < 3; ++i) {
		triangle[i].color = sf::Color(grayscale, grayscale, grayscale);
	}
	window.draw(triangle);
}


struct vec3d
{
	float x, y, z;
};
struct triangle
{
	vec3d p[3];
};

struct mesh
{
	vector <triangle> tris;
};
struct mat4x4
{
	float m[4][4] = { 0 };
};

class My3DEngin {

public:
	My3DEngin(float ScreenWidth, float ScreenHeight, float Accumulated_time, mesh& meshData) :meshCube(meshData)
	{
		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = 90.0f;
		float fAspectRatio = ScreenHeight / ScreenWidth;
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

		this->matProj.m[0][0] = fAspectRatio * fFovRad;
		this->matProj.m[1][1] = fFovRad;
		this->matProj.m[2][2] = fFar / (fFar - fNear);
		this->matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		this->matProj.m[2][3] = 1.0f;
		this->matProj.m[3][3] = 0.0f;

		this->fTheta = 1.0f * Accumulated_time;

	}

	void UserUpdate(float ScreenWidth, float ScreenHeight, float Accumulated_time, sf::RenderWindow& window)
	{
		mat4x4 matRotZ, matRotX;
		fTheta = 1.0f * Accumulated_time;

		// Rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// Rotation X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1;

		// Store triagles for rastering later
		vector<pair<triangle, float>> vecTrianglesToRaster;
		float grayvalue;
		// Draw Triangles
		for (auto tri : meshCube.tris)
		{
			triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;


			// Rotate in Z-Axis
			MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
			MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
			MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

			// Rotate in X-Axis
			MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
			MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
			MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

			// Offset into the screen
			triTranslated = triRotatedZX;
			triTranslated.p[0].z = triRotatedZX.p[0].z + 9.0f;
			triTranslated.p[1].z = triRotatedZX.p[1].z + 9.0f;
			triTranslated.p[2].z = triRotatedZX.p[2].z + 9.0f;

			// Use Cross-Product to get surface normal
			vec3d normal, line1, line2;
			line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
			line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
			line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

			line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
			line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
			line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

			normal.x = line1.y * line2.z - line1.z * line2.y;
			normal.y = line1.z * line2.x - line1.x * line2.z;
			normal.z = line1.x * line2.y - line1.y * line2.x;

			// It's normally normal to normalise the normal
			float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
			normal.x /= l; normal.y /= l; normal.z /= l;

			

			if (normal.x * (triTranslated.p[0].x - vCamera.x) + 
			   normal.y * (triTranslated.p[0].y - vCamera.y) +
			   normal.z * (triTranslated.p[0].z - vCamera.z) < 0.0f)
			{
				// Illumination
				vec3d light_direction = { -1.0f, -1.0f, -1.0f };
				float l = sqrtf(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
				light_direction.x /= l; light_direction.y /= l; light_direction.z /= l;

				

				// Project triangles from 3D --> 2D
				MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
				MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
				MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);

				// Scale into view
				triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
				triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
				triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
				triProjected.p[0].x *= 0.5f * ScreenWidth;
				triProjected.p[0].y *= 0.5f * ScreenHeight;
				triProjected.p[1].x *= 0.5f * ScreenWidth;
				triProjected.p[1].y *= 0.5f * ScreenHeight;
				triProjected.p[2].x *= 0.5f * ScreenWidth;
				triProjected.p[2].y *= 0.5f * ScreenHeight;

				

				// How similar is normal to light direction
				grayvalue=normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;
				// Store triangle for sorting
				vecTrianglesToRaster.push_back(make_pair(triProjected, grayvalue));


			}
		}

		// Sort triangles from back to front
		sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](auto& t1, auto& t2)
			{
				float z1 = (t1.first.p[0].z + t1.first.p[1].z + t1.first.p[2].z) / 3.0f;
				float z2 = (t2.first.p[0].z + t2.first.p[1].z + t2.first.p[2].z) / 3.0f;
				return z1 > z2;
			});
		
		for (auto& triProjected : vecTrianglesToRaster)
		{
			
			DrawFilledTriangle(triProjected.first.p[0].x, triProjected.first.p[0].y, triProjected.first.p[1].x, triProjected.first.p[1].y,
				triProjected.first.p[2].x, triProjected.first.p[2].y, triProjected.second, window);
			//cout << triProjected.second << endl;
			/*
			DrawTriangle(triProjected.first.p[0].x, triProjected.first.p[0].y, triProjected.first.p[1].x, triProjected.first.p[1].y,
				triProjected.first.p[2].x, triProjected.first.p[2].y, window);
				*/
		}

	}



private:
	mesh& meshCube;
	mat4x4 matProj;

	vec3d vCamera = { 0,0,0 };

	float fTheta;

	void MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& m)
	{
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f)
		{
			o.x /= w; o.y /= w; o.z /= w;
		}
	}


};




int main()
{
	float ScreenHeight = 1000;
	float ScreenWidth = 1000;
	sf::VideoMode mode(ScreenWidth, ScreenHeight);
	sf::RenderWindow window(mode, L"3D_Engine");

	double ElapsedTime = 0.01;
	double Accumulated_time = 10;
	mesh meshDataNo1;
	
	ifstream f("VideoShip.obj");
	if (!f.is_open())
		return false;

	// Local cache of verts
	vector<vec3d> verts;

	while (!f.eof())
	{
		char line[128];
		f.getline(line, 128);

		strstream s;
		s << line;

		char junk;

		if (line[0] == 'v')
		{
			vec3d v;
			s >> junk >> v.x >> v.y >> v.z;
			verts.push_back(v);
		}

		if (line[0] == 'f')
		{
			int f[3];
			s >> junk >> f[0] >> f[1] >> f[2];
			meshDataNo1.tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
		}
	}


	My3DEngin cube(ScreenWidth, ScreenHeight, Accumulated_time, meshDataNo1);
	
	sf::Clock clock;


	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		// 清空窗口


		if (clock.getElapsedTime().asSeconds() >= ElapsedTime)
		{
			window.clear();
			Accumulated_time += 0.01;
			cube.UserUpdate(ScreenWidth, ScreenHeight, Accumulated_time, window);
			
			clock.restart();
			window.display();
		}



		// 顯示

	}


}
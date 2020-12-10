#include <stdio.h>
#include <sys/types.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>

#include <tests.h>
#include <FixedPoint.h>

DISPENV disp[2];
DRAWENV draw[2];
int db;

void init(void)
{
	ResetGraph(0);

	SetDefDispEnv(&disp[0], 0, 0, 320, 240);
	SetDefDispEnv(&disp[1], 0, 240, 320, 240);

	SetDefDrawEnv(&draw[0], 0, 240, 320, 240);
	SetDefDrawEnv(&draw[1], 0, 0, 320, 240);

	setRGB0(&draw[0], 0, 96, 0);
	setRGB0(&draw[1], 0, 96, 0);
	draw[0].isbg = 1;
	draw[1].isbg = 1;

	db = 0;

	PutDispEnv(&disp[db]);
	PutDrawEnv(&draw[db]);

	FntLoad(960, 0);
	FntOpen(0, 8, 320, 224, 0, 100);
}

void display(void)
{
	db = !db;
	DrawSync(0);
	VSync(0);
	PutDispEnv(&disp[db]);
	PutDrawEnv(&draw[db]);
	SetDispMask(1);
	
}

typedef FixedPoint<12> Float;

//https://stackoverflow.com/questions/53962225/how-to-know-if-a-line-segment-intersects-a-triangle-in-3d-space
/*olc::GFX3D::vec3d olc::GFX3D::Math::Vec_IntersectPlane(olc::GFX3D::vec3d &plane_p, olc::GFX3D::vec3d &plane_n, olc::GFX3D::vec3d &lineStart, olc::GFX3D::vec3d &lineEnd, float &t)
	{
		plane_n = Vec_Normalise(plane_n);
		float plane_d = -Vec_DotProduct(plane_n, plane_p);
		float ad = Vec_DotProduct(lineStart, plane_n);
		float bd = Vec_DotProduct(lineEnd, plane_n);
		t = (-plane_d - ad) / (bd - ad);
		olc::GFX3D::vec3d lineStartToEnd = Vec_Sub(lineEnd, lineStart);
		olc::GFX3D::vec3d lineToIntersect = Vec_Mul(lineStartToEnd, t);
		return Vec_Add(lineStart, lineToIntersect);
	}*/

	/*

	bool intersect_triangle(
    in Ray R, in vec3 A, in vec3 B, in vec3 C, out float t, 
    out float u, out float v, out vec3 N
) { 
   vec3 E1 = B-A;
   vec3 E2 = C-A;
         N = cross(E1,E2);
   float det = -dot(R.Dir, N);
   float invdet = 1.0/det;
   vec3 AO  = R.Origin - A;
   vec3 DAO = cross(AO, R.Dir);
   u =  dot(E2,DAO) * invdet;
   v = -dot(E1,DAO) * invdet;
   t =  dot(AO,N)  * invdet; 
   return (det >= 1e-6 && t >= 0.0 && u >= 0.0 && v >= 0.0 && (u+v) <= 1.0);
}*/


int main(int argc, const char *argv[])
{
	int counter = 0;
	
	init();


	while(1)
	{
		int failedTests = 0;
		failedTests += test("Boolean test true", true);
		failedTests += test("Testing from_Int(1)", Float(1).rawValue() == 4096);
		failedTests += test("Testing from_Int(32)", Float(32).rawValue() == 131072);
		failedTests += test("Testing from_float(16)", Float(16.0f).rawValue() == 65536);
		failedTests += test("Testing sum 16 + 16", (Float(16) + Float(16)).rawValue() == 131072);
		failedTests += test("Testing sub 32 - 16", (Float(32) - Float(16)).rawValue() == 65536);
		failedTests += test("Testing mul 2 * 8", (Float(2) * Float(8)).rawValue() == 65536);
		failedTests += test("Testing div 64 / 2", (Float(64) / Float(2)).rawValue() == 131072);
		failedTests += test("Testing div 64.0f / 2", (Float(64.0f) / Float(2)).rawValue() == 131072);

		failedTests += test("Testing div 64.0f / int(2)", (Float(64.0f) / 2).rawValue() == 131072);
		failedTests += test("Testing mul 2 * int(8)", (Float(2) * 8).rawValue() == 65536);

		failedTests += test("Testing sub 32 - int(16)", (Float(32) - 16).rawValue() == 65536);
		failedTests += test("Testing sum 16 + int(16)", (Float(16) + 16).rawValue() == 131072);

		failedTests += test("Testing == 16 == (2 * 8)", Float(16) == Float(2) * Float(8));
		failedTests += test("Testing == 16 == int(2 * 8)", Float(16) == 2 * 8);

		failedTests += test("Testing == 16.f == float(16.f)", Float(3.141f) == 3.141f);

		failedTests += test("Testing PI()", Float(3.141f) == 3.141f);

		failedTests += test("Testing PI() != less than 0.001", Float::PI() - 3.141f < 0.001f);

		FntPrint(-1, "PI: %.4f\n", Float::PI().toFloat());
		FntPrint(-1, "PI: %.4f\n", Float(3.1415f).toFloat());
		if(failedTests == 0)
		{
			FntPrint(-1, "All tests passed!\n");
		}			
		else
		{
			FntPrint(-1, "%d tests failed!\n", failedTests);
		}

		FntFlush(-1);
		display();
		counter++;
	}
	
	return 0;
}

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

//int = 32bit
//20bit for int part + 12bits for floating point part
void page1()
{
	FntPrint(-1, "page 1\n");
	test("Boolean test true", true);
	test("Testing from_Int(32)", Float::fromInt(32).rawValue() == 131072);
	test("Testing from_float(16)", Float::fromFloat(16.0f).rawValue() == 65536);
	
}

void page2()
{
	FntPrint(-1, "page 2\n");
	test("Testing sum 16 + 16", (Float::fromInt(16) + Float::fromInt(16)).rawValue() == 131072);
	test("Testing sub 32 - 16", (Float::fromInt(32) - Float::fromInt(16)).rawValue() == 65536);
	test("Testing mul 2 * 8", (Float::fromInt(2) * Float::fromInt(8)).rawValue() == 65536);
}

void page3()
{
	FntPrint(-1, "page 3\n");
	test("Testing div 64 / 2", (Float::fromInt(64) / Float::fromInt(2)).rawValue() == 131072);
	test("Testing div 64.0f / 2", (Float::fromFloat(64.0f) / Float::fromInt(2)).rawValue() == 131072);
}
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


	int pageCountTimer = 60*2;
	int test_page = 1;
	while(1)
	{
		if(pageCountTimer-- == 0)
		{
			test_page++;
			pageCountTimer = 60*2;
		}

		switch (test_page)
		{
		case 1:
			page1();
			break;
		case 2:
			page2();
			break;
		case 3:
			page3();
			break;
		default:
			test_page = 1;
			break;
		}
		
		FntFlush(-1);
		display();
		counter++;
	}
	
	return 0;
}

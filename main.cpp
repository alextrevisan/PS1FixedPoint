#include <stdio.h>
#include <sys/types.h>
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>
#include <psxapi.h>

#include <tests.h>
#include <FixedPoint.h>
#include <Vector.h>
#include <Triangle.h>

DISPENV disp[2];
DRAWENV draw[2];
int db;

typedef FixedPoint<12, int, long long> Float;
typedef FixedPoint<12, short, long long> SFloat;

template<typename T>
constexpr T abs(const T& value)
{
	if(value > 0)
		return value;
	return value * -1;
}

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
	//DrawSync(0);
	//VSync(0);
	PutDispEnv(&disp[db]);
	PutDrawEnv(&draw[db]);
	SetDispMask(1);
	
}


int main(int argc, const char *argv[])
{
	#define RCntIntr		0x1000				/*Interrupt mode*/
	SetRCnt(RCntCNT1, 0xffff, RCntIntr);
	StartRCnt(RCntCNT1);
	int counter = 0;
	
	init();
	volatile int v1 = 1;
	volatile int v2 = 50;
	volatile int v3 = 6;
	
	while(1)
	{
		Assert.Reset();

		//Basic Float tests
		Assert.AreEqual(Float(1).AsFixedPoint(), 4096, __LINE__);
		Assert.AreEqual(Float(32).AsFixedPoint(), 131072, __LINE__);
		Assert.AreEqual(Float(16).AsFixedPoint(), 65536, __LINE__);

		Assert.AreEqual(-Float(16).AsInt(), -16, __LINE__);

		Assert.AreEqual((Float(16) + Float(16)), Float(32), __LINE__, "operator+");
		Assert.AreEqual((Float(32) - Float(16)), Float(16), __LINE__, "operator-");
		Assert.AreEqual((Float(16) * Float(2)), Float(32), __LINE__, "operator*");
		Assert.AreEqual((Float(32) / Float(2)), Float(16), __LINE__, "operator/");

		Assert.AreEqual( (~Float(32)).AsInt(), ~32, __LINE__, "operator~");
		Assert.AreEqual( (+Float(32)).AsInt(), +32, __LINE__, "operator+");
		Assert.AreEqual( (-Float(32)).AsInt(), -32, __LINE__, "operator-");

		Assert.AreEqual<bool>(Float(32) == Float(32), true, __LINE__, "operator==");
		Assert.AreEqual<bool>(Float(0) != Float(32), true, __LINE__, "operator!=");
		
		Assert.AreEqual<bool>(Float(31) < Float(32), true, __LINE__, "operator<");
		Assert.AreEqual<bool>(Float(31) <= Float(32), true, __LINE__, "operator<=");
		Assert.AreEqual<bool>(Float(31) <= Float(31), true, __LINE__, "operator<=");

		Assert.AreEqual<bool>(Float(32) > Float(31), true, __LINE__, "operator>");
		Assert.AreEqual<bool>(Float(32) >= Float(31), true, __LINE__, "operator>=");
		Assert.AreEqual<bool>(Float(32) >= Float(32), true, __LINE__, "operator>=");

		Assert.AreEqual( Float(16)+=16, Float(32), __LINE__, "operator+=");
		Assert.AreEqual( Float(16)-=8, Float(8), __LINE__, "operator-=");
		Assert.AreEqual( Float(16)*=2, Float(32), __LINE__, "operator*=");
		Assert.AreEqual( Float(32)/=2, Float(16), __LINE__, "operator/=");
		Assert.AreEqual( (Float(32)%=2).AsInt(), 32%2, __LINE__, "operator%=");

		Assert.AreEqual( (Float(16)|=8).AsInt(), 16|8, __LINE__, "operator|=");
		Assert.AreEqual( (Float(16)&=2).AsInt(), 16&2, __LINE__, "operator&=");
		Assert.AreEqual( (Float(32)^=2).AsInt(), 32^2, __LINE__, "operator^=");

		Assert.AreEqual( (Float(16)>>1).AsInt(), 16>>1, __LINE__, "operator>>");
		Assert.AreEqual( (Float(16)<<1).AsInt(), 16<<1, __LINE__, "operator<<");
		
		Assert.AreEqual( (Float(16)>>=1).AsInt(), 16>>1, __LINE__, "operator>>=");
		Assert.AreEqual( (Float(16)<<=1).AsInt(), 16<<1, __LINE__, "operator<<=");

		Assert.AreEqual( (Float(16)++).AsInt(), 17, __LINE__, "operator++");
		Assert.AreEqual( (Float(16)--).AsInt(), 15, __LINE__, "operator--");

		Assert.AreEqual( Float(-16).Abs(), Float(16), __LINE__, "Abs");		

		Assert.IsLessThan(abs(Float::PI().AsFloat() - 3.14159265359f), 0.001f, __LINE__);

		Assert.AreEqual(Float::FromFixedPoint(SquareRoot12(Float(49).AsFixedPoint())), Float(7), __LINE__);
																						 //round error from 5.65685424949
		Assert.AreEqual(Float::FromFixedPoint(SquareRoot12(Float(32).AsFixedPoint())).AsFixedPoint(), Float(5.6564f).AsFixedPoint(), __LINE__);

		Vector3D vector(1,50,6);
		const auto dot = vector.dotProduct(vector);
		
		Assert.AreEqual(dot.AsInt(), 2537, __LINE__);

		//There are some precision differences in the SquareRoot12 and sqrt
		Assert.AreEqual(SquareRoot12(2537<<12), Float(50.2736f).AsFixedPoint(), __LINE__);

		const auto magnitude = vector.length();
		Assert.AreEqual(magnitude.AsFixedPoint(), Vector3D::_Float(50.2736f).AsFixedPoint(), __LINE__);

		const Vector3D nomalized = vector.normalize(vector);
		const Vector3D test(0.0199f, 0.9946f, 0.1192f);
		Assert.AreEqual(nomalized.vx.AsFixedPoint(), test.vx.AsFixedPoint(), __LINE__);
		Assert.AreEqual(nomalized.vy.AsFixedPoint(), test.vy.AsFixedPoint(), __LINE__);
		Assert.AreEqual(nomalized.vz.AsFixedPoint(), test.vz.AsFixedPoint(), __LINE__);

		Vector3D input{1.0f,0.75f,0.5f};
        SVector3D output;
        VectorNormalS(input, output);
		Assert.AreEqual(output.vx, SFloat(0.742781352f), __LINE__);
		Assert.AreEqual(output.vy, SFloat(0.557086014f), __LINE__);
		Assert.AreEqual(output.vz, SFloat(0.371390676f), __LINE__);
		Assert.AreEqual((int)sizeof(input),12, __LINE__);
		Assert.AreEqual((int)sizeof(output),6, __LINE__);

		
		//Performance test
		/*
		int OldValue = 0;
		ResetRCnt(RCntCNT1);
		OldValue = GetRCnt(RCntCNT1);
		int value = 0;
		//const Vector3D vectorTest(v1,v2,v3);
		
		for(int i = 0; i < 10000; ++i)
		{
			Vector3D input{v1,v2,v3};	
			const auto normal = input.normalize();
			value += normal.vx.AsFixedPoint();
		}
		int NewValue = GetRCnt(RCntCNT1) - OldValue;
		FntPrint(-1, "It took %d HBlanks! Count=%d\n", NewValue, value);

		ResetRCnt(RCntCNT1);
		OldValue = GetRCnt(RCntCNT1);
		value = 0;
		//const Vector3D vectorTest(v1,v2,v3);
		
		for(int i = 0; i < 10000; ++i)
		{
			VECTOR input{v1,v2,v3};
			SVECTOR output;
			VectorNormalS(&input, &output);
			value += output.vx;
		}
		NewValue = GetRCnt(RCntCNT1) - OldValue;
		FntPrint(-1, "It took %d HBlanks! Count=%d\n", NewValue, value);
		*/

		if(Assert.ErrorCount() == 0)
		{
			FntPrint(-1, "All %d tests passed!\n", Assert.TestCount());
		}
		else
		{
			FntPrint(-1, "%d tests failed! \n%s\n",  Assert.ErrorCount(), Assert.LastError());
		}
		

		FntFlush(-1);
		display();
		counter++;
	}
	
	return 0;
}

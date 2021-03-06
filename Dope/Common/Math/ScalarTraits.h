#ifndef DP_SCALAR_TRAITS_H
#define DP_SCALAR_TRAITS_H
#include <cmath>
#include <cfloat>
namespace dp {

	template <typename ScalarT>
	struct ScalarTraits {};

	template<>
	struct ScalarTraits<float>
	{
		static float pi2() { return 6.283185307179586232f; }
		static float epsilon() { return FLT_EPSILON; }//machine epsilon
		static float max() { return FLT_MAX; }

		static float random() { return float(::rand()) / float(RAND_MAX); }
		static float sqrt(float x) { return ::sqrtf(x); } 
		static float abs(float x)  { return ::fabsf(x); } 

		static float cos(float x) { return ::cosf(x); } 
		static float sin(float x) { return ::sinf(x); } 
		static float tan(float x) { return ::tanf(x); } 

		static float acos(float x) { return ::acosf(x); } 
		static float asin(float x) { return ::asinf(x); } 
		static float atan(float x) { return ::atanf(x); } 
		static float atan2(float x, float y) { return ::atan2f(x, y); } 

		static float exp(float x) { return ::expf(x); } 
		static float log(float x) { return ::logf(x); } 
		static float pow(float x, float y) { return ::powf(x, y); } 
	};

	template<>
	struct ScalarTraits<double> 
	{
		static double pi2()        { return 6.283185307179586232; }
		static double epsilon()    { return DBL_EPSILON; }//machine epsilon
		static double max()        { return DBL_MAX; }

		static double random()       { return double(::rand()) / double(RAND_MAX); }
		static double sqrt(double x) { return ::sqrt(x); } 
		static double abs(double x)  { return ::fabs(x); } 

		static double cos(double x)  { return ::cos(x); } 
		static double sin(double x)  { return ::sin(x); } 
		static double tan(double x)  { return ::tan(x); } 

		static double acos(double x) { return ::acos(x); } 
		static double asin(double x) { return ::asin(x); } 
		static double atan(double x) { return ::atan(x); } 
		static double atan2(double x, double y) { return ::atan2(x, y); } 

		static double exp(double x)  { return ::exp(x); } 
		static double log(double x)  { return ::log(x); } 
		static double pow(double x, double y) { return ::pow(x, y); } 
	};

}//ns dp
#endif //DP_SCALAR_TRAITS_H

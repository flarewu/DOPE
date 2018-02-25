#ifndef DP_REAL_H
#define DP_REAL_H

#include <Dope/Common/Math/ScalarTraits.h>
#define USE_TRACER
#ifdef USE_TRACER
#include <Dope/Common/Math/TracedFloat.h>
#endif

namespace dp {

#ifdef USE_DOUBLE
    using Float = double;
#else
    using Float = float;
#endif

#ifdef USE_TRACER
    using Real = TracedFloat<Float>;
#else
	using Real = Float;
#endif
    using RealTraits = ScalarTraits<Real>;

	const Real PI_2    = RealTraits::pi2();
	const Real PI      = PI_2 * Real(0.5);
	const Real PI_HALF = PI_2 * Real(0.25);
	const Real DEG2RAD = PI_2 / Real(360.0);
	const Real RAD2DEG = Real(360.0) / PI_2;

    const Real EPSILON  = RealTraits::epsilon();
    //const Real INFINITY = RealTraits::max();


    inline Real random()              { return RealTraits::random(); }
    inline Real abs(Real x)           { return RealTraits::abs(x); }
    inline Real sqrt(Real x)          { return RealTraits::sqrt(x); }

    inline Real cos(Real x)           { return RealTraits::cos(x); }
    inline Real sin(Real x)           { return RealTraits::sin(x); }
    inline Real tan(Real x)           { return RealTraits::tan(x); }

    inline Real acos(Real x)          { return RealTraits::acos(x); }
    inline Real asin(Real x)          { return RealTraits::asin(x); }
    inline Real atan(Real x)          { return RealTraits::atan(x); }
    inline Real atan2(Real x, Real y) { return RealTraits::atan2(x, y); }

    inline Real deg2rad(Real x) { return x * DEG2RAD; }
    inline Real rad2deg(Real x) { return x * RAD2DEG; }

}//ns dp
#endif //DP_REAL_H

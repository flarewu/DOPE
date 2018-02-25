#ifndef DP_MATH_H
#define DP_MATH_H

#include <Dope/Common/Math/Real.h>
#include <Dope/Common/Math/TMatrix3.h>
#include <Dope/Common/Math/TQuaternion.h>
#include <Dope/Common/Math/TTransform.h>
#include <Dope/Common/Math/TVector3.h>

namespace dp {

    using Transform  = TTransform<Real>;
    using Quaternion = TQuaternion<Real>;
    using Vector3    = TVector3<Real>;
    using Matrix3    = TMatrix3<Real>;

}//ns dp


#endif //DP_MATH_H
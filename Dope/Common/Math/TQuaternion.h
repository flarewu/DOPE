#ifndef DP_TQUATERNION_H
#define DP_TQUATERNION_H

#include <Dope/Common/Math/TTuple4.h>
#include <Dope/Common/Math/TVector3.h>
namespace dp {

	template <typename ScalarT>	
	class TQuaternion : public TTuple4<ScalarT> 
    {
	public:
		explicit TQuaternion() {}
		
		template <typename ScalarT2>
		explicit TQuaternion(const ScalarT2 *v) 
            : TTuple4<ScalarT>(v) 
        {}
		
		template <typename ScalarT2>
		TQuaternion(const ScalarT2& x, const ScalarT2& y, const ScalarT2& z, const ScalarT2& w) 
			: TTuple4<ScalarT>(x, y, z, w) 
		{}
		
		TQuaternion(const TVector3<ScalarT>& axis, const ScalarT& angle) 
		{ 
			setRotation(axis, angle); 
		}

		template <typename ScalarT2>
		TQuaternion(const ScalarT2& yaw, const ScalarT2& pitch, const ScalarT2& roll)
		{ 
			setEuler(yaw, pitch, roll); 
		}

		void setRotation(const TVector3<ScalarT>& axis, const ScalarT& angle)
		{
			ScalarT d = axis.length();
			assert(d != ScalarT(0.0));
			ScalarT s = ScalarTraits<ScalarT>::sin(angle * ScalarT(0.5)) / d;
			setValue(axis[0] * s, axis[1] * s, axis[2] * s, 
					 ScalarTraits<ScalarT>::cos(angle * ScalarT(0.5)));
		}

		template <typename ScalarT2>
		void setEuler(const ScalarT2& yaw, const ScalarT2& pitch, const ScalarT2& roll)
		{
			ScalarT halfYaw = ScalarT(yaw) * ScalarT(0.5);  
			ScalarT halfPitch = ScalarT(pitch) * ScalarT(0.5);  
			ScalarT halfRoll = ScalarT(roll) * ScalarT(0.5);  
			ScalarT cosYaw   = ScalarTraits<ScalarT>::cos(halfYaw);
			ScalarT sinYaw   = ScalarTraits<ScalarT>::sin(halfYaw);
			ScalarT cosPitch = ScalarTraits<ScalarT>::cos(halfPitch);
			ScalarT sinPitch = ScalarTraits<ScalarT>::sin(halfPitch);
			ScalarT cosRoll  = ScalarTraits<ScalarT>::cos(halfRoll);
			ScalarT sinRoll  = ScalarTraits<ScalarT>::sin(halfRoll);
			setValue(cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw,
					 cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw,
					 sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw,
					 cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw);
		}
  
		TQuaternion<ScalarT>& operator+=(const TQuaternion<ScalarT>& q)
		{
			m_scalars[0] += q[0]; m_scalars[1] += q[1]; m_scalars[2] += q[2]; m_scalars[3] += q[3];
			return *this;
		}
		
		TQuaternion<ScalarT>& operator-=(const TQuaternion<ScalarT>& q) 
		{
			m_scalars[0] -= q[0]; m_scalars[1] -= q[1]; m_scalars[2] -= q[2]; m_scalars[3] -= q[3];
			return *this;
		}

		TQuaternion<ScalarT>& operator*=(const ScalarT& s)
		{
			m_scalars[0] *= s; m_scalars[1] *= s; m_scalars[2] *= s; m_scalars[3] *= s;
			return *this;
		}
		
		TQuaternion<ScalarT>& operator/=(const ScalarT& s) 
		{
			assert(s != ScalarT(0.0));
			return *this *= ScalarT(1.0) / s;
		}
  
		TQuaternion<ScalarT>& operator*=(const TQuaternion<ScalarT>& q)
		{
			setValue(m_scalars[3] * q[0] + m_scalars[0] * q[3] + m_scalars[1] * q[2] - m_scalars[2] * q[1],
					 m_scalars[3] * q[1] + m_scalars[1] * q[3] + m_scalars[2] * q[0] - m_scalars[0] * q[2],
					 m_scalars[3] * q[2] + m_scalars[2] * q[3] + m_scalars[0] * q[1] - m_scalars[1] * q[0],
					 m_scalars[3] * q[3] - m_scalars[0] * q[0] - m_scalars[1] * q[1] - m_scalars[2] * q[2]);
			return *this;
		}
	
		ScalarT dot(const TQuaternion<ScalarT>& q) const
		{
			return m_scalars[0] * q[0] + m_scalars[1] * q[1] + m_scalars[2] * q[2] + m_scalars[3] * q[3];
		}

		ScalarT lengthSq() const
		{
			return dot(*this);
		}

		ScalarT length() const
		{
			return ScalarTraits<ScalarT>::sqrt(lengthSq());
		}

		TQuaternion<ScalarT>& normalize() 
		{
			return *this /= length();
		}
		
		TQuaternion<ScalarT> getNormalize() const 
		{
			return *this / length();
		} 

		ScalarT angle(const TQuaternion<ScalarT>& q) const 
		{
			ScalarT s = ScalarTraits<ScalarT>::sqrt(lengthSq() * q.lengthSq());
			assert(s != ScalarT(0.0));
			return ScalarTraits<ScalarT>::acos(dot(q) / s);
		}
   
		TQuaternion<ScalarT> conjugate() const 
		{
			return TQuaternion<ScalarT>(-m_scalars[0], -m_scalars[1], -m_scalars[2], m_scalars[3]);
		}

		TQuaternion<ScalarT> inverse() const
		{
			return conjugate / lengthSq();
		}
		
		TQuaternion<ScalarT> slerp(const TQuaternion<ScalarT>& q, const ScalarT& t) const
		{
			ScalarT theta = angle(q);
			if (theta != ScalarT(0.0))
			{
				ScalarT d = ScalarT(1.0) / ScalarTraits<ScalarT>::sin(theta);
				ScalarT s0 = ScalarTraits<ScalarT>::sin((ScalarT(1.0) - t) * theta);
				ScalarT s1 = ScalarTraits<ScalarT>::sin(t * theta);   
				return TQuaternion<ScalarT>((m_scalars[0] * s0 + q[0] * s1) * d,
										  (m_scalars[1] * s0 + q[1] * s1) * d,
										  (m_scalars[2] * s0 + q[2] * s1) * d,
										  (m_scalars[3] * s0 + q[3] * s1) * d);
			}
			else
			{
				return *this;
			}
		}

		static TQuaternion<ScalarT> random() 
		{
			// From: "Uniform Random Rotations", Ken Shoemake, Graphics Gems III, pg. 124-132
			ScalarT x0 = ScalarTraits<ScalarT>::random();
			ScalarT r1 = ScalarTraits<ScalarT>::sqrt(ScalarT(1.0) - x0);
			ScalarT r2 = ScalarTraits<ScalarT>::sqrt(x0);
			ScalarT t1 = ScalarTraits<ScalarT>::pi2() * ScalarTraits<ScalarT>::random();
			ScalarT t2 = ScalarTraits<ScalarT>::pi2() * ScalarTraits<ScalarT>::random();
			ScalarT c1 = ScalarTraits<ScalarT>::cos(t1);
			ScalarT s1 = ScalarTraits<ScalarT>::sin(t1);
			ScalarT c2 = ScalarTraits<ScalarT>::cos(t2);
			ScalarT s2 = ScalarTraits<ScalarT>::sin(t2);
			return TQuaternion<ScalarT>(s1 * r1, c1 * r1, s2 * r2, c2 * r2);
		}

	};

	template <typename ScalarT>
	inline TQuaternion<ScalarT>
	operator+(const TQuaternion<ScalarT>& q1, const TQuaternion<ScalarT>& q2)
	{
		return TQuaternion<ScalarT>(q1[0] + q2[0], q1[1] + q2[1], q1[2] + q2[2], q1[3] + q2[3]);
	}
	
	template <typename ScalarT>
	inline TQuaternion<ScalarT>
	operator-(const TQuaternion<ScalarT>& q1, const TQuaternion<ScalarT>& q2)
	{
		return TQuaternion<ScalarT>(q1[0] - q2[0], q1[1] - q2[1], q1[2] - q2[2], q1[3] - q2[3]);
	}
	
	template <typename ScalarT>
	inline TQuaternion<ScalarT>
	operator-(const TQuaternion<ScalarT>& q)
	{
		return TQuaternion<ScalarT>(-q[0], -q[1], -q[2], -q[3]);
	}

	template <typename ScalarT>
	inline TQuaternion<ScalarT>
	operator*(const TQuaternion<ScalarT>& q, const ScalarT& s)
	{
		return TQuaternion<ScalarT>(q[0] * s, q[1] * s, q[2] * s, q[3] * s);
	}
	
	template <typename ScalarT>
	inline TQuaternion<ScalarT>
	operator*(const ScalarT& s, const TQuaternion<ScalarT>& q)
	{
		return q * s;
	}
	
	template <typename ScalarT>
	inline TQuaternion<ScalarT>
	operator*(const TQuaternion<ScalarT>& q1, const TQuaternion<ScalarT>& q2) {
		return TQuaternion<ScalarT>(q1[3] * q2[0] + q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1],
								  q1[3] * q2[1] + q1[1] * q2[3] + q1[2] * q2[0] - q1[0] * q2[2],
								  q1[3] * q2[2] + q1[2] * q2[3] + q1[0] * q2[1] - q1[1] * q2[0],
								  q1[3] * q2[3] - q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2]); 
	}
	
	template <typename ScalarT>
	inline TQuaternion<ScalarT>
	operator*(const TQuaternion<ScalarT>& q, const TVector3<ScalarT>& w)
	{
		return TQuaternion<ScalarT>( q[3] * w[0] + q[1] * w[2] - q[2] * w[1],
								   q[3] * w[1] + q[2] * w[0] - q[0] * w[2],
								   q[3] * w[2] + q[0] * w[1] - q[1] * w[0],
								  -q[0] * w[0] - q[1] * w[1] - q[2] * w[2]); 
	}
	
	template <typename ScalarT>
	inline TQuaternion<ScalarT>
	operator*(const TVector3<ScalarT>& w, const TQuaternion<ScalarT>& q)
	{
		return TQuaternion<ScalarT>( w[0] * q[3] + w[1] * q[2] - w[2] * q[1],
								   w[1] * q[3] + w[2] * q[0] - w[0] * q[2],
								   w[2] * q[3] + w[0] * q[1] - w[1] * q[0],
								  -w[0] * q[0] - w[1] * q[1] - w[2] * q[2]); 
	}
	template <typename ScalarT>
	inline ScalarT dot(const TQuaternion<ScalarT>& q1, const TQuaternion<ScalarT>& q2) 
	{ 
		return q1.dot(q2); 
	}
	template <typename ScalarT>
	inline ScalarT lengthSq(const TQuaternion<ScalarT>& q) 
	{ 
		return q.lengthSq(); 
	}

	template <typename ScalarT>
	inline ScalarT length(const TQuaternion<ScalarT>& q) 
	{ 
		return q.length(); 
	}

	template <typename ScalarT>
	inline ScalarT angle(const TQuaternion<ScalarT>& q1, const TQuaternion<ScalarT>& q2) 
	{ 
		return q1.angle(q2); 
	}

	template <typename ScalarT>
	inline TQuaternion<ScalarT> conjugate(const TQuaternion<ScalarT>& q) 
	{
		return q.conjugate();
	}

	template <typename ScalarT>
	inline TQuaternion<ScalarT> inverse(const TQuaternion<ScalarT>& q) 
	{
		return q.inverse();
	}

	template <typename ScalarT>
	inline TQuaternion<ScalarT> slerp(const TQuaternion<ScalarT>& q1, const TQuaternion<ScalarT>& q2, const ScalarT& t) 
	{
		return q1.slerp(q2, t);
	}
	
}//ns dp

#endif

#ifndef DP_TVECTOR3_H
#define DP_TVECTOR3_H

#include <Dope/Common/Math/TTuple3.h>

namespace dp {

	template <typename ScalarT>	
	class TVector3 : public TTuple3<ScalarT>
    {
    public:
        using BaseType   =  TTuple3<ScalarT>;
        using TraitsType =  ScalarTraits<ScalarT>;
    public:
        static TVector3 zero()
        {
            return TVector3(ScalarT(0),ScalarT(0) ,ScalarT(0) );
        }
	public:
		explicit TVector3() {}
	
		template <typename ScalarT2>
		explicit TVector3(const ScalarT2* v) 
            : BaseType(v)
        {}
		
		template <typename ScalarT2>
		TVector3(const ScalarT2& x, const ScalarT2& y, const ScalarT2& z) 
			: BaseType(x, y, z) 
		{}
		
		TVector3& operator += (const TVector3& rhs)
		{
            m_scalars[0] += rhs[0];
            m_scalars[1] += rhs[1];
            m_scalars[2] += rhs[2];
			return *this;
		}
		TVector3& operator -= (const TVector3& rhs) 
		{
			m_scalars[0] -= rhs[0]; 
            m_scalars[1] -= rhs[1]; 
            m_scalars[2] -= rhs[2];
			return *this;
		}
		TVector3& operator *= (const ScalarT& s)
		{
			m_scalars[0] *= s; 
            m_scalars[1] *= s; 
            m_scalars[2] *= s;
			return *this;
		}
		TVector3& operator /= (const ScalarT& s) 
		{
			assert(s != ScalarT(0.0));
			return *this *= ScalarT(1.0) / s;
		}
		ScalarT dot(const TVector3& rhs) const
		{
			return m_scalars[0] * rhs[0] + 
                   m_scalars[1] * rhs[1] + 
                   m_scalars[2] * rhs[2];
		}
		ScalarT lengthSq() const
		{
			return this->dot(*this);
		}
		ScalarT length() const
		{
			return TraitsType::sqrt(lengthSq());
		}
		ScalarT distanceSq(const TVector3& rhs) const 
		{
			return (rhs - *this).lengthSq();
		}
		ScalarT distance(const TVector3& rhs) const 
		{
			return (rhs - *this).length();
		}
		TVector3& normalize() 
		{
			return *this /= this->length();
		}
		TVector3 getNormalize() const 
		{
			return *this / this->length();
		} 
        // »¡¶È
		ScalarT angle(const TVector3& rhs) const 
		{
			auto s = TraitsType::sqrt(this->lengthSq() * v.lengthSq());
			assert(s != ScalarT(0.0));
			return TraitsType::acos(this->dot(rhs) / s);
		}
		TVector3 getAbsolute() const 
		{
			return TVector3(TraitsType::abs(m_scalars[0]), 
							TraitsType::abs(m_scalars[1]), 
							TraitsType::abs(m_scalars[2]));
		}
		TVector3 cross(const TVector3& v) const
		{
			return TVector3(m_scalars[1] * v[2] - m_scalars[2] * v[1],
							m_scalars[2] * v[0] - m_scalars[0] * v[2],
							m_scalars[0] * v[1] - m_scalars[1] * v[0]);
		}
		ScalarT triple(const TVector3& v1, const TVector3& v2) const
		{
			return m_scalars[0] * (v1[1] * v2[2] - v1[2] * v2[1]) + 
				   m_scalars[1] * (v1[2] * v2[0] - v1[0] * v2[2]) + 
				   m_scalars[2] * (v1[0] * v2[1] - v1[1] * v2[0]);
		}
		int minAxis() const
		{
			return m_scalars[0] < m_scalars[1] ? 
                  (m_scalars[0] < m_scalars[2] ? 0 : 2): 
                  (m_scalars[1] < m_scalars[2] ? 1 : 2);
		}
		int maxAxis() const 
		{
			return  m_scalars[0] < m_scalars[1] ? 
                   (m_scalars[1] < m_scalars[2] ? 2 : 1): 
                   (m_scalars[0] < m_scalars[2] ? 2 : 0);
		}
		int furthestAxis() const
		{
			return this->getAbsolute().minAxis();
		}
		int closestAxis() const 
		{
			return this->getAbsolute().maxAxis();
		}
		TVector3 lerp(const TVector3& v, const ScalarT& t) const 
		{
			return TVector3(m_scalars[0] + (v[0] - m_scalars[0]) * t,
							m_scalars[1] + (v[1] - m_scalars[1]) * t,
							m_scalars[2] + (v[2] - m_scalars[2]) * t);
		}
		static TVector3 random() 
		{
			auto z = ScalarT(2.0) * TraitsType::random() - ScalarT(1.0);
			auto r = TraitsType::sqrt(ScalarT(1.0) - z * z);
			auto t = TraitsType::pi2() * TraitsType::random();
			return TVector3(r * TraitsType::cos(t), 
                            r * TraitsType::sin(t), 
                            z);
		}
	};

	template <typename ScalarT>
	inline TVector3<ScalarT> operator+(const TVector3<ScalarT>& v1, const TVector3<ScalarT>& v2) 
	{
		return TVector3<ScalarT>(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
	}

	template <typename ScalarT>
	inline TVector3<ScalarT> operator-(const TVector3<ScalarT>& v1, const TVector3<ScalarT>& v2)
	{
		return TVector3<ScalarT>(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
	}
	
	template <typename ScalarT>
	inline TVector3<ScalarT>  operator-(const TVector3<ScalarT>& v)
	{
		return TVector3<ScalarT>(-v[0], -v[1], -v[2]);
	}
	
	template <typename ScalarT>
	inline TVector3<ScalarT> operator*(const TVector3<ScalarT>& v, const ScalarT& s)
	{
		return TVector3<ScalarT>(v[0] * s, v[1] * s, v[2] * s);
	}
	
	template <typename ScalarT>
	inline TVector3<ScalarT> operator*(const ScalarT& s, const TVector3<ScalarT>& v)
	{ 
		return v * s; 
	}
	
	template <typename ScalarT>
	inline TVector3<ScalarT> operator/(const TVector3<ScalarT>& v, const ScalarT& s)
	{
		assert(s != ScalarT(0.0));
		return v * (ScalarT(1.0) / s);
	}
	
	template <typename ScalarT>
	inline ScalarT dot(const TVector3<ScalarT>& v1, const TVector3<ScalarT>& v2) 
	{ 
		return v1.dot(v2); 
	}
	
	template <typename ScalarT> 
    inline ScalarT lengthSq(const TVector3<ScalarT>& v) 
	{ 
		return v.lengthSq(); 
	}

	template <typename ScalarT>
	inline ScalarT length(const TVector3<ScalarT>& v) 
	{ 
		return v.length(); 
	}

	template <typename ScalarT>
	inline ScalarT distanceSq(const TVector3<ScalarT>& v1, const TVector3<ScalarT>& v2) 
	{ 
		return v1.distanceSq(v2); 
	}

	template <typename ScalarT>
	inline ScalarT distance(const TVector3<ScalarT>& v1, const TVector3<ScalarT>& v2) 
	{ 
		return v1.distance(v2); 
	}

	template <typename ScalarT>
	inline ScalarT angle(const TVector3<ScalarT>& v1, const TVector3<ScalarT>& v2) 
	{ 
		return v1.angle(v2); 
	}

	template <typename ScalarT>
	inline TVector3<ScalarT> cross(const TVector3<ScalarT>& v1, const TVector3<ScalarT>& v2) 
	{ 
		return v1.cross(v2); 
	}

	template <typename ScalarT>
	inline ScalarT triple(const TVector3<ScalarT>& v1, const TVector3<ScalarT>& v2, const TVector3<ScalarT>& v3)
	{
		return v1.triple(v2, v3);
	}

	template <typename ScalarT>
	inline TVector3<ScalarT> lerp(const TVector3<ScalarT>& v1, const TVector3<ScalarT>& v2, const ScalarT& t)
	{
		return v1.lerp(v2, t);
	}

}//ns dp

#endif

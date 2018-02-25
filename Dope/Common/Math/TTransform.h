#ifndef DP_TTRANSFORM_H
#define DP_TTRANSFORM_H
#include <Dope/Common/Math/TMatrix3.h>
#include <Dope/Common/Math/TVector3.h>
namespace dp {

    // 三维空间下的仿射变换(Affine Transformation)
	template <typename RealT>
	class TTransform 
    {
		enum 
        { 
			TRANSLATION = 1 << 0,
			ROTATION    = 1 << 1,
            SCALING     = 1 << 2,

			RIGID       = TRANSLATION | ROTATION,  
			LINEAR      = ROTATION | SCALING,
			AFFINE      = TRANSLATION | LINEAR
		};
        using Vec3_t = TVector3<RealT>;
        using Mat3_t = TMatrix3<RealT>;
        using Quat_t = TQuaternion<RealT>;
	public:
		TTransform() {}
		
		template <typename ScalarT2>
		explicit TTransform(const ScalarT2 *m) { setValue(m); }

		explicit TTransform(const Quat_t& q, const Vec3_t& c = Vec3_t::zero()) 
			: m_basis(q),
			  m_origin(c),
			  m_type(RIGID)
		{}

		explicit TTransform(const Mat3_t& b, const Vec3_t& c = Vec3_t::zero(), unsigned int type = AFFINE)
			: m_basis(b),
			  m_origin(c),
			  m_type(type)
		{}

		Vec3_t operator()(const Vec3_t& x) const
		{
			return Vec3_t(m_basis[0].dot(x) + m_origin[0], 
						  m_basis[1].dot(x) + m_origin[1], 
						  m_basis[2].dot(x) + m_origin[2]);
		}
    
		Vec3_t operator*(const Vec3_t& x) const
		{
			return (*this)(x);
		}

		Mat3_t&       getBasis()          { return m_basis; }
		const Mat3_t& getBasis()    const { return m_basis; }

		Vec3_t&         getOrigin()         { return m_origin; }
		const Vec3_t&   getOrigin()   const { return m_origin; }

		Quat_t getRotation() const { return m_basis.getRotation(); }

		template <typename ScalarT2>
		void setValue(const ScalarT2 *m) 
		{
			m_basis.setValue(m);
			m_origin.setValue(&m[12]);
			m_type = AFFINE;
		}

		template <typename ScalarT2>
		void getValue(ScalarT2 *m) const 
		{
			m_basis.getValue(m);
			m_origin.getValue(&m[12]);
			m[15] = ScalarT2(1.0);
		}

		void setOrigin(const Vec3_t& origin) 
		{ 
			m_origin = origin;
			m_type |= TRANSLATION;
		}

		void setBasis(const Mat3_t& basis)
		{ 
			m_basis = basis;
			m_type |= LINEAR;
		}

		void setRotation(const Quat_t& q)
		{
			m_basis.setRotation(q);
			m_type = (m_type & ~LINEAR) | ROTATION;
		}

    	void scale(const Vec3_t& scaling)
		{
			m_basis = m_basis.scaled(scaling);
			m_type |= SCALING;
		}
    
		void setIdentity()
		{
			m_basis.setIdentity();
			m_origin.setValue(RealT(0.0), RealT(0.0), RealT(0.0));
			m_type = 0x0;
		}
		
		bool isIdentity() const { return m_type == 0x0; }
    
		TTransform<RealT>& operator*=(const TTransform<RealT>& t) 
		{
			m_origin += m_basis * t.m_origin;
			m_basis *= t.m_basis;
			m_type |= t.m_type; 
			return *this;
		}

		TTransform inverse() const
		{ 
			Mat3_t inv = (m_type & SCALING) ? 
				                    m_basis.inverse() : 
				                    m_basis.transpose();
			
			return TTransform(inv, inv * -m_origin, m_type);
		}

		TTransform inverseTimes(const TTransform& t) const;  

		TTransform operator*(const TTransform& t) const;

	private:
		Mat3_t m_basis;
		Vec3_t m_origin;
		unsigned int m_type;
	};


	template <typename RealT>
	inline TTransform<RealT> TTransform<RealT>::inverseTimes(const TTransform<RealT>& t) const  
	{
		Vec3_t v = t.getOrigin() - m_origin;
		if (m_type & SCALING) 
		{
			Mat3_t inv = m_basis.inverse();
			return TTransform<RealT>(inv * t.getBasis(), inv * v, m_type | t.m_type);
		}
		else 
		{
			return TTransform<RealT>(m_basis.transposeTimes(t.m_basis), v * m_basis, m_type | t.m_type);
		}
	}

	template <typename RealT>
	inline TTransform<RealT> 
	TTransform<RealT>::operator*(const TTransform<RealT>& t) const
	{
		return TTransform<RealT>(m_basis * t.m_basis, (*this)(t.m_origin), m_type | t.m_type);
	}	
}//ns dp

#endif

#ifndef DP_TMATRIX3_H
#define DP_TMATRIX3_H

#include <Dope/Common/Math/TVector3.h>
#include <Dope/Common/Math/TQuaternion.h>

namespace dp {

    // Row-major 3x3 matrix
	template <typename ScalarT>
	class TMatrix3 
    {
        using TraitsType = ScalarTraits<ScalarT>;
	public:
		TMatrix3() {}
		
		template <typename ScalarT2>
		explicit TMatrix3(const ScalarT2 *m) { setValue(m); }
		
		explicit TMatrix3(const TQuaternion<ScalarT>& q) { setRotation(q); }
		
		template <typename ScalarT2>
		TMatrix3(const ScalarT2& yaw, const ScalarT2& pitch, const ScalarT2& roll)
		{ 
			setEuler(yaw, pitch, roll);
		}
		
		template <typename ScalarT2>
		TMatrix3(const ScalarT2& xx, const ScalarT2& xy, const ScalarT2& xz,
				  const ScalarT2& yx, const ScalarT2& yy, const ScalarT2& yz,
				  const ScalarT2& zx, const ScalarT2& zy, const ScalarT2& zz)
		{ 
			setValue(xx, xy, xz, 
					 yx, yy, yz, 
					 zx, zy, zz);
		}
		
		TVector3<ScalarT>&  operator[](int i)
		{ 
			assert(0 <= i && i < 3);
			return m_el[i]; 
		}
		
		const TVector3<ScalarT>& operator[](int i) const
		{
			assert(0 <= i && i < 3);
			return m_el[i]; 
		}
		
		TMatrix3<ScalarT>& operator*=(const TMatrix3<ScalarT>& m); 

		template <typename ScalarT2>
		void setValue(const ScalarT2 *m)
		{
			m_el[0][0] = ScalarT(m[0]); 
			m_el[1][0] = ScalarT(m[1]); 
			m_el[2][0] = ScalarT(m[2]);
			m_el[0][1] = ScalarT(m[4]); 
			m_el[1][1] = ScalarT(m[5]); 
			m_el[2][1] = ScalarT(m[6]);
			m_el[0][2] = ScalarT(m[8]); 
			m_el[1][2] = ScalarT(m[9]); 
			m_el[2][2] = ScalarT(m[10]);
		}
		template <typename ScalarT2>
		void setValue(const ScalarT2& xx, const ScalarT2& xy, const ScalarT2& xz, 
					  const ScalarT2& yx, const ScalarT2& yy, const ScalarT2& yz, 
					  const ScalarT2& zx, const ScalarT2& zy, const ScalarT2& zz)
		{
			m_el[0][0] = ScalarT(xx); 
			m_el[0][1] = ScalarT(xy); 
			m_el[0][2] = ScalarT(xz);
			m_el[1][0] = ScalarT(yx); 
			m_el[1][1] = ScalarT(yy); 
			m_el[1][2] = ScalarT(yz);
			m_el[2][0] = ScalarT(zx); 
			m_el[2][1] = ScalarT(zy); 
			m_el[2][2] = ScalarT(zz);
		}
		void setRotation(const TQuaternion<ScalarT>& q) 
		{
			ScalarT d = q.lengthSq();
			assert(d != ScalarT(0.0));
			ScalarT s = ScalarT(2.0) / d;
			ScalarT xs = q[0] * s,   ys = q[1] * s,   zs = q[2] * s;
			ScalarT wx = q[3] * xs,  wy = q[3] * ys,  wz = q[3] * zs;
			ScalarT xx = q[0] * xs,  xy = q[0] * ys,  xz = q[0] * zs;
			ScalarT yy = q[1] * ys,  yz = q[1] * zs,  zz = q[2] * zs;
			setValue(ScalarT(1.0) - (yy + zz), xy - wz, xz + wy,
					 xy + wz, ScalarT(1.0) - (xx + zz), yz - wx,
					 xz - wy, yz + wx, ScalarT(1.0) - (xx + yy));
		}
		template <typename ScalarT2> 
		void setEuler(const ScalarT2& yaw, const ScalarT2& pitch, const ScalarT2& roll) 
		{
			// yaw is CW around y-axis, pitch is CCW around x-axis, and roll is CW around z-axis

			ScalarT cy(TraitsType::cos(yaw)); 
			ScalarT sy(TraitsType::sin(yaw)); 
			ScalarT cp(TraitsType::cos(pitch)); 
			ScalarT sp(TraitsType::sin(pitch)); 
			ScalarT cr(TraitsType::cos(roll));
			ScalarT sr(TraitsType::sin(roll));
			ScalarT cc = cy * cr; 
			ScalarT cs = cy * sr; 
			ScalarT sc = sy * cr; 
			ScalarT ss = sy * sr;
			setValue(cc + sp * ss, cs - sp * sc, -sy * cp,
					     -cp * sr,      cp * cr,      -sp,
					 sc - sp * cs, ss + sp * cc,  cy * cp);
					    
		}
		void setIdentity()
		{ 
			setValue(ScalarT(1.0), ScalarT(0.0), ScalarT(0.0), 
					 ScalarT(0.0), ScalarT(1.0), ScalarT(0.0), 
					 ScalarT(0.0), ScalarT(0.0), ScalarT(1.0)); 
		}
		template <typename ScalarT2>
		void getValue(ScalarT2 *m) const 
		{
			m[0]  = ScalarT2(m_el[0][0]); 
			m[1]  = ScalarT2(m_el[1][0]);
			m[2]  = ScalarT2(m_el[2][0]);
			m[3]  = ScalarT2(0.0); 
			m[4]  = ScalarT2(m_el[0][1]);
			m[5]  = ScalarT2(m_el[1][1]);
			m[6]  = ScalarT2(m_el[2][1]);
			m[7]  = ScalarT2(0.0); 
			m[8]  = ScalarT2(m_el[0][2]); 
			m[9]  = ScalarT2(m_el[1][2]);
			m[10] = ScalarT2(m_el[2][2]);
			m[11] = ScalarT2(0.0); 
		}
		void getRotation(TQuaternion<ScalarT>& q) const
		{
			ScalarT trace = m_el[0][0] + m_el[1][1] + m_el[2][2];
			
			if (trace > ScalarT(0.0)) 
			{
				ScalarT s = TraitsType::sqrt(trace + ScalarT(1.0));
				q[3] = s * ScalarT(0.5);
				s = ScalarT(0.5) / s;
				
				q[0] = (m_el[2][1] - m_el[1][2]) * s;
				q[1] = (m_el[0][2] - m_el[2][0]) * s;
				q[2] = (m_el[1][0] - m_el[0][1]) * s;
			} 
			else 
			{
				int i = m_el[0][0] < m_el[1][1] ? 
					(m_el[1][1] < m_el[2][2] ? 2 : 1) :
					(m_el[0][0] < m_el[2][2] ? 2 : 0); 
				int j = (i + 1) % 3;  
				int k = (i + 2) % 3;
				
				ScalarT s = TraitsType::sqrt(m_el[i][i] - m_el[j][j] - m_el[k][k] + ScalarT(1.0));
				q[i] = s * ScalarT(0.5);
				s = ScalarT(0.5) / s;
				
				q[3] = (m_el[k][j] - m_el[j][k]) * s;
				q[j] = (m_el[j][i] + m_el[i][j]) * s;
				q[k] = (m_el[k][i] + m_el[i][k]) * s;
			}
		}
		template <typename ScalarT2>
		void getEuler(ScalarT2& yaw, ScalarT2& pitch, ScalarT2& roll) const
		{
			if (m_el[1][2] < ScalarT(1))
			{
				if (m_el[1][2] > -ScalarT(1))
				{
					yaw   = ScalarT2(TraitsType::atan2(-m_el[0][2], m_el[2][2]));
					pitch = ScalarT2(asin(-m_el[1][2]));
					roll  = ScalarT2(TraitsType::atan2(-m_el[1][0], m_el[1][1]));
				}
				else 
				{
					yaw   = ScalarT2(TraitsType::atan2(-m_el[0][1], m_el[0][0]));
					pitch = ScalarTraits<ScalarT2>::pi() * ScalarT2(0.5); 
					roll  = ScalarT2(0.0);
				}
			}
			else
			{
				yaw   =  ScalarT2(TraitsType::atan2(m_el[0][1], m_el[0][0]));
				pitch = -ScalarTraits<ScalarT2>::pi() * ScalarT2(0.5); 
				roll  =  ScalarT2(0.0);
			}
		}

		TVector3<ScalarT> getScaling() const
		{
			return TVector3<ScalarT>(m_el[0][0] * m_el[0][0] + m_el[1][0] * m_el[1][0] + m_el[2][0] * m_el[2][0],
								     m_el[0][1] * m_el[0][1] + m_el[1][1] * m_el[1][1] + m_el[2][1] * m_el[2][1],
								     m_el[0][2] * m_el[0][2] + m_el[1][2] * m_el[1][2] + m_el[2][2] * m_el[2][2]);
		}
		
		
		TMatrix3<ScalarT> scaled(const TVector3<ScalarT>& s) const
		{
			return TMatrix3<ScalarT>(m_el[0][0] * s[0], m_el[0][1] * s[1], m_el[0][2] * s[2],
									 m_el[1][0] * s[0], m_el[1][1] * s[1], m_el[1][2] * s[2],
									 m_el[2][0] * s[0], m_el[2][1] * s[1], m_el[2][2] * s[2]);
		}

		ScalarT           determinant() const;
		TMatrix3<ScalarT> adjoint()     const;
		TMatrix3<ScalarT> getAbsolute() const;
		TMatrix3<ScalarT> transpose()   const;
		TMatrix3<ScalarT> inverse()     const; 
		
		TMatrix3<ScalarT> transposeTimes(const TMatrix3<ScalarT>& m) const;
		TMatrix3<ScalarT> timesTranspose(const TMatrix3<ScalarT>& m) const;
		
		ScalarT tdot(int c, const TVector3<ScalarT>& v) const 
		{
			return m_el[0][c] * v[0] + m_el[1][c] * v[1] + m_el[2][c] * v[2];
		}
		
	protected:
		ScalarT cofac(int r1, int c1, int r2, int c2) const 
		{
			return m_el[r1][c1] * m_el[r2][c2] - m_el[r1][c2] * m_el[r2][c1];
		}

		TVector3<ScalarT> m_el[3];
	};

	template <typename ScalarT>
	inline TMatrix3<ScalarT>& 
	TMatrix3<ScalarT>::operator*=(const TMatrix3<ScalarT>& m)
	{
		setValue(m.tdot(0, m_el[0]), m.tdot(1, m_el[0]), m.tdot(2, m_el[0]),
				 m.tdot(0, m_el[1]), m.tdot(1, m_el[1]), m.tdot(2, m_el[1]),
				 m.tdot(0, m_el[2]), m.tdot(1, m_el[2]), m.tdot(2, m_el[2]));
		return *this;
	}
	
	template <typename ScalarT>
	inline ScalarT 
	TMatrix3<ScalarT>::determinant() const
	{ 
		return triple((*this)[0], (*this)[1], (*this)[2]);
	}
	

	template <typename ScalarT>
	inline TMatrix3<ScalarT> 
	TMatrix3<ScalarT>::getAbsolute() const
	{
		return TMatrix3<ScalarT>(
			TraitsType::abs(m_el[0][0]), TraitsType::abs(m_el[0][1]), TraitsType::abs(m_el[0][2]),
			TraitsType::abs(m_el[1][0]), TraitsType::abs(m_el[1][1]), TraitsType::abs(m_el[1][2]),
			TraitsType::abs(m_el[2][0]), TraitsType::abs(m_el[2][1]), TraitsType::abs(m_el[2][2]));
	}

	template <typename ScalarT>
	inline TMatrix3<ScalarT> 
	TMatrix3<ScalarT>::transpose() const 
	{
		return TMatrix3<ScalarT>(m_el[0][0], m_el[1][0], m_el[2][0],
								 m_el[0][1], m_el[1][1], m_el[2][1],
								 m_el[0][2], m_el[1][2], m_el[2][2]);
	}
	
	template <typename ScalarT>
	inline TMatrix3<ScalarT> 
	TMatrix3<ScalarT>::adjoint() const 
	{
		return TMatrix3<ScalarT>(cofac(1, 1, 2, 2), cofac(0, 2, 2, 1), cofac(0, 1, 1, 2),
								 cofac(1, 2, 2, 0), cofac(0, 0, 2, 2), cofac(0, 2, 1, 0),
								 cofac(1, 0, 2, 1), cofac(0, 1, 2, 0), cofac(0, 0, 1, 1));
	}
	
	template <typename ScalarT>
	inline TMatrix3<ScalarT> 
	TMatrix3<ScalarT>::inverse() const
	{
		TVector3<ScalarT> co(cofac(1, 1, 2, 2), cofac(1, 2, 2, 0), cofac(1, 0, 2, 1));
		ScalarT det = (*this)[0].dot(co);
		assert(det != ScalarT(0.0));
		ScalarT s = ScalarT(1.0) / det;
		return TMatrix3<ScalarT>(co[0] * s, cofac(0, 2, 2, 1) * s, cofac(0, 1, 1, 2) * s,
								 co[1] * s, cofac(0, 0, 2, 2) * s, cofac(0, 2, 1, 0) * s,
								 co[2] * s, cofac(0, 1, 2, 0) * s, cofac(0, 0, 1, 1) * s);
	}
	
	template <typename ScalarT>
	inline TMatrix3<ScalarT> 
	TMatrix3<ScalarT>::transposeTimes(const TMatrix3<ScalarT>& m) const
	{
		return TMatrix3<ScalarT>(
			m_el[0][0] * m[0][0] + m_el[1][0] * m[1][0] + m_el[2][0] * m[2][0],
			m_el[0][0] * m[0][1] + m_el[1][0] * m[1][1] + m_el[2][0] * m[2][1],
			m_el[0][0] * m[0][2] + m_el[1][0] * m[1][2] + m_el[2][0] * m[2][2],
			m_el[0][1] * m[0][0] + m_el[1][1] * m[1][0] + m_el[2][1] * m[2][0],
			m_el[0][1] * m[0][1] + m_el[1][1] * m[1][1] + m_el[2][1] * m[2][1],
			m_el[0][1] * m[0][2] + m_el[1][1] * m[1][2] + m_el[2][1] * m[2][2],
			m_el[0][2] * m[0][0] + m_el[1][2] * m[1][0] + m_el[2][2] * m[2][0],
			m_el[0][2] * m[0][1] + m_el[1][2] * m[1][1] + m_el[2][2] * m[2][1],
			m_el[0][2] * m[0][2] + m_el[1][2] * m[1][2] + m_el[2][2] * m[2][2]);
	}
	
	template <typename ScalarT>
	inline TMatrix3<ScalarT> 
	TMatrix3<ScalarT>::timesTranspose(const TMatrix3<ScalarT>& m) const
	{
		return TMatrix3<ScalarT>(
			m_el[0].dot(m[0]), m_el[0].dot(m[1]), m_el[0].dot(m[2]),
			m_el[1].dot(m[0]), m_el[1].dot(m[1]), m_el[1].dot(m[2]),
			m_el[2].dot(m[0]), m_el[2].dot(m[1]), m_el[2].dot(m[2]));
		
	}

	template <typename ScalarT>
	inline TVector3<ScalarT> 
	operator*(const TMatrix3<ScalarT>& m, const TVector3<ScalarT>& v) 
	{
		return TVector3<ScalarT>(m[0].dot(v), m[1].dot(v), m[2].dot(v));
	}
	

	template <typename ScalarT>
	inline TVector3<ScalarT>
	operator*(const TVector3<ScalarT>& v, const TMatrix3<ScalarT>& m)
	{
		return TVector3<ScalarT>(m.tdot(0, v), m.tdot(1, v), m.tdot(2, v));
	}

	template <typename ScalarT>
	inline TMatrix3<ScalarT> 
	operator*(const TMatrix3<ScalarT>& m1, const TMatrix3<ScalarT>& m2)
	{
		return TMatrix3<ScalarT>(
			m2.tdot(0, m1[0]), m2.tdot(1, m1[0]), m2.tdot(2, m1[0]),
			m2.tdot(0, m1[1]), m2.tdot(1, m1[1]), m2.tdot(2, m1[1]),
			m2.tdot(0, m1[2]), m2.tdot(1, m1[2]), m2.tdot(2, m1[2]));
	}
}//ns dp
#endif

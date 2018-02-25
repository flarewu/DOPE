#ifndef DP_TTUPLE3_H
#define DP_TTUPLE3_H
namespace dp {

	template <typename ScalarT>
	class TTuple3 
    {
	public:
		TTuple3() {}
		
		template <typename ScalarT2>
		explicit TTuple3(const ScalarT2 *v) 
		{ 
			this->setValue(v);
		}
		
		template <typename ScalarT2>
		TTuple3(const ScalarT2& x, const ScalarT2& y, const ScalarT2& z) 
		{ 
			this->setValue(x, y, z); 
		}
		
		template <typename ScalarT2>
		TTuple3(const TTuple3<ScalarT2>& t) 
		{ 
			*this = t; 
		}
		
		template <typename ScalarT2>
		TTuple3<ScalarT>& operator = (const TTuple3<ScalarT2>& rhs) 
		{ 
			m_scalars[0] = ScalarT(rhs[0]); 
			m_scalars[1] = ScalarT(rhs[1]); 
			m_scalars[2] = ScalarT(rhs[2]);
			return *this;
		}
		
		operator       ScalarT *()       { return m_scalars; }
		operator const ScalarT *() const { return m_scalars; }

		ScalarT&       operator[](size_t i)       { return m_scalars[i]; }      
		const ScalarT& operator[](size_t i) const { return m_scalars[i]; }

		ScalarT&       x()       { return m_scalars[0]; }
		const ScalarT& x() const { return m_scalars[0]; }
		
		ScalarT&       y()       { return m_scalars[1]; }
		const ScalarT& y() const { return m_scalars[1]; }
		
		ScalarT&       z()       { return m_scalars[2]; }
		const ScalarT& z() const { return m_scalars[2]; }

		template <typename ScalarT2>
		void setValue(const ScalarT2* scalars) 
		{
			m_scalars[0] = ScalarT(scalars[0]); 
			m_scalars[1] = ScalarT(scalars[1]); 
			m_scalars[2] = ScalarT(scalars[2]);
		}

		template <typename ScalarT2>
		void setValue(const ScalarT2& x, const ScalarT2& y, const ScalarT2& z)
		{
			m_scalars[0] = ScalarT(x); 
			m_scalars[1] = ScalarT(y); 
			m_scalars[2] = ScalarT(z);
		}

		template <typename ScalarT2>
		void getValue(ScalarT2 *scalars) const 
		{
			scalars[0] = ScalarT2(m_scalars[0]);
			scalars[1] = ScalarT2(m_scalars[1]);
			scalars[2] = ScalarT2(m_scalars[2]);
		}
	protected:
		ScalarT m_scalars[3];                            
	};
}//ns dp

#endif

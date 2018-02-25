#ifndef DP_TTUPLE4_H
#define DP_TTUPLE4_H

namespace dp {

	template <typename ScalarT>
	class TTuple4 
    {
	public:
		TTuple4() {}
		
		template <typename ScalarT2>
		explicit TTuple4(const ScalarT2* scalars) 
		{ 
			this->setValue(scalars);
		}
		
		template <typename ScalarT2>
		TTuple4(const ScalarT2& x, const ScalarT2& y, const ScalarT2& z, const ScalarT2& w) 
		{ 
			this->setValue(x, y, z, w); 
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

		ScalarT&       w()       { return m_scalars[3]; }
		const ScalarT& w() const { return m_scalars[3]; }
    
		template <typename ScalarT2>
		void setValue(const ScalarT2* scalars) 
		{
			m_scalars[0] = ScalarT(scalars[0]); 
			m_scalars[1] = ScalarT(scalars[1]); 
			m_scalars[2] = ScalarT(scalars[2]);
			m_scalars[3] = ScalarT(scalars[3]);
		}

		template <typename ScalarT2>
		void setValue(const ScalarT2& x, const ScalarT2& y, const ScalarT2& z, const ScalarT2& w)
		{
			m_scalars[0] = ScalarT(x); 
			m_scalars[1] = ScalarT(y); 
			m_scalars[2] = ScalarT(z);
			m_scalars[3] = ScalarT(w);
		}

		template <typename ScalarT2>
		void getValue(ScalarT2* scalarsOut) const 
		{
            scalarsOut[0] = ScalarT2(m_scalars[0]);
            scalarsOut[1] = ScalarT2(m_scalars[1]);
            scalarsOut[2] = ScalarT2(m_scalars[2]);
            scalarsOut[3] = ScalarT2(m_scalars[3]);
		}

	protected:
		ScalarT m_scalars[4];
	};

}//ns dp

#endif

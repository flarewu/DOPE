#ifndef DP_TRACED_FLOAT_H
#define DP_TRACED_FLOAT_H

#include <Dope/Common/Math/ScalarTraits.h>
namespace dp {
    template <typename FloatT>
    class TracedFloat
    {
    public:
        TracedFloat() {}
        TracedFloat(FloatT value, FloatT error = FloatT(0.0))
            : m_value(value)
            , m_error(value == FloatT(0.0) ? FloatT(0.0) : error)
        {}

        FloatT getValue() const { return m_value; }
        FloatT getError() const { return m_error; }

        void setError(FloatT error) { m_error = error; }

        operator FloatT() const { return m_value; }

        TracedFloat operator-() const
        {
            return TracedFloat<FloatT>(-m_value, m_error);
        }

        TracedFloat& operator=(FloatT value)
        {
            m_value = value;
            m_error = FloatT(0.0);
            return *this;
        }

        TracedFloat& operator+=(const TracedFloat& x)
        {
            *this = *this + x;
            return *this;
        }

        TracedFloat& operator-=(const TracedFloat& x)
        {
            *this = *this - x;
            return *this;
        }

        TracedFloat& operator*=(const TracedFloat& x)
        {
            *this = *this * x;
            return *this;
        }

        TracedFloat& operator/=(const TracedFloat& x)
        {
            *this = *this / x;
            return *this;
        }

    private:
        FloatT m_value; // the value of the scalar
        FloatT m_error; // the relative rounding error
    };
    // `m_error' times the machine epsilon (FLT_EPSILON for floats, DBL_EPSILON for doubles) 
    // gives an estimated upper bound for the rounding error in `m_value'.
    // The number of dirty bits in the mantissa is `log(m_error) / log(2)', so if `m_error'
    // reaches 2^24 = 16777216 then we used up all bits in the mantissa of a float. 
    // Note that the error is a rough upper bound. In reality i386 platforms may 
    // evaluate compound expressions in higher precision, so the given machine epsilon
    // is usually too large in most cases.   
    template <class FloatT>
    inline TracedFloat<FloatT> operator+(const TracedFloat<FloatT>& x,const TracedFloat<FloatT>& y)
    {
        FloatT value = x.getValue() + y.getValue();
        return TracedFloat<FloatT>(
            value,
            (ScalarTraits<FloatT>::abs(x.getValue()) * x.getError() + ScalarTraits<FloatT>::abs(y.getValue()) * y.getError()) / ScalarTraits<FloatT>::abs(value) + FloatT(1.0));
    }
    template <typename FloatT>
    inline TracedFloat<FloatT> operator-(const TracedFloat<FloatT>& x,const TracedFloat<FloatT>& y)
    {
        FloatT value = x.getValue() - y.getValue();
        return TracedFloat<FloatT>(
            value,
            (ScalarTraits<FloatT>::abs(x.getValue()) * x.getError() + ScalarTraits<FloatT>::abs(y.getValue()) * y.getError()) / ScalarTraits<FloatT>::abs(value) + FloatT(1.0));
    }
    template <typename FloatT>
    inline TracedFloat<FloatT> operator*(const TracedFloat<FloatT>& x,const TracedFloat<FloatT>& y)
    {
        return TracedFloat<FloatT>(x.getValue() * y.getValue(),x.getError() + y.getError() + FloatT(1.0));
    }
    template <typename FloatT>
    inline TracedFloat<FloatT> operator/(const TracedFloat<FloatT>& x, const TracedFloat<FloatT>& y)
    {
        return TracedFloat<FloatT>(x.getValue() / y.getValue(), x.getError() + y.getError() + FloatT(1.0));
    }
    template <typename FloatT>
    inline TracedFloat<FloatT> sqrt(const TracedFloat<FloatT>& x)
    {
        return TracedFloat<FloatT>(sqrt(x.getValue()), FloatT(0.5) * x.getError() + FloatT(1.0));
    }
    template <typename FloatT>
    inline TracedFloat<FloatT> abs(const TracedFloat<FloatT>& x)
    {
        return TracedFloat<FloatT>(ScalarTraits<FloatT>::abs(x.getValue()), x.getError());
    }
    template <typename FloatT>
    inline TracedFloat<FloatT> cos(const TracedFloat<FloatT>& x)
    {
        return TracedFloat<FloatT>(ScalarTraits<FloatT>::cos(x.getValue()), x.getError() + FloatT(1.0));
    }
    template <typename FloatT>
    inline TracedFloat<FloatT> sin(const TracedFloat<FloatT>& x)
    {
        return TracedFloat<FloatT>(ScalarTraits<FloatT>::sin(x.getValue()), x.getError() + FloatT(1.0));
    }
    template <typename FloatT>
    inline TracedFloat<FloatT> tan(const TracedFloat<FloatT>& x)
    {
        FloatT value = ScalarTraits<FloatT>::tan(x.getValue());
        return TracedFloat<FloatT>(value, (FloatT(1.0) + value * value) * x.getError() + FloatT(1.0));
    }
    template <typename FloatT>
    inline TracedFloat<FloatT> acos(const TracedFloat<FloatT>& x)
    {
        return TracedFloat<FloatT>(ScalarTraits<FloatT>::acos(x.getValue()), x.getError() + FloatT(1.0));
    }
    template <typename FloatT>
    inline TracedFloat<FloatT> asin(const TracedFloat<FloatT>& x)
    {
        return TracedFloat<FloatT>(ScalarTraits<FloatT>::asin(x.getValue()), x.getError() + FloatT(1.0));
    }
    template <typename FloatT>
    inline TracedFloat<FloatT> atan(const TracedFloat<FloatT>& x)
    {
        return TracedFloat<FloatT>(ScalarTraits<FloatT>::atan(x.getValue()), x.getError() + FloatT(1.0));
    }
    template <typename FloatT>
    inline TracedFloat<FloatT> atan2(const TracedFloat<FloatT>& x, const TracedFloat<FloatT>& y)
    {
        return TracedFloat<FloatT>(ScalarTraits<FloatT>::atan2(x.getValue(), y.getValue()), x.getError() + y.getError() + FloatT(1.0));
    }
    template <typename FloatT>
    inline TracedFloat<FloatT> exp(const TracedFloat<FloatT>& x)
    {
        FloatT value = ScalarTraits<FloatT>::exp(x.getValue());
        return TracedFloat<FloatT>(value, value * x.getError() + FloatT(1.0));
    }
    template <typename FloatT>
    inline TracedFloat<FloatT> log(const TracedFloat<FloatT>& x)
    {
        return TracedFloat<FloatT>(ScalarTraits<FloatT>::log(x.getValue()), x.getError() / x.getValue() + FloatT(1.0));
    }
    template <typename FloatT>
    inline TracedFloat<FloatT> pow(const TracedFloat<FloatT>& x, const TracedFloat<FloatT>& y)
    {
        assert(x.getValue() >= FloatT(0.0));
        FloatT value = ScalarTraits<FloatT>::pow(x.getValue(), y.getValue());
        return TracedFloat<FloatT>(value,
            ScalarTraits<FloatT>::abs(y.getValue()) * x.getError() +
            ScalarTraits<FloatT>::abs(value * ScalarTraits<FloatT>::log(x.getValue())) * y.getError() + FloatT(1.0));
    }
    template <typename FloatT>
    struct ScalarTraits<TracedFloat<FloatT>> 
    {
        static TracedFloat<FloatT> pi2()     { return TracedFloat<FloatT>(ScalarTraits<FloatT>::pi2());     }
        static TracedFloat<FloatT> epsilon() { return TracedFloat<FloatT>(ScalarTraits<FloatT>::epsilon()); }
        static TracedFloat<FloatT> max()     { return TracedFloat<FloatT>(ScalarTraits<FloatT>::max());     }
        static TracedFloat<FloatT> random()  { return TracedFloat<FloatT>(ScalarTraits<FloatT>::random());  }

        static TracedFloat<FloatT> sqrt(TracedFloat<FloatT> x) { return ::sqrt(x); }
        static TracedFloat<FloatT> abs (TracedFloat<FloatT> x) { return ::abs(x); }

        static TracedFloat<FloatT> cos(TracedFloat<FloatT> x) { return ::cos(x); }
        static TracedFloat<FloatT> sin(TracedFloat<FloatT> x) { return ::sin(x); }
        static TracedFloat<FloatT> tan(TracedFloat<FloatT> x) { return ::tan(x); }

        static TracedFloat<FloatT> acos(TracedFloat<FloatT> x) { return ::acos(x); }
        static TracedFloat<FloatT> asin(TracedFloat<FloatT> x) { return ::asin(x); }
        static TracedFloat<FloatT> atan(TracedFloat<FloatT> x) { return ::atan(x); }
        static TracedFloat<FloatT> atan2(TracedFloat<FloatT> x, TracedFloat<FloatT> y) { return ::atan2(x, y); }

        static TracedFloat<FloatT> exp(TracedFloat<FloatT> x) { return ::exp(x); }
        static TracedFloat<FloatT> log(TracedFloat<FloatT> x) { return ::log(x); }
        static TracedFloat<FloatT> pow(TracedFloat<FloatT> x, TracedFloat<FloatT> y) { return ::pow(x, y); }
    };
}//ns dp
#endif //DP_TRACED_FLOAT_H






#ifndef TEMPLATE_TYPES_HPP
#define TEMPLATE_TYPES_HPP

template<typename T> class Scale
{
    public:
        Scale(T min, T max) : mMin(min), mMax(max){}
        template<typename scale> T operator()(scale scalingval) 
        {
            return mMin + (mMax * scalingval);
        }
    private:
        T mMin;
        T mMax;        
};


#endif
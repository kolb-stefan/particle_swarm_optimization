/**
Copyright (C) 2008-2013 Stefan Kolb.

This file is part of the program pso (particle swarm optimization).

The program pso is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public
License as published by the Free Software Foundation, either
version 2 of the License, or (at your option) any later version.

The program pso is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with pso. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef VECTORN_H
#define VECTORN_H

#include<ostream>
#include<sstream>
#include<string>
#include<vector>
#include<utility>
#include<cmath>

#include "exception.h"
#include "error.h"

template<typename T>
class VectorN
{
    public:
        VectorN( size_t N_ );
        VectorN( const VectorN &vm );
        ~VectorN();
        VectorN &operator = ( const VectorN &vm );
        VectorN &operator *= ( const T &vm );
        VectorN &operator += ( const VectorN &vm );
        VectorN &operator -= ( const VectorN &vm );

        bool operator == ( const VectorN &vm ) const;
        bool compare_equal( const VectorN &a , const VectorN &b, double epsilon = 1e-30 ) const;

        VectorN operator + ( const VectorN &vm ) const;
        T operator * ( const VectorN &vm ) const;
        VectorN operator * ( const T &vm ) const;
        VectorN operator - ( const VectorN &vm ) const;

        T operator []( int i ) const;
        T &operator []( int i );

        void add( const VectorN &vm, VectorN &ret ) const;
        void sub( const VectorN &vm, VectorN &ret ) const;
        void mul( const T &vm, VectorN &ret ) const;

        T dotProduct( const VectorN &vm ) const;
        void dotProduct( const VectorN &vm, T &ret ) const;

        T length() const;
        T abs() const;

        VectorN<T> getNormalized( double epsilon = 1e-30 ) const;
        void getNormalized( VectorN<T> &result, double epsilon = 1e-30 ) const;
        void normalize( double epsilon = 1e-30 );

        bool isZero( double epsilon = 1e-30 ) const;

        void checkDimension( const VectorN<T> &var ) const;

        void set( int i, T  t );
        void setAll( T t );
        T get( int i ) const;

        size_t size() const;
        void resize( size_t dim );

        std::string toString( unsigned int digits = 3 ) const;

        friend std::ostream &operator << ( std::ostream &os, VectorN &vm )
        {
            for( int i = 0; i < vm.N ; i++ )
            {
                os << vm.data_container[i] << std::endl;
            }

            return os;
        }

    protected:
        size_t N;
        std::vector<T> data_container;
};

template<typename T>
VectorN<T>::VectorN( size_t N_ ) : N( N_ ), data_container( N )
{

}

template<typename T>
VectorN<T>::VectorN( const VectorN &vm ) : N( vm.N ), data_container( vm.data_container )
{

}

template<typename T>
VectorN<T>::~VectorN()
{

}

template<typename T>
void VectorN<T>::checkDimension( const VectorN<T> &var ) const
{
    if( N != var.N )
    {
        throw RuntimeError( "Wrong Dimension N for Operation" );
    }
}

template<typename T>
VectorN<T> &VectorN<T>::operator = ( const VectorN<T> &vm )
{
    N = vm.N;
    data_container = vm.data_container;
    return *this;
}

template<typename T>
VectorN<T> &VectorN<T>::operator += ( const VectorN<T> &vm )
{
    checkDimension( vm );

    for( int i = 0; i < N ; i++ )
    {
        data_container[i] += vm.data_container[i];
    }

    return *this;
}

template<typename T>
VectorN<T> &VectorN<T>::operator *= ( const T &vm )
{
    checkDimension( vm );

    for( int i = 0; i < N ; i++ )
    {
        data_container[i] *= vm;
    }

    return *this;
}

template<typename T>
VectorN<T> &VectorN<T>::operator -= ( const VectorN<T> &vm )
{
    checkDimension( vm );

    for( int i = 0; i < N ; i++ )
    {
        data_container[i] -= vm.data_container[i];
    }

    return *this;
}

template<typename T>
void VectorN<T>::add( const VectorN<T> &vm, VectorN<T> &ret ) const
{
    checkDimension( vm );

    if( N != ret.N ) {ret.resize( N );}

    for( int i = 0; i < N ; i++ )
    {
        ret.data_container[i] = data_container[i] + vm.data_container[i];
    }
}


template<typename T>
VectorN<T> VectorN<T>::operator + ( const VectorN<T> &vm ) const
{
    checkDimension( vm );
    VectorN<T> ret( N );

    for( unsigned int i = 0; i < N ; i++ )
    {
        ret.data_container[i] = data_container[i] + vm.data_container[i];
    }

    return ret;
}

template<typename T>
void VectorN<T>::mul( const T &vm, VectorN<T> &ret ) const
{
    checkDimension( vm );

    if( N != ret.N ) {ret.resize( N );}

    for( int i = 0; i < N ; i++ )
    {
        ret.data_container[i] = data_container[i] * vm;
    }
}

template<typename T>
T VectorN<T>::operator * ( const VectorN<T> &vm ) const
{
    checkDimension( vm );
    T ret = data_container[0] * vm.data_container[0];

    for( int i = 1; i < N ; i++ )
    {
        ret += data_container[i] * vm.data_container[i];
    }

    return ret;
}

template<typename T>
VectorN<T> VectorN<T>::operator * ( const T &vm ) const
{
    VectorN<T> ret( N );

    for( unsigned int i = 0; i < N ; i++ )
    {
        ret.data_container[i] = data_container[i] * vm;
    }

    return ret;
}

template<typename T>
void VectorN<T>::sub( const VectorN &vm, VectorN &ret ) const
{
    checkDimension( vm );

    if( N != ret.N ) {ret.resize( N );}

    for( int i = 0; i < N ; i++ )
    {
        ret.data_container[i] = data_container[i] - vm.data_container[i];
    }
}

template<typename T>
VectorN<T> VectorN<T>::operator - ( const VectorN<T> &vm ) const
{
    checkDimension( vm );
    VectorN<T> ret( N );

    for( unsigned int i = 0; i < N ; i++ )
    {
        ret.data_container[i] = data_container[i] - vm.data_container[i];
    }

    return ret;
}

template<typename T>
void VectorN<T>::set( int i, T  t )
{
    if( ( i >= 0 ) && ( i < N ) )
    {
        data_container[i] = t;
    }
    else
    {
        throw RuntimeError( "Access out of range!" );
    }
}

template<typename T>
void VectorN<T>::setAll( T t )
{
    for( unsigned int i = 0; i < N ; i++ )
    {
        data_container[i] = t;
    }
}

template<typename T>
T VectorN<T>::get( int i ) const
{
    if( ( i >= 0 ) && ( i < N ) )
    {
        return data_container[i];
    }
    else
    {
        throw RuntimeError( "Access out of range!" );
    }
}

template<typename T>
T VectorN<T>::operator []( int i ) const
{
    return data_container[i];
}

template<typename T>
T &VectorN<T>::operator []( int i )
{
    return data_container[i];
}


template<typename T>
size_t VectorN<T>::size() const
{
    return N;
}

template<typename T>
void VectorN<T>::resize( size_t dim )
{
    N = dim;
    data_container.resize( N );
}

template<typename T>
T VectorN<T>::dotProduct( const VectorN &vm ) const
{
    checkDimension( vm );
    T ret = data_container[0] * vm.data_container[0];

    for( int i = 1; i < N ; i++ )
    {
        ret += data_container[i] * vm.data_container[i];
    }

    return ret;
}

template<typename T>
void VectorN<T>::dotProduct( const VectorN &vm, T &ret ) const
{
    checkDimension( vm );

    if( N != ret.N ) {ret.resize( N );}

    ret = data_container[0] * vm.data_container[0];

    for( int i = 1; i < N ; i++ )
    {
        ret += data_container[i] * vm.data_container[i];
    }
}

template<typename T>
T VectorN<T>::length() const
{
    T tmp = data_container[0] * data_container[0];

    for( int i = 1; i < N; i++ )
    {
        tmp += ( data_container[i] * data_container[i] );
    }

    return sqrt( tmp );
}

template<typename T>
T VectorN<T>::abs() const
{
    return length();
}

template<typename T>
VectorN<T> VectorN<T>::getNormalized( double epsilon ) const
{
    VectorN<T> ret;
    getNormalized( ret, epsilon );
    return ret;
}

template<typename T>
void VectorN<T>::getNormalized( VectorN<T> &result, double epsilon ) const
{
    if( N != result.N ) {result.resize( N );}

    T l = length();

    if( l <= epsilon )
    {
        throw Exception( "Length of Vector is Zero" );
    }

    for( int i = 0; i < N; i++ )
    {
        result.data_container[i] = result.data_container[i] / l;
    }
}

template<typename T>
void VectorN<T>::normalize( double epsilon )
{
    getNormalized( *this, epsilon );
}

template<typename T>
bool VectorN<T>::isZero( double epsilon ) const
{
    bool zero = true;

    for( unsigned int i = 0; i < data_container.size(); i++ )
    {
        if( data_container[i] > epsilon )
        {
            zero = false;
        }
    }

    return zero;
}

template<typename T>
bool VectorN<T>::operator == ( const VectorN<T> &vm ) const
{
    return compare_equal( *this, vm );
}

template<typename T>
bool VectorN<T>::compare_equal( const VectorN<T> &a, const VectorN<T> &b, double epsilon ) const
{
    if( a.N != b.N )
    {
        throw RuntimeError( "Wrong Dimension N for Operation" );
    }

    for( unsigned int i = 0 ; i < N; i++ )
    {
        if( ( a.data_container[i] - b.data_container[i] ) > epsilon )
        {
            return false;
        }
    }

    return true;
}

template<typename T>
std::string VectorN<T>::toString( unsigned int digits ) const
{
    std::stringstream ss;
    ss.precision( digits );

    for( unsigned int i = 0; i < data_container.size() - 1; i++ )
    {
        ss << data_container[i] << ",";
    }

    if( data_container.size() > 0 )
    {
        ss << data_container[data_container.size() - 1];
    }

    return ss.str();
}

#endif

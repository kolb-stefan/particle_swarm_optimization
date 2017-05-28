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

#ifndef VECTOR3_H
#define VECTOR3_H

#include <sstream>
#include <string>

#define N 3

template<typename T>
class Vector
{
    public:
        Vector();
        Vector( T x, T y , T z );

        Vector( const Vector<T> &vm );
        ~Vector();

        Vector<T> &operator = ( const Vector<T> &vm );
        Vector<T> &operator *= ( const T &vm );
        Vector<T> &operator += ( const Vector<T> &vm );
        Vector<T> &operator -= ( const Vector<T> &vm );

        bool operator == ( const Vector<T> &vm ) const;
        bool compare_equal( const Vector<T> &a, const Vector<T> &b, double epsilon = 1e-30 ) const;

        Vector<T> operator + ( const Vector<T> &vm ) const;
        T operator * ( const Vector<T> &vm ) const; //dotProduct
        Vector<T> operator * ( const T &vm ) const;
        Vector<T> operator / ( const T &vm ) const;
        Vector<T> operator - ( const Vector<T> &vm ) const;

        T operator []( int i ) const;
        T &operator []( int i );

        void add( const Vector<T> &vm, Vector<T> &ret ) const;
        void sub( const Vector<T> &vm, Vector<T> &ret ) const;
        void mul( const T &vm, Vector<T> &ret ) const;

        T dotProduct( const Vector<T> &vm ) const;
        void dotProduct( const Vector<T> &vm, T &ret ) const;

        void crossProduct( const Vector<T> &vm, Vector<T> &ret ) const;
        Vector<T> crossProduct( const Vector<T> &vm ) const;

        T length() const;
        T abs() const;

        /*
            Rotation with euler angles
        */
        Vector<T> rotate( double alpha, double beta, double gamma ) const;
        void rotate( double alpha, double beta, double gamma, Vector<T> &ret ) const;
        void rot( double alpha, double beta, double gamma );

        Vector<T> getNormalized( double epsilon = 1e-30 ) const;
        void getNormalized( Vector<T> &result, double epsilon = 1e-30 ) const;
        void normalize( double epsilon = 1e-30 );

        bool isZero( double epsilon = 1e-30 ) const;

        void set( const Vector<T> &vm );
        void set( int i, T  t );
        void set( T x, T y, T z );
        T get( int i ) const;

        size_t size() const;

        std::string toString( unsigned int digits = 3 ) const;

        friend std::ostream &operator << ( std::ostream &os, Vector<T> &vm )
        {
            for( int i = 0; i < N ; i++ )
            {
                os << vm.data_container[i] << "\t";
            }

            return os;
        }

    protected:
        T data_container[3];
};

template<typename T>
Vector<T>::Vector()
{
    data_container[0] = T();
    data_container[1] = T();
    data_container[2] = T();
}

template<typename T>
Vector<T>::Vector( T x, T y , T z )
{
    data_container[0] = x;
    data_container[1] = y;
    data_container[2] = z;
}

template<typename T>
Vector<T>::Vector( const Vector &vm )
{
    data_container[0] = vm.data_container[0];
    data_container[1] = vm.data_container[1];
    data_container[2] = vm.data_container[2];
}

template<typename T>
Vector<T>::~Vector()
{
}

template<typename T>
Vector<T> &Vector<T>::operator = ( const Vector<T> &vm )
{
    for( int i = 0; i < N ; i++ )
    {
        data_container[i] = vm.data_container[i];
    }

    return *this;
}

template<typename T>
Vector<T> &Vector<T>::operator += ( const Vector<T> &vm )
{
    for( int i = 0; i < N ; i++ )
    {
        data_container[i] += vm.data_container[i];
    }

    return *this;
}

template<typename T>
Vector<T> &Vector<T>::operator *= ( const T &vm )
{
    for( int i = 0; i < N ; i++ )
    {
        data_container[i] *= vm;
    }

    return *this;
}

template<typename T>
Vector<T> &Vector<T>::operator -= ( const Vector<T> &vm )
{
    for( int i = 0; i < N ; i++ )
    {
        data_container[i] -= vm.data_container[i];
    }

    return *this;
}

template<typename T>
void Vector<T>::add( const Vector<T> &vm, Vector<T> &ret ) const
{
    for( int i = 0; i < N ; i++ )
    {
        ret.data_container[i] = data_container[i] + vm.data_container[i];
    }
}


template<typename T>
Vector<T> Vector<T>::operator + ( const Vector<T> &vm ) const
{
    return Vector<T>( data_container[0] + vm.data_container[0], data_container[1] + vm.data_container[1], data_container[2] + vm.data_container[2] );
}

template<typename T>
void Vector<T>::mul( const T &vm, Vector<T> &ret ) const
{
    for( int i = 0; i < N ; i++ )
    {
        ret.data_container[i] = data_container[i] * vm;
    }
}

template<typename T>
T Vector<T>::operator * ( const Vector<T> &vm ) const
{
    T ret = data_container[0] * vm.data_container[0];

    for( int i = 1; i < N ; i++ )
    {
        ret += data_container[i] * vm.data_container[i];
    }

    return ret;
}

template<typename T>
Vector<T> Vector<T>::operator * ( const T &vm ) const
{
    return Vector<T>( data_container[0] * vm , data_container[1] * vm, data_container[2] * vm );
}

template<typename T>
Vector<T> Vector<T>::operator / ( const T &vm ) const
{
    return Vector<T>( data_container[0] / vm, data_container[1] / vm, data_container[2] / vm );
}

template<typename T>
void Vector<T>::sub( const Vector &vm, Vector &ret ) const
{
    for( int i = 0; i < N ; i++ )
    {
        ret.data_container[i] = data_container[i] - vm.data_container[i];
    }
}

template<typename T>
Vector<T> Vector<T>::operator - ( const Vector<T> &vm ) const
{
    return Vector<T>( data_container[0] - vm.data_container[0], data_container[1] - vm.data_container[1], data_container[2] - vm.data_container[2] );
}

template<typename T>
void Vector<T>::set( const Vector<T> &vm )
{
    data_container[0] = vm.data_container[0];
    data_container[1] = vm.data_container[1];
    data_container[2] = vm.data_container[2];
}

template<typename T>
void Vector<T>::set( int i, T  t )
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
void Vector<T>::set( T x, T y, T z )
{
    data_container[0] = x;
    data_container[1] = y;
    data_container[2] = z;
}

template<typename T>
T Vector<T>::get( int i ) const
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
T Vector<T>::operator []( int i ) const
{
    return data_container[i];
}

template<typename T>
T &Vector<T>::operator []( int i )
{
    return data_container[i];
}

template<typename T>
size_t Vector<T>::size() const
{
    return N;
}

template<typename T>
T Vector<T>::dotProduct( const Vector &vm ) const
{
    T ret = data_container[0] * vm.data_container[0];

    for( int i = 1; i < N ; i++ )
    {
        ret += data_container[i] * vm.data_container[i];
    }

    return ret;
}

template<typename T>
void Vector<T>::dotProduct( const Vector &vm, T &ret ) const
{
    ret = data_container[0] * vm.data_container[0];

    for( int i = 1; i < N ; i++ )
    {
        ret += data_container[i] * vm.data_container[i];
    }
}

template<typename T>
void Vector<T>::crossProduct( const Vector<T> &vm, Vector<T> &ret ) const
{
    ret.data_container[0] = data_container[1] * vm.data_container[2] - data_container[2] * vm.data_container[1];
    ret.data_container[1] = data_container[2] * vm.data_container[0] - data_container[0] * vm.data_container[2];
    ret.data_container[2] = data_container[0] * vm.data_container[1] - data_container[1] * vm.data_container[0];
}

template<typename T>
Vector<T> Vector<T>::crossProduct( const Vector<T> &vm ) const
{
    return Vector<T>( data_container[1] * vm.data_container[2] - data_container[2] * vm.data_container[1],
                      data_container[2] * vm.data_container[0] - data_container[0] * vm.data_container[2],
                      data_container[0] * vm.data_container[1] - data_container[1] * vm.data_container[0] );
}


template<typename T>
T Vector<T>::length() const
{
    T tmp = data_container[0] * data_container[0];

    for( int i = 1; i < N; i++ )
    {
        tmp += ( data_container[i] * data_container[i] );
    }

    return std::sqrt( tmp );
}

template<typename T>
T Vector<T>::abs() const
{
    T tmp = data_container[0] * data_container[0];

    for( int i = 1; i < N; i++ )
    {
        tmp += ( data_container[i] * data_container[i] );
    }

    return std::sqrt( tmp );
}

template<typename T>
Vector<T> Vector<T>::rotate( double alpha, double beta, double gamma ) const
{
    const double x = data_container[0], y = data_container[1], z = data_container[2];
    Vector<T> ret;
    ret[0] = -z * cos( alpha ) * sin( beta )
             + y * ( cos( gamma ) * sin( alpha ) + cos( alpha ) * cos( beta ) * sin( gamma ) )
             + x * ( cos( alpha ) * cos( beta ) * cos( gamma ) - sin( alpha ) * sin( gamma ) );
    ret[1] = z * sin( alpha ) * sin( beta )
             + x * ( -cos( beta ) * cos( gamma ) * sin( alpha ) - cos( alpha ) * sin( gamma ) )
             + y * ( cos( alpha ) * cos( gamma ) - cos( beta ) * sin( alpha ) * sin( gamma ) );
    ret[2] = z * cos( beta ) + x * cos( gamma ) * sin( beta ) + y * sin( beta ) * sin( gamma );

    return ret;
}

template<typename T>
void Vector<T>::rotate( double alpha, double beta, double gamma, Vector<T> &ret ) const
{
    const double x = data_container[0], y = data_container[1], z = data_container[2];
    ret[0] = -z * cos( alpha ) * sin( beta )
             + y * ( cos( gamma ) * sin( alpha ) + cos( alpha ) * cos( beta ) * sin( gamma ) )
             + x * ( cos( alpha ) * cos( beta ) * cos( gamma ) - sin( alpha ) * sin( gamma ) );
    ret[1] = z * sin( alpha ) * sin( beta )
             + x * ( -cos( beta ) * cos( gamma ) * sin( alpha ) - cos( alpha ) * sin( gamma ) )
             + y * ( cos( alpha ) * cos( gamma ) - cos( beta ) * sin( alpha ) * sin( gamma ) );
    ret[2] = z * cos( beta ) + x * cos( gamma ) * sin( beta ) + y * sin( beta ) * sin( gamma );
}

template<typename T>
void Vector<T>::rot( double alpha, double beta, double gamma )
{
    double x = data_container[0], y = data_container[1], z = data_container[2];
    data_container[0] = -z * cos( alpha ) * sin( beta )
                        + y * ( cos( gamma ) * sin( alpha ) + cos( alpha ) * cos( beta ) * sin( gamma ) )
                        + x * ( cos( alpha ) * cos( beta ) * cos( gamma ) - sin( alpha ) * sin( gamma ) );
    data_container[1] = z * sin( alpha ) * sin( beta )
                        + x * ( -cos( beta ) * cos( gamma ) * sin( alpha ) - cos( alpha ) * sin( gamma ) )
                        + y * ( cos( alpha ) * cos( gamma ) - cos( beta ) * sin( alpha ) * sin( gamma ) );
    data_container[2] = z * cos( beta ) + x * cos( gamma ) * sin( beta ) + y * sin( beta ) * sin( gamma );
}

template<typename T>
Vector<T> Vector<T>::getNormalized( double epsilon ) const
{
    Vector<T> ret;
    getNormalized( ret, epsilon );
    return ret;
}

template<typename T>
void Vector<T>::getNormalized( Vector<T> &result, double epsilon ) const
{
    T l = length();

    if( l <= epsilon )
    {
        throw Exception( "Length of Vector is Zero" );
    }

    for( int i = 0; i < N; i++ )
    {
        result.data_container[i] = result.data_container[i] / l;
    }

    return result;
}

template<typename T>
void Vector<T>::normalize( double epsilon )
{
    getNormalized( *this, epsilon );
}

template<typename T>
bool Vector<T>::isZero( double epsilon ) const
{
    return ( data_container[0] <= epsilon ) && ( data_container[1] <= epsilon ) && ( data_container[2] <= epsilon );
}

template<typename T>
bool Vector<T>::operator == ( const Vector<T> &vm ) const
{
    return compare_equal( *this, vm );
}

template<typename T>
bool Vector<T>::compare_equal( const Vector<T> &a, const Vector<T> &b, double epsilon ) const
{
    return ( a.data_container[0] - b.data_container[0] ) < epsilon && ( a.data_container[1] - b.data_container[1] ) < epsilon && ( a.data_container[2] - b.data_container[2] ) < epsilon ;
}

template<typename T>
std::string Vector<T>::toString( unsigned int digits ) const
{
    std::stringstream ss;
    ss.precision( digits );
    ss << data_container[0] << "," << data_container[1] << "," << data_container[2];
    return ss.str();
}

#undef N
#endif

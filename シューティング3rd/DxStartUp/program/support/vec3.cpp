//
//  vector
//
//  Created by 常木 徹 on 11/10/19.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <float.h>
#include <math.h>
#include "vec3.h"

namespace t2k {

	//===========================================================================================
	// 反転
	//===========================================================================================
	vec3 vec3::operator - () const
	{
		vec3 tmp ;
		tmp.x = -x ;
		tmp.y = -y ;
		tmp.z = -z ;
		return tmp ;
	}

	//===========================================================================================
	// 加算
	//===========================================================================================
	vec3& vec3::operator += (const vec3& v)
	{
		x += v.x ;
		y += v.y ;
		z += v.z ;
		return *this ;
	}
	vec3 vec3::operator + (const vec3& v) const
	{
		vec3 tmp ;
		tmp.x = x + v.x ;
		tmp.y = y + v.y ;
		tmp.z = z + v.z ;
		return tmp ;
	}

	//===========================================================================================
	// 減算
	//===========================================================================================
	vec3& vec3::operator -= (const vec3& v)
	{
		x -= v.x ;
		y -= v.y ;
		z -= v.z ;
		return *this ;
	}
	vec3 vec3::operator - (const vec3& v) const
	{
		vec3 tmp ;
		tmp.x = x - v.x ;
		tmp.y = y - v.y ;
		tmp.z = z - v.z ;
		return tmp ;
	}

	//===========================================================================================
	// 積算
	//===========================================================================================
	vec3& vec3::operator *= (const float mul)
	{
		x *= mul ;
		y *= mul ;
		z *= mul ;
		return *this ;
	}
	vec3 vec3::operator * (const float mul) const
	{
		vec3 tmp ;
		tmp.x = x * mul ;
		tmp.y = y * mul ;
		tmp.z = z * mul ;
		return tmp ;
	}

	vec3& vec3::operator *= (const vec3& v)
	{
		x *= v.x ;
		y *= v.y ;
		z *= v.z ;
		return *this ;
	}
	vec3 vec3::operator * (const vec3& v) const
	{
		vec3 tmp ;
		tmp.x = x * v.x ;
		tmp.y = y * v.y ;
		tmp.z = z * v.z ;
		return tmp ;
	}


	//===========================================================================================
	// 除算
	//===========================================================================================
	vec3& vec3::operator /= (const float div)
	{
		x = ( div == 0.0f ) ? 0 : x / div ;
		y = ( div == 0.0f ) ? 0 : y / div ;
		z = ( div == 0.0f ) ? 0 : z / div ;
		return *this ;
	}
	vec3 vec3::operator / (const float div) const
	{
		vec3 tmp ;
		tmp.x = ( div == 0.0f ) ? 0 : x / div ;
		tmp.y = ( div == 0.0f ) ? 0 : y / div ;
		tmp.z = ( div == 0.0f ) ? 0 : z / div ;
		return tmp ;
	}
	vec3& vec3::operator /= (const vec3& v)
	{
		x = ( v.x == 0.0f ) ? 0 : x / v.x ;
		y = ( v.y == 0.0f ) ? 0 : y / v.y ;
		z = ( v.z == 0.0f ) ? 0 : z / v.z ;
		return *this ;
	}
	vec3 vec3::operator / (const vec3& v) const
	{
		vec3 tmp ;
		tmp.x = ( v.x == 0.0f ) ? 0 : x / v.x ;
		tmp.y = ( v.y == 0.0f ) ? 0 : y / v.y ;
		tmp.z = ( v.z == 0.0f ) ? 0 : z / v.z ;
		return tmp ;
	}


	//===========================================================================================
	// 比較
	//===========================================================================================
	bool vec3::operator == (const vec3& v) const
	{
		if( fabsf(x - v.x) >= FLT_MIN ) return false ;
		if( fabsf(y - v.y) >= FLT_MIN ) return false ;
		if( fabsf(z - v.z) >= FLT_MIN ) return false ;
		return true ;
	}
	bool vec3::operator != (const vec3& v) const
	{
		if( fabsf(x - v.x) < FLT_MIN ) return false ;
		if( fabsf(y - v.y) < FLT_MIN ) return false ;
		if( fabsf(z - v.z) < FLT_MIN ) return false ;
		return true ;
	}

	//===========================================================================================
	// name... vec3Normalize 
	// work... ベクトルの正規化
	//===========================================================================================
	vec3 vec3Normalize(vec3& _v)
	{
		vec3 v ;
		float len = sqrt(( _v.x * _v.x ) + ( _v.y * _v.y ) + ( _v.z * _v.z )) ;
		len = ( len > 0.0f ) ? 1.0f / len : len ;
		v.x = _v.x * len ;
		v.y = _v.y * len ;
		v.z = _v.z * len ;
		return v ;
	}

	//===========================================================================================
	// name... vec3Dot 
	// work... ベクトルの内積
	//===========================================================================================
	float vec3Dot(const vec3& _v1, const vec3& _v2)
	{
		return ( _v1.x * _v2.x ) + ( _v1.y * _v2.y ) + ( _v1.z * _v2.z ) ;
	}

	//===========================================================================================
	// name... vec3Cross 
	// work... ベクトルの外積
	//===========================================================================================
	vec3 vec3Cross(const vec3& _v1, const vec3& _v2)
	{
		vec3 v ;
		v.x = ( _v1.y * _v2.z ) - ( _v1.z * _v2.y ) ;
		v.y = ( _v1.z * _v2.x ) - ( _v1.x * _v2.z ) ;
		v.z = ( _v1.x * _v2.y ) - ( _v1.y * _v2.x ) ;
		return v ;
	}

	//===========================================================================================
	// name... vec3Angle 
	// work... ベクトルの角度
	//===========================================================================================
	float vec3Angle(const vec3& _v1, const vec3& _v2)
	{

		vec3 v1 = _v1 ;
		vec3 v2 = _v2 ;
		v1 = vec3Normalize(v1) ;
		v2 = vec3Normalize(v2) ;

		float wkDot = vec3Dot(v1, v2) ;

		if( 1.0f < fabs(wkDot) ) {
			if( 0.0f < wkDot ) {
				wkDot = 1.0f ;
			}
			else {
				wkDot = -1.0f ;
			}
		}

		return acos(wkDot) ;
	}

	//===========================================================================================
	// name... vec3Distance
	// work... ベクトルの距離
	//===========================================================================================
	float vec3Distance(const vec3& _v1, const vec3& _v2)
	{
		vec3 v ;
		v = (_v1)-( _v2 ) ;
		return sqrt(( v.x * v.x ) + ( v.y * v.y ) + ( v.z * v.z )) ;
	}

	//===========================================================================================
	// name... vec3Length
	// work... ベクトルの長さ
	//===========================================================================================
	float vec3Length(const vec3& _v)
	{
		return sqrt(( _v.x * _v.x ) + ( _v.y * _v.y ) + ( _v.z * _v.z )) ;
	}

	//===========================================================================================
	// name... vec3Lerp
	// work... ベクトルの線形補間
	//===========================================================================================
	vec3 vec3Lerp(const vec3& _v1, const vec3& _v2, float t)
	{

		vec3 v ;
		float d = vec3Distance(_v2, _v1) ;
		v = (_v2)-( _v1 ) ;
		v = vec3Normalize(v) ;
		t = d * t ;
		v = v * t ;
		v = (_v1)+v ;
		return v ;
	}

	//===========================================================================================
	// name... vec3BezierSpline
	// work... 3次元空間上のベジェスプライン曲線補間
	//===========================================================================================
	vec3 vec3BezierSpline(const vec3& _a1, const vec3& _a2, const vec3& _a3, const vec3& _a4, float t)
	{
		vec3 v ;

		t = ( 1.0f < t ) ? 1.0f : t ;
		t = ( 0.0f > t ) ? 0.0f : t ;

		float wkT = ( 1.0f - t ) ;
		float wkX, wkY, wkZ ;
		wkX = wkY = wkZ = 0.0f ;

		float T2 = ( t*t ) ;
		float T3 = ( t*t*t ) ;
		float wkT2 = ( wkT*wkT ) ;
		float wkT3 = ( wkT*wkT*wkT ) ;

		wkX = ( (wkT3)*_a1.x ) ;
		wkX = wkX + ( ( 3 * ( wkT2 ) )*( t*_a2.x ) ) ;
		wkX = wkX + ( ( ( 3 * wkT )*( T2 ) )*( _a3.x ) ) ;
		wkX = wkX + ( (T3)*_a4.x ) ;

		wkY = ( (wkT3)*_a1.y ) ;
		wkY = wkY + ( ( 3 * ( wkT2 ) )*( t*_a2.y ) ) ;
		wkY = wkY + ( ( ( 3 * wkT )*( T2 ) )*( _a3.y ) ) ;
		wkY = wkY + ( (T3)*_a4.y ) ;

		wkZ = ( (wkT3)*_a1.z ) ;
		wkZ = wkZ + ( ( 3 * ( wkT2 ) )*( t*_a2.z ) ) ;
		wkZ = wkZ + ( ( ( 3 * wkT )*( T2 ) )*( _a3.z ) ) ;
		wkZ = wkZ + ( (T3)*_a4.z ) ;

		v.x = wkX ;
		v.y = wkY ;
		v.z = wkZ ;

		return v ;
	}


}
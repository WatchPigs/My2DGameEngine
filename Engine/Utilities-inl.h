#include <assert.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

namespace Engine
{
namespace Math
{
	
inline float DegreesToRadians( float i_Degrees )
{
	static const float DegToRad = float( M_PI ) / 180.0f;

	return i_Degrees * DegToRad;
}

inline double DegreesToRadians( double i_Degrees )
{
	static const double DegToRad = M_PI / 180.0;

	return i_Degrees * DegToRad;
}

inline bool IsNAN( float i_val )
{
	volatile float val = i_val;

	return val != val;
}

inline bool IsZero( float i_val )
{
	return AreEqual_Eps( 0.0f, i_val, .000000001f );
}

inline bool AreEqual_Eps( float i_lhs, float i_rhs, float i_maxDiff )
{
	return fabs( i_lhs - i_rhs ) < i_maxDiff;
}

inline bool AreEqual_Rel( float i_lhs, float i_rhs, float i_maxDiff )
{
	if( i_lhs == i_rhs )
		return true;

	float relDiff;

	if( fabs( i_rhs ) > fabs( i_lhs ) )
		relDiff = static_cast<float>(fabs( (i_lhs - i_rhs ) / i_rhs ));
	else
		relDiff = static_cast<float>(fabs( (i_lhs - i_rhs ) / i_lhs ));

	return relDiff <= i_maxDiff;
}

inline bool AreEqual_Accurate( float i_lhs, float i_rhs, float i_maxDiff, unsigned int i_maxULPS )
{
	assert( sizeof( float ) == sizeof( int ) );

	if( i_lhs == i_rhs )
		return true;

	double diff = fabs( i_lhs - i_rhs );

	if( diff <= i_maxDiff )
		return true;

	unsigned int intDiff = abs( *reinterpret_cast<int *>( &i_lhs ) - *reinterpret_cast<int *>( &i_rhs ) );

	return intDiff <= i_maxULPS;
}

inline unsigned int RandInRange( unsigned int i_lowerBound, unsigned int i_upperBound )
{
	assert( i_lowerBound < i_upperBound );

	return i_lowerBound + rand() % (i_upperBound - i_lowerBound );
}

template<typename T>
inline void Swap( T & i_Left, T & i_Right )
{
	T temp = i_Left;
	i_Left = i_Right;
	i_Right = temp;
}

} // namespace Math
} // namespace Engine
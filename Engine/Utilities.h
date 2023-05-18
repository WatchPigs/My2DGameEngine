#pragma once

#include <stdint.h>

namespace Engine {
	inline float 			DegreesToRadians(float i_Degrees);
	inline double 			DegreesToRadians(double i_Degrees);

	inline bool 			IsNAN(float i_val);
	inline bool 			IsZero(float i_val);

	// fastest
	inline bool 			AreEqual_Eps(float i_lhs, float i_rhs, float i_maxDiff = 0.00001f);
	// balanced
	inline bool 			AreEqual_Rel(float i_lhs, float i_rhs, float i_maxDiff = 0.00001f);
	// slow but sure
	inline bool 			AreEqual_Accurate(float i_lhs, float i_rhs, float i_maxDiff, unsigned int i_maxULPS = 12);

	inline unsigned int 	RandInRange(unsigned int i_lowerBound, unsigned int i_upperBound);

	template< typename T>
	inline void				Swap(T& i_Left, T& i_Right);

	static const uint32_t 	allBitsSet = ~0;
} // namespace Engine

#include "Utilities.inl"

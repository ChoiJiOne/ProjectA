#pragma once

#include <algorithm>
#include <cmath>
#include <chrono>
#include <random>

#include "Matrix2x2.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"


/**
 * @brief 파이(π) 값입니다.
 */
const float Pi = 3.141592654f;


/**
 * @brief 2파이(2π) 값입니다.
 */
const float TwoPi = 6.283185307f;


/**
 * @brief 파이(π) 의 역수 값입니다.
 */
const float OneDivPi = 0.318309886f;


/**
 * @brief 2파이(2π) 의 역수 값입니다.
 */
const float OneDivTwoPi = 0.159154943f;


/**
 * @brief 파이(π) 의 반값(π/2)입니다.
 */
const float PiDiv2 = 1.570796327f;


/**
 * @brief 파이(π) 의 반의 반 값(π/4)입니다.
 */
const float PiDiv4 = 0.785398163f;


/**
 * @brief 양의 무한대 값입니다.
 */
const float Infinity = std::numeric_limits<float>::infinity();


/**
 * @brief 음의 무한대 값입니다.
 */
const float NInfinity = -std::numeric_limits<float>::infinity();


/**
 * @brief 엡실론(ε) 값입니다.
 */
const float Epsilon = std::numeric_limits<float>::epsilon();


/**
 * @brief 수학 관련 확장 기능을 제공합니다.
 */
class MathUtils
{
public:
	/**
	 * @brief 라디안 각을 육십분법 각으로 변환합니다.
	 *
	 * @param radian 변환할 라디안 각입니다.
	 *
	 * @return 변환된 육십분법 각입니다.
	 */
	static inline float ToDegree(float radian)
	{
		return (radian * 180.0f) / Pi;
	}


	/**
	 * @brief 육십분법 각을 라디안 각으로 변환합니다.
	 *
	 * @param degree 변환할 육십분법 각입니다.
	 *
	 * @return 변환된 라디안 각입니다.
	 */
	static inline float ToRadian(float degree)
	{
		return (degree * Pi) / 180.0f;
	}


	/**
	 * @brief 부동 소수점 값이 0에 가까운지 확인합니다.
	 *
	 * @param value 0에 가까운지 확인할 값입니다.
	 * @param epsilon 값 확인을 위한 엡실론 값입니다.
	 *
	 * @return 부동 소수점 값이 0에 가깝다면 true, 그렇지 않다면 false를 반환합니다.
	 */
	static inline bool NearZero(float value, float epsilon = Epsilon)
	{
		return (std::fabs(value) <= epsilon);
	}


	/**
	 * @brief 두 값중 큰 값을 반환합니다.
	 *
	 * @param lhs 크기를 비교할 값 중 왼쪽 값입니다.
	 * @param rhs 크기를 비교할 값 중 오른쪽 값입니다.
	 *
	 * @return 두 값 중 큰 값을 반환합니다.
	 */
	template <typename T>
	static inline T Max(const T& lhs, const T& rhs)
	{
		return (lhs < rhs ? rhs : lhs);
	}


	/**
	 * @brief 두 값중 작은 값을 반환합니다.
	 *
	 * @param lhs 크기를 비교할 값 중 왼쪽 값입니다.
	 * @param rhs 크기를 비교할 값 중 오른쪽 값입니다.
	 *
	 * @return 두 값 중 작은 값을 반환합니다.
	 */
	template <typename T>
	static inline T Min(const T& lhs, const T& rhs)
	{
		return (lhs < rhs ? lhs : rhs);
	}
};
#pragma once

#include <array>
#include <vector>

#include "Shader.h"


/**
 * @brief 3D 기본 도형을 렌더링하는 셰이더입니다.
 */
class GeometryShader3D : public Shader
{
public:
	/**
	 * @brief 3D 기본 도형을 렌더링하는 셰이더의 디폴트 생성자입니다.
	 *
	 * @note 생성자 이외의 메서드에서 적절한 초기화를 수행해야 합니다.
	 */
	GeometryShader3D() = default;


	/**
	 * @brief 3D 기본 도형을 렌더링하는 셰이더의 가상 소멸자입니다.
	 *
	 * @note 셰이더의 내부에서 할당된 요소를 해제하기 위해서는 반드시 Release를 호출해야 합니다.
	 */
	virtual ~GeometryShader3D();


	/**
	 * @brief 3D 기본 도형을 렌더링하는 셰이더의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(GeometryShader3D);


	/**
	 * @brief GLSL 셰이더 소스 파일을 컴파일하고 셰이더 프로그램을 생성합니다.
	 *
	 * @param vsFile 버텍스 셰이더의 확장자(.vert)를 포함한 경로입니다.
	 * @param fsFile 프레그먼트 셰이더의 확장자(.frag)를 포함한 경로입니다.
	 */
	virtual void Initialize(const std::wstring & vsPath, const std::wstring & fsPath) override;


	/**
	 * @brief 셰이더의 내부 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 화면에 3D 점들을 그립니다.
	 * 
	 * @param view 시야 행렬입니다.
	 * @param projection 투영 행렬입니다.
	 * @param positions 화면 상의 3D 점들입니다.
	 * @param color 점들의 색상입니다.
	 * 
	 * @note 3D 점들의 개수는 MAX_VERTEX_SIZE(10000)의 크기를 넘을 수 없습니다.
	 */
	void DrawPoints3D(const Matrix4x4f& view, const Matrix4x4f& projection, const std::vector<Vector3f>& positions, const Vector4f& color);


	/**
	 * @brief 화면에 3D 좌표 공간을 그립니다.
	 * 
	 * @param view 시야 행렬입니다.
	 * @param projection 투영 행렬입니다.
	 * @param minPosition 좌표 공간의 최소값입니다.
	 * @param maxPosition 좌표 공간의 최대값입니다.
	 * @param gap 격자 간 간격입니다.
	 * @param color X,Y,Z 축 이외의 격자 색상입니다.
	 *
	 * @note X축의 색상은 Red, Y축의 색상은 Green, Z축의 색상은 Blue입니다.
	 */
	void DrawAxisGrid3D(const Matrix4x4f& view, const Matrix4x4f& projection, const Vector3f& minPosition, const Vector3f& maxPosition, float gap, const Vector4f& color);

	
private:
	/**
	 * @brief 3D 기본 도형 그리기를 수행하는 셰이더 내부에서 사용하는 정점입니다.
	 */
	struct VertexPositionColor
	{
		/**
		 * @brief 위치와 색상 정보를 가진 정점의 기본 생성자입니다.
		 */
		VertexPositionColor() noexcept
			: position(0.0f, 0.0f, 0.0f)
			, color(0.0f, 0.0f, 0.0f, 0.0f) {}


		/**
		 * @brief 위치와 색상 정보를 가진 정점의 생성자입니다.
		 *
		 * @param position 정점의 위치입니다.
		 * @param color 정점의 색상입니다.
		 */
		VertexPositionColor(Vector3f&& position, Vector4f&& color) noexcept
			: position(position)
			, color(color) {}


		/**
		 * @brief 위치와 색상 정보를 가진 정점의 생성자입니다.
		 *
		 * @param position 정점의 위치입니다.
		 * @param color 정점의 색상입니다.
		 */
		VertexPositionColor(const Vector3f& position, const Vector4f& color) noexcept
			: position(position)
			, color(color) {}


		/**
		 * @brief 위치와 색상 정보를 가진 정점의 생성자입니다.
		 *
		 * @param x 위치의 X좌표입니다.
		 * @param y 위치의 Y좌표입니다.
		 * @param z 위치의 Z좌표입니다.
		 * @param r 정점 색상의 R입니다.
		 * @param g 정점 색상의 G입니다.
		 * @param b 정점 색상의 B입니다.
		 * @param a 정점 색상의 A입니다.
		 */
		VertexPositionColor(
			float x, float y, float z,
			float r, float g, float b, float a
		) noexcept
			: position(x, y, z)
			, color(r, g, b, a) {}


		/**
		 * @brief 위치와 색상 정보를 가진 정점의 생성자입니다.
		 *
		 * @param instance 복사할 정점 인스턴스입니다.
		 */
		VertexPositionColor(VertexPositionColor&& instance) noexcept
			: position(instance.position)
			, color(instance.color) {}


		/**
		 * @brief 위치와 색상 정보를 가진 정점의 생성자입니다.
		 *
		 * @param instance 복사할 정점 인스턴스입니다.
		 */
		VertexPositionColor(const VertexPositionColor& instance) noexcept
			: position(instance.position)
			, color(instance.color) {}


		/**
		 * @brief 위치와 색상 정보를 가진 정점의 대입 연산자입니다.
		 *
		 * @param instance 대입할 정점 인스턴스입니다.
		 *
		 * @return 대입한 객체의 참조자를 반환합니다.
		 */
		VertexPositionColor& operator=(VertexPositionColor&& instance) noexcept
		{
			if (this == &instance) return *this;

			position = instance.position;
			color = instance.color;

			return *this;
		}


		/**
		 * @brief 위치와 색상 정보를 가진 정점의 대입 연산자입니다.
		 *
		 * @param instance 대입할 정점 인스턴스입니다.
		 *
		 * @return 대입한 객체의 참조자를 반환합니다.
		 */
		VertexPositionColor& operator=(const VertexPositionColor& instance) noexcept
		{
			if (this == &instance) return *this;

			position = instance.position;
			color = instance.color;

			return *this;
		}


		/**
		 * @brief 정점의 바이트 보폭 값을 얻습니다.
		 *
		 * @return 정점의 파이트 보폭(Stride) 값을 반환합니다.
		 */
		static uint32_t GetStride()
		{
			return sizeof(VertexPositionColor);
		}


		/**
		 * @brief 정점의 위치입니다.
		 */
		Vector3f position;


		/**
		 * @brief 정점의 색상입니다.
		 */
		Vector4f color;
	};


private:
	/**
	 * @brief 도형 정점 목록의 최대 크기입니다.
	 */
	static const int32_t MAX_VERTEX_SIZE = 10000;


	/**
	 * @brief 도형의 정점 목록입니다.
	 */
	std::array<VertexPositionColor, MAX_VERTEX_SIZE> vertices_;


	/**
	 * @brief 정점 버퍼의 오브젝트입니다.
	 */
	uint32_t vertexBufferObject_ = 0;


	/**
	 * @brief 정점 버퍼 목록의 오브젝트입니다.
	 */
	uint32_t vertexArrayObject_ = 0;
};
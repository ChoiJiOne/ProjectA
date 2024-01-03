#pragma once

#include <string>

#include "IResource.h"


/**
 * @brief 큐브맵 파일을 로딩하고 OpenGL 파이프라인에 바인딩 가능한 텍스처 리소스를 생성 및 관리합니다.
 */
class CubeMap : public IResource
{
public:
	/**
	 * @brief 큐브맵의 디폴트 생성자입니다.
	 * 
	 * @note 생성자 이외의 메서드에서 적절한 초기화를 수행해야 합니다.
	 */
	CubeMap() = default;


	/**
	 * @brief 큐브맵의 가상 소멸자입니다.
	 * 
	 * @note 큐브맵 내부에서 할당된 요소를 해제하기 위해서는 반드시 Release를 호출해야 합니다.
	 */
	virtual ~CubeMap();


	/**
	 * @brief 큐브맵의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(CubeMap);


	/**
	 * @brief 큐브맵 이미지 파일을 읽고 큐브맵 리소스를 생성합니다.
	 *
	 * @param rightPath 큐브맵 RIGHT(+X) 부분의 텍스처 리소스 경로입니다.
	 * @param leftPath 큐브맵 LEFT(-X) 부분의 텍스처 리소스 경로입니다.
	 * @param topPath 큐브맵 TOP(+Y) 부분의 텍스처 리소스 경로입니다.
	 * @param bottomPath 큐브맵 BOTTOM(-Y) 부분의 텍스처 리소스 경로입니다.
	 * @param frontPath 큐브맵 FRONT(+Z) 부분의 텍스처 리소스 경로입니다.
	 * @param backPath 큐브맵 BACK(-Z) 부분의 텍스처 리소스 경로입니다.
	 */
	void Initialize(
		const std::wstring& rightPath,
		const std::wstring& leftPath,
		const std::wstring& topPath,
		const std::wstring& bottomPath,
		const std::wstring& frontPath,
		const std::wstring& backPath
	);


	/**
	 * @brief 큐브맵 내부 리소스를 할당 해제합니다.
	 */
	virtual void Release() override;


	/**
	 * @brief 큐브맵을 활성화합니다.
	 * 
	 * @param unit 활성화 할 텍스처 유닛입니다.
	 */
	void Active(uint32_t unit) const;


private:
	/**
	 * @brief 큐브맵 텍스처의 ID입니다.
	 */
	uint32_t cubeMapID_ = 0;
};

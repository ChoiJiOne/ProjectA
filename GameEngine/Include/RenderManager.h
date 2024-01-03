#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <windows.h>

#include "IManager.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix2x2.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"

class Window;
class Shader;
class Mesh;
class Texture2D;
class TTFont;


/**
 * @brief 렌더링 처리를 수행하는 매니저입니다.
 * 
 * @note 이 클래스는 싱글턴입니다.
 */
class RenderManager : public IManager
{
public:
	/**
	 * @brief 렌더링 처리를 수행하는 매니저의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(RenderManager);


	/**
	 * @brief 렌더링 처리를 수행하는 매니저의 참조 인스턴스를 얻습니다.
	 * 
	 * @return 렌더링 처리를 수행하는 매니저의 참조자를 반환합니다.
	 */
	static RenderManager& Get()
	{
		static RenderManager instance;
		return instance;
	}


	/**
	 * @brief 렌더링 처리를 수행하는 매니저의 사용을 시작합니다.
	 * 
	 * @note 
	 * - 이 메서드 호출 전에 렌더링 대상이 되는 윈도우 포인터를 설정해야 합니다.
	 * - 이 메서드는 한 번만 호출되어야 합니다.
	 */
	virtual void Startup() override;


	/**
	 * @brief 렌더링 처리를 수행하는 매니저의 사용을 종료합니다.
	 * 
	 * @note
	 * - 애플리케이션 종료 전에 이 메서드를 반드시 호출하여 내부 리소스를 정리해야 합니다.
	 * - 이 메서드는 반드시 한 번만 호출되어야 합니다.
	 */
	virtual void Shutdown() override;


	/**
	 * @brief 렌더링 대상이 되는 윈도우를 설정합니다.
	 * 
	 * @param window 렌더링 대상이 되는 윈도우입니다.
	 */
	void SetRenderTargetWindow(Window* window) { renderTargetWindow_ = window; }


	/**
	 * @brief 렌더링 대상이 되는 윈도우를 얻습니다.
	 * 
	 * @return 렌더링 대상이 되는 윈도우의 포인터를 반환합니다.
	 */
	Window* GetRenderTargetWindow() const { return renderTargetWindow_; }


	/**
	 * @brief 프레임 렌더링을 시작합니다.
	 * 
	 * @param red 색상 버퍼의 R 값입니다.
	 * @param green 색상 버퍼의 G 값입니다.
	 * @param blue 색상 버퍼의 B 값입니다.
	 * @param alpha 색상 버퍼의 A 값입니다.
	 * @param depth 깊이 버퍼의 초기화 값입니다.
	 * @param stencil 스텐실 버퍼의 초기화 값입니다.
	 */
	void BeginFrame(float red, float green, float blue, float alpha, float depth = 1.0f, uint8_t stencil = 0);


	/**
	 * @brief 백버퍼와 프론트 버퍼를 스왑합니다.
	 */
	void EndFrame();


	/**
	 * @brief 뷰 포트를 설정합니다.
	 * 
	 * @param x 뷰포트 사각형의 왼쪽 아래 모서리(픽셀)입니다. 기본값은 (0,0)입니다.
	 * @param y 뷰포트 사각형의 왼쪽 아래 모서리(픽셀)입니다. 기본값은 (0,0)입니다.
	 * @param width 뷰포트의 너비입니다. OpenGL 컨텍스트가 창에 처음 연결되면 너비 와 높이 가 해당 창의 크기로 설정됩니다.
	 * @param height 뷰포트의 높이입니다. OpenGL 컨텍스트가 창에 처음 연결되면 너비 와 높이 가 해당 창의 크기로 설정됩니다.
	 */
	void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);


	/**
	 * @brief 수직 동기화 옵션을 설정합니다.
	 * 
	 * @param bIsEnable 수직 동기화 옵션을 활성화하려면 true, 비활성화하려면 false입니다.
	 */
	void SetVsyncMode(bool bIsEnable);


	/**
	 * @brief 깊이 테스트 옵션을 설정합니다.
	 * 
	 * @param bIsEnable 깊이 테스트 옵션을 활성화하려면 true, 비활성화하려면 false입니다.
	 */
	void SetDepthMode(bool bIsEnable);


	/**
	 * @brief 스텐실 테스트 옵션을 설정합니다.
	 * 
	 * @param bIsEnable 스텐실 테스트 옵션을 활성화하려면 true, 비활성화하려면 false입니다.
	 */
	void SetStencilMode(bool bIsEnable);


	/**
	 * @brief 알파 블랜딩 옵션을 설정합니다.
	 * 
	 * @param bIsEnable 알파 블랜딩 옵션을 활성화하려면 true, 비활성화하려면 false입니다.
	 */
	void SetAlphaBlendMode(bool bIsEnable);


	/**
	 * @brief 윈도우 크기 변경 시 내부에서 윈도우 크기와 관련된 요소를 업데이트합니다.
	 */
	void Resize();


	/**
	 * @brief 화면에 2D 점들을 그립니다.
	 *
	 * @note 2D 점들의 개수는 MAX_VERTEX_SIZE(10000)의 크기를 넘을 수 없습니다.
	 *
	 * @param positions 화면 상의 2D 점들입니다.
	 * @param color 점들의 RGB 색상입니다.
	 * @param pointSize 점의 크기입니다. 기본 값은 1.0f입니다.
	 */
	void RenderPoints2D(const std::vector<Vector2f>& positions, const Vector4f& color, float pointSize = 1.0f);


	/**
	 * @brief 화면에 점들을 연결한 2D 선을 그립니다.
	 *
	 * @note 2D 점들의 개수는 MAX_VERTEX_SIZE(10000)의 크기를 넘을 수 없습니다.
	 *
	 * @param positions 화면 상의 2D 점들입니다.
	 * @param color 점들의 RGB 색상입니다.
	 */
	void RenderConnectPoints2D(const std::vector<Vector2f>& positions, const Vector4f& color);


	/**
	 * @brief 화면에 2D 선을 그립니다.
	 *
	 * @param fromPosition 선의 시작점입니다.
	 * @param toPosition 선의 끝점입니다.
	 * @param color 선의 RGBA색상입니다.
	 */
	void RenderLine2D(const Vector2f& fromPosition, const Vector2f& toPosition, const Vector4f& color);


	/**
	 * @brief 화면에 2D 선을 그립니다.
	 *
	 * @param fromPosition 선의 시작점입니다.
	 * @param fromColor 선의 시작점 색상입니다.
	 * @param toPosition 선의 끝점입니다.
	 * @param toColor 선의 끝점 색상입니다.
	 */
	void RenderLine2D(const Vector2f& fromPosition, const Vector4f& fromColor, const Vector2f& toPosition, const Vector4f& toColor);


	/**
	 * @brief 화면에 2D 삼각형을 그립니다.
	 *
	 * @param fromPosition 삼각형 시작 점입니다.
	 * @param byPosition 삼각형 중간 점입니다.
	 * @param toPosition 삼각형 끝 점입니다.
	 * @param color 삼각형의 RGBA색상입니다.
	 */
	void RenderTriangle2D(const Vector2f& fromPosition, const Vector2f& byPosition, const Vector2f& toPosition, const Vector4f& color);


	/**
	 * @brief 화면에 2D 삼각형을 그립니다.
	 *
	 * @param ortho 직교 투영 행렬입니다.
	 * @param fromPosition 삼각형 시작 점입니다.
	 * @param fromColor 삼각형 시작 점의 색상입니다.
	 * @param byPosition 삼각형 중간 점입니다.
	 * @param byColor 삼각형 중간 점의 색상입니다.
	 * @param toPosition 삼각형 끝 점입니다.
	 * @param toColor 삼각형 끝 점의 색상입니다.
	 */
	void RenderTriangle2D(
		const Vector2f& fromPosition, const Vector4f& fromColor,
		const Vector2f& byPosition, const Vector4f& byColor,
		const Vector2f& toPosition, const Vector4f& toColor
	);


	/**
	 * @brief 화면에 2D 와이어 프레임 삼각형을 그립니다.
	 *
	 * @param ortho 직교 투영 행렬입니다.
	 * @param fromPosition 삼각형 시작 점입니다.
	 * @param byPosition 삼각형 중간 점입니다.
	 * @param toPosition 삼각형 끝 점입니다.
	 * @param color 삼각형의 RGBA색상입니다.
	 */
	void RenderWireframeTriangle2D(const Vector2f& fromPosition, const Vector2f& byPosition, const Vector2f& toPosition, const Vector4f& color);


	/**
	 * @brief 화면에 2D 와이어 프레임 삼각형을 그립니다.
	 *
	 * @param ortho 직교 투영 행렬입니다.
	 * @param fromPosition 삼각형 시작 점입니다.
	 * @param fromColor 삼각형 시작 점의 색상입니다.
	 * @param byPosition 삼각형 중간 점입니다.
	 * @param byColor 삼각형 중간 점의 색상입니다.
	 * @param toPosition 삼각형 끝 점입니다.
	 * @param toColor 삼각형 끝 점의 색상입니다.
	 */
	void RenderWireframeTriangle2D(
		const Vector2f& fromPosition, const Vector4f& fromColor,
		const Vector2f& byPosition, const Vector4f& byColor,
		const Vector2f& toPosition, const Vector4f& toColor
	);


	/**
	 * @brief 화면에 2D 직사각형을 그립니다.
	 *
	 * @param ortho 직교 투영 행렬입니다.
	 * @param center 직사각형의 중심 좌표입니다.
	 * @param width 직사각형의 가로 크기입니다.
	 * @param heigt 직사각형의 세로 크기입니다.
	 * @param rotate 직사각형의 라디안 회전 각도입니다.
	 * @param color 직사각형의 색상입니다.
	 */
	void RenderRectangle2D(const Vector2f& center, float width, float height, float rotate, const Vector4f& color);


	/**
	 * @brief 화면에 2D 와이어 프레임 직사각형을 그립니다.
	 *
	 * @param center 직사각형의 중심 좌표입니다.
	 * @param width 직사각형의 가로 크기입니다.
	 * @param heigt 직사각형의 세로 크기입니다.
	 * @param rotate 직사각형의 라디안 회전 각도입니다.
	 * @param color 직사각형의 색상입니다.
	 */
	void RenderWireframeRectangle2D(const Vector2f& center, float width, float height, float rotate, const Vector4f& color);


	/**
	 * @brief 화면에 2D 원을 그립니다.
	 *
	 * @param center 원의 중심 좌표입니다.
	 * @param radius 원의 반지름 길이입니다.
	 * @param color 원의 색상입니다.
	 * @param sliceCount 원의 둘레 절단 횟수입니다. 기본 값은 300입니다.
	 */
	void RenderCircle2D(const Vector2f& center, float radius, const Vector4f& color, int32_t sliceCount = 300);


	/**
	 * @brief 화면에 2D 와이어 프레임 원을 그립니다.
	 *
	 * @param center 원의 중심 좌표입니다.
	 * @param radius 원의 반지름 길이입니다.
	 * @param color 원의 색상입니다.
	 * @param sliceCount 원 둘레의 절단 횟수입니다. 기본 값은 300입니다.
	 */
	void RenderWireframeCircle2D(const Vector2f& center, float radius, const Vector4f& color, int32_t sliceCount = 300);


	/**
	 * @brief 화면에 2D 타원을 그립니다.
	 *
	 * @param center 타원의 중심 좌표입니다.
	 * @param xAxis 타원의 x축 길이입니다. 만약, 이 길이가 y축 길이보다 크다면 장축이 되고, 그렇지 않으면 단축이 됩니다.
	 * @param yAxis 타원의 y축 길이입니다. 만약, 이 길이가 x축 길이보다 크다면 장축이 되고, 그렇지 않으면 단축이 됩니다.
	 * @param color 타원의 색상입니다.
	 * @param sliceCount 타원 둘레의 절단 횟수입니다. 기본 값은 300입니다.
	 */
	void RenderEllipse2D(const Vector2f& center, float xAxis, float yAxis, const Vector4f& color, int32_t sliceCount = 300);


	/**
	 * @brief 화면에 2D 와이어 프레임 타원을 그립니다.
	 *
	 * @param center 타원의 중심 좌표입니다.
	 * @param xAxis 타원의 x축 길이입니다. 만약, 이 길이가 y축 길이보다 크다면 장축이 되고, 그렇지 않으면 단축이 됩니다.
	 * @param yAxis 타원의 y축 길이입니다. 만약, 이 길이가 x축 길이보다 크다면 장축이 되고, 그렇지 않으면 단축이 됩니다.
	 * @param color 타원의 색상입니다.
	 * @param sliceCount 타원 둘레의 절단 횟수입니다. 기본 값은 300입니다.
	 */
	void RenderWireframeEllipse2D(const Vector2f& center, float xAxis, float yAxis, const Vector4f& color, int32_t sliceCount = 300);


	/**
	 * @brief 화면에 2D 격자를 그립니다.
	 *
	 * @param minX X좌표의 최소값입니다.
	 * @param maxX X좌표의 최댓값입니다.
	 * @param strideX 격차의 X축 방향으로의 간격입니다.
	 * @param minY Y좌표의 최소값입니다.
	 * @param maxY Y좌표의 최댓값입니다.
	 * @param strideY 격차의 Y축 방향으로의 간격입니다.
	 * @param color 격자의 색상입니다.
	 */
	void RenderGrid2D(float minX, float maxX, float strideX, float minY, float maxY, float strideY, const Vector4f& color);


	/**
	 * @brief 2D 텍스트를 화면에 그립니다.
	 *
	 * @param font 폰트 리소스입니다.
	 * @param text 렌더링할 텍스트입니다.
	 * @param center 텍스트의 중심 좌표입니다.
	 * @param color 텍스트의 RGBA 색상입니다.
	 */
	void RenderText2D(const TTFont* font, const std::wstring& text, const Vector2f& center, const Vector4f& color);


	/**
	 * @brief 3D 점들을 화면에 그립니다.
	 * 
	 * @param view 시야 행렬입니다.
	 * @param projection 투영 행렬입니다.
	 * @param positions 화면 상의 3D 점들입니다.
	 * @param color 점들의 색상입니다.
	 * 
	 * @note 3D 점들의 개수는 10000개를 넘을 수 없습니다.
	 */
	void RenderPoints3D(const Matrix4x4f& view, const Matrix4x4f& projection, const std::vector<Vector3f>& positions, const Vector4f& color);


	/**
	 * @brief 점들을 연결한 3D 선을 화면에 그립니다.
	 *
	 * @param view 시야 행렬입니다.
	 * @param projection 투영 행렬입니다.
	 * @param positions 화면 상의 3D 점들입니다.
	 * @param color 점들의 색상입니다.
	 *
	 * @note 3D 점들의 개수는 10000개를 넘을 수 없습니다.
	 */
	void RenderConnectPoints3D(const Matrix4x4f& view, const Matrix4x4f& projection, const std::vector<Vector3f>& positions, const Vector4f& color);


	/**
	 * @brief 3D 선을 화면에 그립니다.
	 *
	 * @param view 시야 행렬입니다.
	 * @param projection 투영 행렬입니다.
	 * @param fromPosition 선의 시작점입니다.
	 * @param toPosition 선의 끝점입니다.
	 * @param color 선의 색상입니다.
	 */
	void RenderLine3D(const Matrix4x4f& view, const Matrix4x4f& projection, const Vector3f& fromPosition, const Vector3f& toPosition, const Vector4f& color);


	/**
	 * @brief 3D 선을 화면에 그립니다.
	 *
	 * @param view 시야 행렬입니다.
	 * @param projection 투영 행렬입니다.
	 * @param fromPosition 선의 시작점입니다.
	 * @param fromColor 선의 시작점 색상입니다.
	 * @param toPosition 선의 끝점입니다.
	 * @param toColor 선의 끝점 색상입니다.
	 */
	void RenderLine3D(
		const Matrix4x4f& view,
		const Matrix4x4f& projection,
		const Vector3f& fromPosition,
		const Vector4f& fromColor,
		const Vector3f& toPosition,
		const Vector4f& toColor
	);


	/**
	 * @brief 3D 축 정렬 경계 상자를 화면에 그립니다.
	 * 
	 * @param view 시야 행렬입니다.
	 * @param projection 투영 행렬입니다.
	 * @param minPosition 경계 영역의 최솟값입니다.
	 * @param maxPosition 경계 영역의 최댓값입니다.
	 * @param color 경계 상자의 색상입니다.
	 */
	void RenderAxisAlignedBoundingBox3D(
		const Matrix4x4f& view,
		const Matrix4x4f& projection,
		const Vector3f& minPosition,
		const Vector3f& maxPosition,
		const Vector4f& color
	);
	

	/**
	 * @brief 3D 좌표 공간을 화면에 그립니다.
	 * 
	 * @param view 시야 행렬입니다.
	 * @param projection 투영 행렬입니다.
	 * @param minX X좌표의 최소값입니다.
	 * @param maxX X좌표의 최댓값입니다.
	 * @param strideX X축 방향으로의 간격입니다.
	 * @param minZ Z좌표의 최소값입니다.
	 * @param maxZ Z좌표의 최댓값입니다.
	 * @param strideZ Z축 방향으로의 간격입니다.
	 * @param color 격자의 색상입니다.
	 */
	void RenderGrid3D(const Matrix4x4f& view, const Matrix4x4f& projection, float minX, float maxX, float strideX, float minZ, float maxZ, float strideZ, const Vector4f& color);

	
private:
	/**
	 * @brief 렌더링 처리를 수행하는 매니저에 디폴트 생성자와 빈 가상 소멸자를 삽입합니다.
	 */
	DEFAULT_CONSTRUCTOR_AND_VIRTUAL_DESTRUCTOR(RenderManager);


private:
	/**
	 * @brief 렌더링 대상이 되는 윈도우 포인터입니다.
	 */
	Window* renderTargetWindow_ = nullptr;


	/**
	 * @brief 수직 동기화 활성화 여부입니다. 기본 값은 비활성화입니다.
	 */
	bool bIsEnableVsync_ = false;


	/**
	 * @brief 깊이 테스트 활성화 여부입니다. 기본 값은 활성화입니다.
	 */
	bool bIsEnableDepth_ = true;


	/**
	 * @brief 스텐실 테스트 활성화 여부입니다. 기본 값은 비활성화입니다.
	 */
	bool bIsEnableStencil_ = false;


	/**
	 * @brief 알파 블랜딩 활성화 여부입니다. 기본 값은 활성화입니다.
	 */
	bool bIsEnableAlphaBlend_ = true;


	/**
	 * @brief 렌더링이 수행될 디바이스 컨텍스트의 핸들입니다.
	 */
	HDC deviceContext_ = nullptr;


	/**
	 * @brief 렌더링이 수행될 OpenGL 컨텍스트의 핸들입니다.
	 */
	HGLRC glRenderContext_ = nullptr;


	/**
	 * @brief 셰이더 리소스의 경로입니다.
	 */
	std::wstring shaderPath_;

	
	/**
	 * @brief 렌더 매니저에서 사용할 셰이더 캐시입니다.
	 */
	std::unordered_map<std::wstring, Shader*> shaderCache_;


	/**
	 * @brief 현재 스크린에 대응하는 직교 투영 행렬입니다.
	 */
	Matrix4x4f screenOrtho_;
};
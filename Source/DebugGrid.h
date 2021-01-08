//------------------------------------------------+
// デバッグ用グリッドライン
//------------------------------------------------+
#include "GameMain.h"
#include "Renderer.h"
#include "Shader.h"

class DebugGrid
{

public:

	// コンストラクタ・デストラクタ
	DebugGrid(float in_size, int in_split, Vector3& in_color);
	~DebugGrid();

	void Draw();                     // 描画関数


private:

	bool Initialize();               // 初期化関数


	bool m_isReady;                  // 初期化が完了したか
	unsigned int m_numVerts;         // 頂点数
	unsigned int m_vbo;              // 頂点バッファ
	unsigned int m_vao;              // 頂点配列

	GLfloat* m_vertexData;           // 頂点データ

	Vector3 m_color;                 // グリッドカラー

	class Shader* m_gridShader;      // グリッドライン用シェーダー

};
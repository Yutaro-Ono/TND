//----------------------------------------------------------------------------------------+
//
// GBuffer(座標・法線・アルベド＆スペキュラ)へ描画結果を出力する
//
//----------------------------------------------------------------------------------------+

#pragma once



class GBuffer
{

private:






public:


	unsigned int m_gBuffer;      // G-Buffer (3要素を持つフレームバッファ)

	// G-Bufferに関連付ける各種情報
	unsigned int m_gPos;             // 3D空間座標
	unsigned int m_gNormal;          // 法線ベクトル
	unsigned int m_gAlbedoSpec;      // アルベド(RGB)＆スペキュラ(A)

	unsigned int m_gRBO;             // G-Bufferに紐付ける描画バッファ

};
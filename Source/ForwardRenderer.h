//--------------------------------------------------------------+
// �t�H���[�h�����_�����O�N���X
//--------------------------------------------------------------+
#pragma once



class ForwardRenderer
{

public:

	ForwardRenderer(class Renderer* in_renderer);
	~ForwardRenderer();


	void Draw();


private:

	class Renderer* m_renderer;



};
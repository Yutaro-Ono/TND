//--------------------------------------------------------+
// �˗��l�A�N�^ (�~�b�V���������|�C���g)
//--------------------------------------------------------+
#pragma once
#include "Actor.h"

class ClientActor : public Actor
{

public:

	ClientActor(const Vector3& in_pos);
	~ClientActor();

	void UpdateActor(float in_deltaTime) override;

	//--------------------------------------------------+
	// Getter / Setter
	//--------------------------------------------------+
	bool GetIsSelected() { return m_isSelected; }
	void SetIsSelected(bool in_select) { m_isSelected = in_select; }


private:

	bool m_isSelected;             // ���݃A�N�e�B�u�ȃ~�b�V�����̍��W�Ƃ��Đݒ肳��Ă��邩

	class SpriteComponent* m_landMark;

};
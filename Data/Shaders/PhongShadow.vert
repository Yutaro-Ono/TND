#version 330 core

// attribute
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// ���[���h�ϊ��s��E�r���[�v���W�F�N�V�����s��E���C�g�X�y�[�X
uniform mat4 uWorldTransform;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uViewProj;
uniform mat4 uLightSpaceMatrix;     

// �t���O�����g�ւ̏o��
out vec2 fragTexCoord;          // �e�N�X�`�����W
out vec3 fragNormal;            // ���[���h�X�y�[�X��̖@��
out vec3 fragWorldPos;          // ���[���h�X�y�[�X��̍��W
out vec4 fragPosLightSpace;     // ���C�g�X�y�[�X��̍��W

void main()
{
	vec4 pos = vec4(inPosition, 1.0);
	pos = pos * uWorldTransform;
	fragWorldPos     = pos.xyz;
	gl_Position = pos * uView * uProjection;
	fragNormal      =  (vec4(inNormal, 0.0) * uWorldTransform).xyz;
	fragTexCoord   = inTexCoord;
	fragPosLightSpace = vec4(fragWorldPos, 1.0) * uLightSpaceMatrix;


}

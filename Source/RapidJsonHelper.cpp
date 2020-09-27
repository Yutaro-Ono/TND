#include "RapidJsonHelper.h"

// Json�t�@�C�����J��
// in      : inFileName : �I�[�v��������json�t�@�C���p�X
// inout   : inOutDoc   : rapidjson�I�u�W�F�N�g
bool OpenJsonFile(rapidjson::Document & inOutDoc, const char * in_fileName)
{
	// Json�t�@�C�����J���邩�ǂ���
	std::ifstream file(in_fileName);
	if (!file.is_open())
	{
		return false;
	}

	// fileStream�Ƀt�@�C�����������荞��
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();

	// �t�@�C���������Json������ɕϊ�
	rapidjson::StringStream jsonStr(contents.c_str());
	// �t�@�C���X�g���[������
	file.close();

	// ��̓I�u�W�F�N�g�쐬
	inOutDoc.ParseStream(jsonStr);

	// JSonObject�Ƃ��ēǂ߂����H
	if (!inOutDoc.IsObject())
	{
		return false;
	}
	return true;
}

// Doc���ɂ��̃����o�[�͑��݂��邩
// in  : inDoc        rapidjson Document
//     : memberName   �����o�[��
// �߂�l   Document���� memberName�����݂������̂�true��Ԃ�
bool IsExistMember(const rapidjson::Document & in_doc, const char * in_memberName, const char * in_matchValue)
{
	rapidjson::Value::ConstMemberIterator iter;
	iter = in_doc.FindMember(in_memberName);

	if (iter == in_doc.MemberEnd())
	{
		return false;
	}

	return true;
}

// Doc����memberName�Ƃ������O�̔z��͂��邩
bool IsExistArrayName(const rapidjson::Document & in_doc, const char * in_memberName)
{
	rapidjson::Value::ConstMemberIterator iter;
	iter = in_doc.FindMember(in_memberName);
	if (iter == in_doc.MemberEnd())
	{
		return false;
	}

	return in_doc[in_memberName].IsArray();
}

const int& OutputInt(const rapidjson::Value & in_object, const char * in_id)
{
	// ���͂��ꂽID�����݂��邩���`�F�b�N
	auto iter = in_object.FindMember(in_id);
	if (iter == in_object.MemberEnd())
	{
		return false;
	}

	// Value�^�C�v�̎擾���A�����������ǂ������`�F�b�N
	auto& property = iter->value;
	if (!property.IsInt())
	{
		return false;
	}

	// int���o��
	return property.GetInt();

}

void AddInt(rapidjson::Document::AllocatorType & alloc, rapidjson::Value & in_object, const char * name, int value)
{
	rapidjson::Value v(value);
	in_object.AddMember(rapidjson::StringRef(name), v, alloc);
}

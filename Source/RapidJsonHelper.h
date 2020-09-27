#pragma once

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include "Math.h"


// Json�t�@�C���̃I�[�v��
bool OpenJsonFile(rapidjson::Document & inOutDoc, const char* in_fileName);

// Doc���ɂ��̃����o�[�͑��݂��邩
bool IsExistMember(const rapidjson::Document& in_doc, const char* in_memberName, const char* in_matchValue);

// Doc����memberName�Ƃ������O�̔z��͂��邩�H
bool IsExistArrayName(const rapidjson::Document &in_doc, const char* in_memberName);



// �w�肳�ꂽ�t�@�C����ID�ɕt���������(int�^)���o�͂���
const int& OutputInt(const rapidjson::Value& in_object, const char* in_id);

// int�ɂ��json�t�@�C����������
void AddInt(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& in_object, const char* name, int value);
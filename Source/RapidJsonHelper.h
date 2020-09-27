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


// Jsonファイルのオープン
bool OpenJsonFile(rapidjson::Document & inOutDoc, const char* in_fileName);

// Doc内にそのメンバーは存在するか
bool IsExistMember(const rapidjson::Document& in_doc, const char* in_memberName, const char* in_matchValue);

// Doc中にmemberNameという名前の配列はあるか？
bool IsExistArrayName(const rapidjson::Document &in_doc, const char* in_memberName);



// 指定されたファイルのIDに付随した情報(int型)を出力する
const int& OutputInt(const rapidjson::Value& in_object, const char* in_id);

// intによるjsonファイル書き込み
void AddInt(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& in_object, const char* name, int value);
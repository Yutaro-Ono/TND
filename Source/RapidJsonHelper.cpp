#include "RapidJsonHelper.h"

// Jsonファイルを開く
// in      : inFileName : オープンしたいjsonファイルパス
// inout   : inOutDoc   : rapidjsonオブジェクト
bool OpenJsonFile(rapidjson::Document & inOutDoc, const char * in_fileName)
{
	// Jsonファイルが開けるかどうか
	std::ifstream file(in_fileName);
	if (!file.is_open())
	{
		return false;
	}

	// fileStreamにファイル文字列を取り込む
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();

	// ファイル文字列をJson文字列に変換
	rapidjson::StringStream jsonStr(contents.c_str());
	// ファイルストリーム閉じる
	file.close();

	// 解析オブジェクト作成
	inOutDoc.ParseStream(jsonStr);

	// JSonObjectとして読めたか？
	if (!inOutDoc.IsObject())
	{
		return false;
	}
	return true;
}

// Doc内にそのメンバーは存在するか
// in  : inDoc        rapidjson Document
//     : memberName   メンバー名
// 戻り値   Document内に memberNameが存在した時のみtrueを返す
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

// Doc中にmemberNameという名前の配列はあるか
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
	// 入力されたIDが存在するかをチェック
	auto iter = in_object.FindMember(in_id);
	if (iter == in_object.MemberEnd())
	{
		return false;
	}

	// Valueタイプの取得し、正しい数かどうかをチェック
	auto& property = iter->value;
	if (!property.IsInt())
	{
		return false;
	}

	// intを出力
	return property.GetInt();

}

void AddInt(rapidjson::Document::AllocatorType & alloc, rapidjson::Value & in_object, const char * name, int value)
{
	rapidjson::Value v(value);
	in_object.AddMember(rapidjson::StringRef(name), v, alloc);
}

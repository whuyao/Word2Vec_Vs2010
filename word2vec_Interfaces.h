#ifndef WORD2VEC_INTERFACES_FUNCTIONS_CLASS
#define WORD2VEC_INTERFACES_FUNCTIONS_CLASS

#include "word2vec.h"
#include "word2phrase.h"
#include <QtCore/QList>
#include <QtCore/QString>

//分词文本文件转成词向量文件
//inputfilename: 输入分词文件
//outputfilename: 输出词向量文件
//params: 参数列表
int WORD2VEC(char* inputfilename, char* outputfilename, W2VParameters params);

//分词文本文件转成短语文件
//inputfilename: 输入分词文件
//outputfilename: 输出词向量文件
//params: 参数列表
int WORD2PHRASE(char* inputfilename, char* outputfilename, W2PParameters params);


struct ClosestWords
{
	QString sword;
	float fdistance;
};

typedef QList<ClosestWords> WordsList;

class WVDistanceClass
{
public:
	WVDistanceClass();
	~WVDistanceClass();

public:
	//载入word2vec训练后的文件
	//file_name: 单词向量文件
	bool loadVectorsFile(const char* file_name);

	//获取距离最近单词
	//sinputWords:输入单词，用空格隔开
	//vcws: 输出结果保存在QList中
	//isStdOut: 是否屏幕输出
	//max_cloest_words_num: 查找出最多单词数
	bool getClosestWords(char* sinputWords, WordsList& vcws, bool isStdOut = false, int max_closest_words_num = 100);	//用空格隔开

	//语义迁移
	//sinputWords:输入单词，用空格隔开（必须为3个，如“男人 男孩 女人”）
	//vcws: 输出结果保存在QList中
	//isStdOut: 是否屏幕输出
	//max_cloest_words_num: 查找出最多单词数
	bool semanticMigrate(char* sinputWords, WordsList& vcws, bool isStdOut = false, int max_closest_words_num = 100);	//用空格隔开

	//清空内存，复位
	void reset();

protected:
	char* vocab;
	float* M;
	long long words, size;

private:
	int max_w;		//max length of vocabulary entries
	int max_size;	//max length of strings
	
private:
	int N;			//number of closests words that will be show
};






#endif
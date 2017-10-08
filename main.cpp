
#include <QtCore>
#include "word2vec_Interfaces.h"
#include <iostream>
using namespace std;

// #include "word2vec.h"
// #include "distance.h"
//#include "word_analogy.h"

//-train questions-words.txt -output vec.bin -size 200 -window 5 -sample 1e-4 -negative 5 -hs 0 -binary 0 -cbow 1 -iter 3
//-train Result_GNSS.txt -output vec.bin -size 200 -window 5 -sample 1e-4 -negative 5 -hs 0 -binary 0 -cbow 1 -iter 3 -min-count 2
//-train sogounews_lingdata.txt -output vec.bin -size 200 -window 5 -sample 1e-4 -negative 5 -hs 0 -binary 0 -cbow 1 -iter 3 -min-count 5
//GOOGLE_WORD2VEC::word2vec_display(argc, argv);

//vec.bin
//GOOGLE_WORD2VEC::distance_display(argc, argv);
//GOOGLE_WORD2VEC::word_analogy_display(argc, argv);

void testInputFun()
{
	WVDistanceClass wdc;
	wdc.loadVectorsFile("C:\\Users\\YAO\\Desktop\\GZLandUse\\2011_2016_POI_Corpus\\2011_2016_POI语料\\POIVec_result\\2011_2016_Method2_Corpus_s200_w5.bin");	//载入词向量文件
	WordsList vcws;	//结果保存于此
	char* str;	//输入数据

	while (1)
	{
		//计算词向量最近距离
		while (1)
		{
			cout<<"closest words detection. please input several words: ";
			gets(str);

			if(QString(str)=="EXIT") break;

			wdc.getClosestWords(str, vcws, false, 10);	
			foreach(ClosestWords cw, vcws)
			{
				qDebug()<<cw.sword<<", "<<cw.fdistance;
			}
		}


		//语义迁移
		while (1)
		{
			cout<<"sematic migaration. please input 3 words: ";
			gets(str);

			if(QString(str)=="EXIT") break;

			wdc.semanticMigrate(str, vcws, false, 10);	//获取距离（多个单词），支持中文
			foreach(ClosestWords cw, vcws)
			{
				qDebug()<<cw.sword<<", "<<cw.fdistance;
			}

		}

	}
}

int main(int argc, char *argv[])
{
	//set code for locale
	QTextCodec* codec =QTextCodec::codecForLocale();
	QTextCodec::setCodecForCStrings(codec);
	QTextCodec::setCodecForTr(codec);

	testInputFun();
	return 0;
	
	//默认参数
	W2VParameters wvparams;
	wvparams.nWordVectorSize = 100;
	wvparams.nMaxSkipLength = 5;
	wvparams.fThreadsholdOccurence = 1e-4;
	wvparams.nNegativeSamples = 5;
	wvparams.nThreadNum = 8;
	wvparams.nIterations = 5;
	wvparams.nMinAppearCount = 5;	//单词出现最小频率
	wvparams.nClasses = 0;			//不为0则聚类
	wvparams.nDebugMode = 2;
	wvparams.nBinary = 0; //输出文本文件而不是二进制文件
	wvparams.nHierachicalSofemax = 0;
	wvparams.ncbow = 1;
	wvparams.fAlpha = 0.05;

	//训练分词文件，形成单词向量文件
	WORD2VEC("./data/questions-words.txt", "./data/vec.txt", wvparams);	//训练，支持中文语料


	//计算词向量最近距离
	WVDistanceClass wdc;
	wdc.loadVectorsFile("./data/vec.txt");	//载入词向量文件
	WordsList vcws;	//结果保存于此
	wdc.getClosestWords("California Texas Florida", vcws, false, 100);	//获取距离（多个单词），支持中文
	foreach(ClosestWords cw, vcws)
	{
		qDebug()<<cw.sword<<", "<<cw.fdistance;
	}

	wdc.getClosestWords("California", vcws, false, 100);	//获取距离（一个单词），支持中文
	foreach(ClosestWords cw, vcws)
	{
		qDebug()<<cw.sword<<", "<<cw.fdistance;
	}

	wdc.getClosestWords("China", vcws, false, 100);		//获取距离（不存在单词），支持中文
	foreach(ClosestWords cw, vcws)
	{
		qDebug()<<cw.sword<<", "<<cw.fdistance;
	}

	//语义迁移
	wdc.semanticMigrate("California Texas Florida", vcws, false, 100);	//获取距离（多个单词），支持中. e.g. 广东 广州 湖北
	foreach(ClosestWords cw, vcws)
	{
		qDebug()<<cw.sword<<", "<<cw.fdistance;
	}


	//词间聚类
	wvparams.nClasses = 5;			//不为0则聚类，其他参数一样
	WORD2VEC("./data/questions-words.txt", "./data/cluster.txt", wvparams);

	return 0;
}

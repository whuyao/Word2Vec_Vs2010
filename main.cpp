
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
	wdc.loadVectorsFile("C:\\Users\\YAO\\Desktop\\GZLandUse\\2011_2016_POI_Corpus\\2011_2016_POI����\\POIVec_result\\2011_2016_Method2_Corpus_s200_w5.bin");	//����������ļ�
	WordsList vcws;	//��������ڴ�
	char* str;	//��������

	while (1)
	{
		//����������������
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


		//����Ǩ��
		while (1)
		{
			cout<<"sematic migaration. please input 3 words: ";
			gets(str);

			if(QString(str)=="EXIT") break;

			wdc.semanticMigrate(str, vcws, false, 10);	//��ȡ���루������ʣ���֧������
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
	
	//Ĭ�ϲ���
	W2VParameters wvparams;
	wvparams.nWordVectorSize = 100;
	wvparams.nMaxSkipLength = 5;
	wvparams.fThreadsholdOccurence = 1e-4;
	wvparams.nNegativeSamples = 5;
	wvparams.nThreadNum = 8;
	wvparams.nIterations = 5;
	wvparams.nMinAppearCount = 5;	//���ʳ�����СƵ��
	wvparams.nClasses = 0;			//��Ϊ0�����
	wvparams.nDebugMode = 2;
	wvparams.nBinary = 0; //����ı��ļ������Ƕ������ļ�
	wvparams.nHierachicalSofemax = 0;
	wvparams.ncbow = 1;
	wvparams.fAlpha = 0.05;

	//ѵ���ִ��ļ����γɵ��������ļ�
	WORD2VEC("./data/questions-words.txt", "./data/vec.txt", wvparams);	//ѵ����֧����������


	//����������������
	WVDistanceClass wdc;
	wdc.loadVectorsFile("./data/vec.txt");	//����������ļ�
	WordsList vcws;	//��������ڴ�
	wdc.getClosestWords("California Texas Florida", vcws, false, 100);	//��ȡ���루������ʣ���֧������
	foreach(ClosestWords cw, vcws)
	{
		qDebug()<<cw.sword<<", "<<cw.fdistance;
	}

	wdc.getClosestWords("California", vcws, false, 100);	//��ȡ���루һ�����ʣ���֧������
	foreach(ClosestWords cw, vcws)
	{
		qDebug()<<cw.sword<<", "<<cw.fdistance;
	}

	wdc.getClosestWords("China", vcws, false, 100);		//��ȡ���루�����ڵ��ʣ���֧������
	foreach(ClosestWords cw, vcws)
	{
		qDebug()<<cw.sword<<", "<<cw.fdistance;
	}

	//����Ǩ��
	wdc.semanticMigrate("California Texas Florida", vcws, false, 100);	//��ȡ���루������ʣ���֧����. e.g. �㶫 ���� ����
	foreach(ClosestWords cw, vcws)
	{
		qDebug()<<cw.sword<<", "<<cw.fdistance;
	}


	//�ʼ����
	wvparams.nClasses = 5;			//��Ϊ0����࣬��������һ��
	WORD2VEC("./data/questions-words.txt", "./data/cluster.txt", wvparams);

	return 0;
}

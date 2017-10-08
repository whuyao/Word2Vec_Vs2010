#ifndef WORD2VEC_INTERFACES_FUNCTIONS_CLASS
#define WORD2VEC_INTERFACES_FUNCTIONS_CLASS

#include "word2vec.h"
#include "word2phrase.h"
#include <QtCore/QList>
#include <QtCore/QString>

//�ִ��ı��ļ�ת�ɴ������ļ�
//inputfilename: ����ִ��ļ�
//outputfilename: ����������ļ�
//params: �����б�
int WORD2VEC(char* inputfilename, char* outputfilename, W2VParameters params);

//�ִ��ı��ļ�ת�ɶ����ļ�
//inputfilename: ����ִ��ļ�
//outputfilename: ����������ļ�
//params: �����б�
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
	//����word2vecѵ������ļ�
	//file_name: ���������ļ�
	bool loadVectorsFile(const char* file_name);

	//��ȡ�����������
	//sinputWords:���뵥�ʣ��ÿո����
	//vcws: ������������QList��
	//isStdOut: �Ƿ���Ļ���
	//max_cloest_words_num: ���ҳ���൥����
	bool getClosestWords(char* sinputWords, WordsList& vcws, bool isStdOut = false, int max_closest_words_num = 100);	//�ÿո����

	//����Ǩ��
	//sinputWords:���뵥�ʣ��ÿո����������Ϊ3�����硰���� �к� Ů�ˡ���
	//vcws: ������������QList��
	//isStdOut: �Ƿ���Ļ���
	//max_cloest_words_num: ���ҳ���൥����
	bool semanticMigrate(char* sinputWords, WordsList& vcws, bool isStdOut = false, int max_closest_words_num = 100);	//�ÿո����

	//����ڴ棬��λ
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
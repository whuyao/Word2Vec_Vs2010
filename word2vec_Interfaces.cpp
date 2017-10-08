#include "word2vec_Interfaces.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

#include <iostream>
using namespace std;

int WORD2VEC( char* inputfilename, char* outputfilename, W2VParameters params )
{
	return GOOGLE_WORD2VEC::word2vec(inputfilename, outputfilename, params);
}

int WORD2PHRASE( char* inputfilename, char* outputfilename, W2PParameters params )
{
	return GOOGLE_WORD2PHRASE::word2phrase(inputfilename, outputfilename, params);
}

WVDistanceClass::WVDistanceClass()
{
	vocab = NULL;
	M = NULL;
	max_w = 50;
	max_size = 2000;
	N = 50;
	//bestw = NULL;
	//bestd = NULL;
	words = 0;
	size = 0;
}

WVDistanceClass::~WVDistanceClass()
{
	reset();
}

void WVDistanceClass::reset()
{
	if (vocab!=NULL)
		delete []vocab;
	vocab = NULL;

	if (M!=NULL)
		delete []M;
	M=NULL;

	max_w = 50;
	max_size = 2000;
	N = 50;
	words = 0;
	size = 0;
}


bool WVDistanceClass::loadVectorsFile( const char* file_name )
{
	float len;
	long long a, b;

	FILE *f;
	f = fopen(file_name, "rb");
	if (f == NULL) {
		printf("Input file not found\n");
		return false;
	}

// 	if (vocab!=NULL)
// 		delete []vocab;
// 	vocab = NULL;
// 
// 	if (M!=NULL)
// 		delete []M;
// 	M=NULL;

	reset();	

	fscanf(f, "%lld", &words);
	fscanf(f, "%lld", &size);
	vocab = new char[(long long)words * max_w ];//(char *)malloc((long long)words * max_w * sizeof(char));
	
	M = new float[(long long)words * (long long)size];//(float *)malloc((long long)words * (long long)size * sizeof(float));
	if (M == NULL) {
		printf("Cannot allocate memory: %lld MB    %lld  %lld\n", (long long)words * size * sizeof(float) / 1048576, words, size);
		return false;
	}
	for (b = 0; b < words; b++) {
		a = 0;
		while (1) {
			vocab[b * max_w + a] = fgetc(f);
			if (feof(f) || (vocab[b * max_w + a] == ' ')) break;
			if ((a < max_w) && (vocab[b * max_w + a] != '\n')) a++;
		}
		vocab[b * max_w + a] = 0;
		for (a = 0; a < size; a++) 
		{
			//fread(&M[a + b * size], sizeof(float), 1, f);
			fscanf(f, "%f", &M[a + b * size]);
		}
		fgetc(f);	//句尾空格

		len = 0;
		for (a = 0; a < size; a++) len += M[a + b * size] * M[a + b * size];
		len = sqrt(len);
		for (a = 0; a < size; a++) M[a + b * size] /= len;
	}
	fclose(f);

	cout<<"load dataset success. filename: "<<file_name<<endl;
	return true;
}

bool WVDistanceClass::getClosestWords( char* sinputWords, WordsList& vcws, bool isStdOut, int max_closest_words_num  )
{
	if (M==NULL)
	{
		cout<<"please load training word vectors dataset!"<<endl;
		return false;
	}

	N = max_closest_words_num;
	vcws.clear();

	long long a, b, c, d, cn, bi[100];
	//char* st1 = new char[max_size];
	char* st[100];
	for (a=0;a<100;a++) st[a] = new char[max_size];
	char ch;
	float* vec=new float[max_size];
	float dist, len;

	//开辟空间
	char** bestw = new char*[N];
	for (a = 0; a < N; a++) bestw[a] = new char[max_size];
	float* bestd = new float[N];

	//开始处理
	for (a = 0; a < N; a++) bestd[a] = 0;
	for (a = 0; a < N; a++) bestw[a][0] = 0;
	a = 0;
	cn = 0;
	b = 0;
	c = 0;
	while (1) {
		st[cn][b] = sinputWords[c];
		b++;
		c++;
		st[cn][b] = 0;
		if (sinputWords[c] == 0) break;
		if (sinputWords[c] == ' ') {
			cn++;
			b = 0;
			c++;
		}
	}
	cn++;	//最后一个单词加1
		
	for (a = 0; a < cn; a++) {
		for (b = 0; b < words; b++) if (!strcmp(&vocab[b * max_w], st[a])) break;
		if (b == words) b = -1;
		bi[a] = b;
		if(isStdOut)
			printf("\nWord: %s  Position in vocabulary: %lld\n", st[a], bi[a]);
		if (b == -1) {
			printf("Out of dictionary word!\n");
			break;
		}
	}
	if (b == -1) 
	{
		//delete []st1;

		for (a=0; a<100; a++)
			delete []st[a];

		if (bestw!=NULL)
		{
			for (int i=0; i<N; i++)
				delete []bestw[i];
			delete []bestw;
			bestw = NULL;
		}

		if (bestd!=NULL)
			delete []bestd;
		bestd = NULL;

		delete []vec;

		return false;
	}

	if (isStdOut)
		printf("\n                                              Word       Cosine distance\n------------------------------------------------------------------------\n");
	
	for (a = 0; a < size; a++) vec[a] = 0;
	for (b = 0; b < cn; b++) {
		if (bi[b] == -1) continue;
		for (a = 0; a < size; a++) vec[a] += M[a + bi[b] * size];
	}

	//计算余弦
	len = 0;
	for (a = 0; a < size; a++) len += vec[a] * vec[a];
	len = sqrt(len);
	for (a = 0; a < size; a++) vec[a] /= len;
	for (a = 0; a < N; a++) bestd[a] = -1;
	for (a = 0; a < N; a++) bestw[a][0] = 0;
	for (c = 0; c < words; c++) {
		a = 0;

		for (b = 0; b < cn; b++) if (bi[b] == c) a = 1;
		if (a == 1) continue;

		dist = 0;
		for (a = 0; a < size; a++) dist += vec[a] * M[a + c * size];
		for (a = 0; a < N; a++) {
			if (dist > bestd[a]) {
				for (d = N - 1; d > a; d--) {
					bestd[d] = bestd[d - 1];
					strcpy(bestw[d], bestw[d - 1]);
				}
				bestd[a] = dist;
				strcpy(bestw[a], &vocab[c * max_w]);
				break;
			}
		}
	}

	//记录并输出
	for (a = 0; a < N; a++) 
	{
		if (isStdOut)					
			printf("%50s\t\t%f\n", bestw[a], bestd[a]);

		ClosestWords wrd;
		wrd.sword = QString(bestw[a]).trimmed();
		wrd.fdistance = bestd[a];

		if (!wrd.sword.isEmpty())
			vcws.append(wrd);
		
	}
		


	//释放内存
	//delete []st1;

	for (a=0; a<100; a++)
		delete []st[a];
	
	if (bestw!=NULL)
	{
		for (int i=0; i<N; i++)
			delete []bestw[i];
		delete []bestw;
		bestw = NULL;
	}

	if (bestd!=NULL)
		delete []bestd;
	bestd = NULL;

	delete []vec;

	return true;

}

bool WVDistanceClass::semanticMigrate( char* sinputWords, WordsList& vcws, bool isStdOut /*= false*/, int max_closest_words_num /*= 100*/ )
{
	if (M==NULL)
	{
		cout<<"please load training word vectors dataset!"<<endl;
		return false;
	}

	N = max_closest_words_num;
	vcws.clear();

	long long a, b, c, d, cn, bi[100];
	//char* st1 = new char[max_size];
	char* st[100];
	for (a=0;a<100;a++) st[a] = new char[max_size];
	char ch;
	float* vec=new float[max_size];
	float dist, len;

	//开辟空间
	char** bestw = new char*[N];
	for (a = 0; a < N; a++) bestw[a] = new char[max_size];
	float* bestd = new float[N];


	for (a = 0; a < N; a++) bestd[a] = 0;
	for (a = 0; a < N; a++) bestw[a][0] = 0;
	a = 0;
	cn = 0;
	b = 0;
	c = 0;
	while (1) {
		st[cn][b] = sinputWords[c];
		b++;
		c++;
		st[cn][b] = 0;
		if (sinputWords[c] == 0) break;
		if (sinputWords[c] == ' ') {
			cn++;
			b = 0;
			c++;
		}
	}
	cn++;
	if (cn < 3) {
		printf("Only %lld words were entered.. three words are needed at the input to perform the calculation\n", cn);
		
		for (a=0; a<100; a++)
			delete []st[a];

		if (bestw!=NULL)
		{
			for (int i=0; i<N; i++)
				delete []bestw[i];
			delete []bestw;
			bestw = NULL;
		}

		if (bestd!=NULL)
			delete []bestd;
		bestd = NULL;

		delete []vec;

		return false;
	}

	/////////////////////////////////
	for (a = 0; a < cn; a++) {
		for (b = 0; b < words; b++) if (!strcmp(&vocab[b * max_w], st[a])) break;
		if (b == words) b = 0;
		bi[a] = b;
		if(isStdOut)
			printf("\nWord: %s  Position in vocabulary: %lld\n", st[a], bi[a]);
		if (b == 0) {
			printf("Out of dictionary word!\n");
			break;
		}
	}
	if (b == 0) 
	{
		for (a=0; a<100; a++)
			delete []st[a];

		if (bestw!=NULL)
		{
			for (int i=0; i<N; i++)
				delete []bestw[i];
			delete []bestw;
			bestw = NULL;
		}

		if (bestd!=NULL)
			delete []bestd;
		bestd = NULL;

		delete []vec;
		return false;
	}

	if (isStdOut)	
		printf("\n                                              Word              Distance\n------------------------------------------------------------------------\n");
	
	for (a = 0; a < size; a++) vec[a] = M[a + bi[1] * size] - M[a + bi[0] * size] + M[a + bi[2] * size];
	len = 0;
	for (a = 0; a < size; a++) len += vec[a] * vec[a];
	len = sqrt(len);
	for (a = 0; a < size; a++) vec[a] /= len;
	for (a = 0; a < N; a++) bestd[a] = 0;
	for (a = 0; a < N; a++) bestw[a][0] = 0;
	for (c = 0; c < words; c++) {
		if (c == bi[0]) continue;
		if (c == bi[1]) continue;
		if (c == bi[2]) continue;
		a = 0;
		for (b = 0; b < cn; b++) if (bi[b] == c) a = 1;
		if (a == 1) continue;
		dist = 0;
		for (a = 0; a < size; a++) dist += vec[a] * M[a + c * size];
		for (a = 0; a < N; a++) {
			if (dist > bestd[a]) {
				for (d = N - 1; d > a; d--) {
					bestd[d] = bestd[d - 1];
					strcpy(bestw[d], bestw[d - 1]);
				}
				bestd[a] = dist;
				strcpy(bestw[a], &vocab[c * max_w]);
				break;
			}
		}
	}

	for (a = 0; a < N; a++) 
	{
		if (isStdOut)
			printf("%50s\t\t%f\n", bestw[a], bestd[a]);

		ClosestWords wrd;
		wrd.sword = QString(bestw[a]).trimmed();
		wrd.fdistance = bestd[a];

		if (!wrd.sword.isEmpty())
			vcws.append(wrd);
	}


	//
	for (a=0; a<100; a++)
		delete []st[a];

	if (bestw!=NULL)
	{
		for (int i=0; i<N; i++)
			delete []bestw[i];
		delete []bestw;
		bestw = NULL;
	}

	if (bestd!=NULL)
		delete []bestd;
	bestd = NULL;

	delete []vec;

	return true;



}


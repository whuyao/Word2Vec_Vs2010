#ifndef WORD2VEC_FUNCTION_GOOGLE
#define WORD2VEC_FUNCTION_GOOGLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct vocab_word {
	long long cn;
	int *point;
	char *word, *code, codelen;
};

struct W2VParameters
{
	int nWordVectorSize;			//-size, 100-200, default=100
	int nMaxSkipLength;				//-window, 3-5, default=5
	float fThreadsholdOccurence;	//-sample, (0, 1e-5), default=1e-4

	int nNegativeSamples;			//-negative, 3-10, 0=not used, default = 5
	int nThreadNum;					//-threads, default = 12
	int nIterations;				//-iter, run more training iterations, default = 5
	int nMinAppearCount;			//-min-count, This will discard words that appear less than <int> times; default is 5
	
	int nClasses;					//-classes, Output word classes rather than word vectors; default number of classes is 0 (vectors are written)
	int nDebugMode;					//-debug, Set the debug mode (default = 2 = more info during training)
	int nBinary;					//-binrary, Save the resulting vectors in binary moded; default is 0

	int nHierachicalSofemax;		//-hs, Use Hierarchical Softmax; default is 0 (not used)
	int ncbow;						//-cbow, default=1, else use 0 for skip-gram model
	float fAlpha;					//-alpha, learning rate, default: skip-gram = 0.025, cbow = 0.05
};

namespace GOOGLE_WORD2VEC
{

	void InitUnigramTable();

	// Reads a single word from a file, assuming space + tab + EOL to be word boundaries
	void ReadWord(char *word, FILE *fin) ;

	// Returns hash value of a word
	int GetWordHash(char *word);

	// Returns position of a word in the vocabulary; if the word is not found, returns -1
	int SearchVocab(char *word);

	// Reads a word and returns its index in the vocabulary
	int ReadWordIndex(FILE *fin);

	// Adds a word to the vocabulary
	int AddWordToVocab(char *word);

	// Used later for sorting by word counts
	int VocabCompare(const void *a, const void *b);

	// Sorts the vocabulary by frequency using word counts
	void SortVocab();

	// Reduces the vocabulary by removing infrequent tokens
	void ReduceVocab();

	// Create binary Huffman tree using the word counts
	// Frequent words will have short uniqe binary codes
	void CreateBinaryTree() ;

	void LearnVocabFromTrainFile();

	void SaveVocab();

	void ReadVocab();

	void InitNet() ;

	void *TrainModelThread(void *id);

	void TrainModel();

	int ArgPos(char *str, int argc, char **argv);

	int word2vec_display(int argc, char **argv);

	int word2vec(char* inputfilename, char* outputfilename, W2VParameters params);

}










#endif
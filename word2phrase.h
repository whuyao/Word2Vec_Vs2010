#ifndef WORD2PHRASE_FUNCTION_GOOGLE
#define WORD2PHRASE_FUNCTION_GOOGLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct vocab_phrase {
	long long cn;
	char *word;
};

struct W2PParameters
{
	int nMinAppearCount;			//-min-count, This will discard words that appear less than <int> times; default is 5
	int nDebugMode;					//-debug, Set the debug mode (default = 2 = more info during training)
	float fThreshold;				//-threshold, represents threshold for forming the phrases (higher means less phrases); default=100
};

namespace GOOGLE_WORD2PHRASE
{
	// Reads a single word from a file, assuming space + tab + EOL to be word boundaries
	void ReadWord(char *word, FILE *fin);

	// Returns hash value of a word
	int GetWordHash(char *word) ;

	// Returns position of a word in the vocabulary; if the word is not found, returns -1
	int SearchVocab(char *word);

	// Reads a word and returns its index in the vocabulary
	int ReadWordIndex(FILE *fin);

	// Adds a word to the vocabulary
	int AddWordToVocab(char *word);

	// Used later for sorting by word counts
	int VocabCompare(const void *a, const void *b);

	// Sorts the vocabulary by frequency using word counts
	void SortVocab() ;

	// Reduces the vocabulary by removing infrequent tokens
	void ReduceVocab() ;

	void LearnVocabFromTrainFile() ;

	void TrainModel() ;

	int ArgPos(char *str, int argc, char **argv) ;

	int word2phrase_display(int argc, char **argv);


	int word2phrase(char* inputfilename, char* outputfilename, W2PParameters params);

}

#endif
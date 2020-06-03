/**
* Ryan Huett
* Date: 12/02/2018
* Visual Studio 2017
* Class Hasher that collects a list of up to 40 words from a text file,
* then sorts them into a hashed list using DJB2 hashing algorithm. Can 
* also display the results of the algorithm once sorted.
*/
#pragma once
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

const string FILE_PATH = "dictionary1.txt";
const int MAX_WORDS = 40;
const int HASH_SIZE = 10;	// Gave the smallest array size vs collisions
							// during testing when compared to nearest primes.

class Hasher {
	struct wordNode {
		string *word;
		wordNode *next;
	};
	wordNode *dictionary[HASH_SIZE];

	string wordList[MAX_WORDS];
	int totalWords;
	int extraWords;
	bool dictionaryIsCreated;

	void displayMenu();
	void readFromFile();
	void displayWordList();
	void createDictionary();
	int hashWord(int wordListIndex);
	void displayDictionary();

public:
	Hasher();
};
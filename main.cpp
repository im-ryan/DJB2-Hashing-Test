/**
* Ryan Huett
* Date: 12/02/2018
* Visual Studio 2017
* The driver for class Hasher that includes a simple UI as well as
* input validation. Also contains class Hasher's member functions.
*/
#include "Hasher.h"

int main() {
	Hasher hasher;
	return 0;
}

Hasher::Hasher() {
	for (int i = 0; i < HASH_SIZE; i++) {
		wordList[i] = "";
		dictionary[i] = nullptr;
	}

	totalWords = 0;
	extraWords = 0;

	dictionaryIsCreated = false;

	displayMenu();
}

void Hasher::displayMenu() {
	char input = NULL;
	bool userWantsToGoAgain = true;

	do {
		system("CLS");

		cout << "::Main Menu::\n\n"
			<< "[C]reate a hashed dictionary\n";
		if (dictionaryIsCreated) {
			cout << "[D]isplay all collected words\n"
				<< "[H]ash algorithm results\n";
		}
		cout << "[Q]uit the program\n\n"
			<< "Choice: ";

		cin.get(input);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		system("CLS");
		if (input == 'C' || input == 'c') {
			readFromFile();
		}
		else if ( (input == 'D' || input == 'd') && dictionaryIsCreated ) {
			displayWordList();
		}
		else if ( (input == 'H' || input == 'h') && dictionaryIsCreated ) {
			displayDictionary();
		}
		else if (input == 'Q' || input == 'q') {
			userWantsToGoAgain = false;
			cout << "::Exit::\n\nGoodbye!" << endl;
		}
		else {
			system("CLS");
			cout << "::Error::\n" << endl
				<< "Unrecognized selection. Please try again.\n" << endl;
		}
		cout << endl;
		system("PAUSE");

	} while (userWantsToGoAgain);
}

void Hasher::readFromFile() {
	string line;
	ifstream file(FILE_PATH);
	int count = 0;

	// Make sure the file exists
	if (file.is_open())
	{
		cout << "::Log::\n" << endl;
		cout << "Reading all words from " << FILE_PATH << "..." << endl;

		// While there is a next line
		while (getline(file, line))
		{

			// Make sure the word list is the expected size
			if (count >= MAX_WORDS) {
				extraWords++;
			}
			else {

				// Don't collect blank lines
				if (line != "") {
					wordList[count] = line;
					count++;
				}

			}

		} // end while loop

		totalWords = count;

		// If there are any extra words, warn the user.
		if (extraWords > 0) {
			cout << "--Warning: " 
				<< "The last " << extraWords << " words will not be used in the current hash.\n" 
				<< "Please make sure your list of words is less than " << HASH_SIZE << " words long.\n" << endl;
		}

		cout << "Success! A total number of " << count << " words have been read." << endl;
		dictionaryIsCreated = true;
		file.close();

		// Hash the collected words
		createDictionary();
	}
	else {
		system("CLS");
		cout << "::File Error::\n\n"
			<< "Unable to open file." << endl;
	}
}

void Hasher::displayWordList() {

	cout << "::All Collected Words::\n" << endl;
	for (int i = 0; i < totalWords; i++) {
		cout << (i + 1) << ". " << wordList[i] << endl;
	}

}

void Hasher::createDictionary() {
	wordNode *currWord = nullptr, *walkerNode = nullptr;
	int hash = 0;

	cout << "\nHashing all collected words..." << endl;
	for (int i = 0; i < totalWords; i++) {
		hash = hashWord(i); // Get hash of the current word

		// Create new word to append to dictionary
		currWord = new wordNode;
		currWord->word = &wordList[i];
		currWord->next = nullptr;

		// Decide where to append in the dictionary
		if (!dictionary[hash]) {
			dictionary[hash] = currWord;
		}
		else {
			walkerNode = dictionary[hash];

			while (walkerNode->next) {
				walkerNode = walkerNode->next;
			}

			walkerNode->next = currWord;
		}
	}
	cout << "Success! Hashing process is complete." << endl;

}

// DJB2 Hashing Algorithm
int Hasher::hashWord(int index) {
	string word = wordList[index];
	long hash = 5381;

	for (char c : word) {
		hash = (hash << 5) + hash + c;
	}

	return abs(hash % HASH_SIZE);
}

void Hasher::displayDictionary() {
	wordNode *walkerNode = nullptr;
	int subListPos = 1, wordsUsed = 0, maxCollisions = 0,
		unusedIndexes = 0, totalCollisions = 0;

	cout << "::Hash Algorithm Results::\n" << endl;
	cout << "\n\t--Generated View of Hash Storage--\n" << endl;
	for (int i = 0; i < HASH_SIZE; i++) {

		// Display all dictionary indexes
		if (dictionary[i]) {
			cout << "Hash " << i << ": " << *dictionary[i]->word << endl;
			wordsUsed++;

			// Check if any collisions exist
			if (dictionary[i]->next) {
				walkerNode = dictionary[i]->next;
				subListPos = 0;

				// Display any collisions in a sub list
				while (walkerNode) {
					cout << "\t Node " << ++subListPos << " - " << *walkerNode->word << endl;
					wordsUsed++;

					walkerNode = walkerNode->next;
				}

				// Keep a running tally of total & max collisions
				if (subListPos > maxCollisions) {
					maxCollisions = subListPos;
				}
				totalCollisions += subListPos;
			} // end collision check
		}
		else {
			cout << "Hash " << i << ". --NULL--" << endl;
			unusedIndexes++;
		}

	} // end for loop
	cout << "\n\t-- End Generated View--\n\n" << endl;

	cout << "\n\t--Generated Hash Algorithm Information--\n" << endl;
	cout << "All words been used in this hash:        " << ((wordsUsed == totalWords) ? "Yes" : "No") << endl;
	cout << "Size of array used for this hash:        " << HASH_SIZE << endl;
	cout << "Total amount of collisions in this hash: " << totalCollisions << endl;
	cout << "Max collisions for this hash:            " << maxCollisions << endl;
	cout << "Unused indexes in this hash:             " << unusedIndexes << endl;
	cout << "\n\t--End Algorithm Information--\n" << endl;
}
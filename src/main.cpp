#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <set>
#include <iomanip> // setw


#include "dictionary.h"

using namespace std;

void lower(string& s);
string stripPunct(const string& s);
void checkSpelling(ifstream& in, Dictionary& dict);

void findTransp(std::string& word, Dictionary* dictionary, set<string>* result);
void findDelete(std::string& word, Dictionary* dictionary, set<string>* result);
void findReplace(std::string& word, Dictionary* dictionary, set<string>* result);
void findInsert(std::string& word, Dictionary* dictionary, set<string>* result);

void outputSuggestions(set<string>* result, std::string indent);

// program arguments to run, example: main.exe ../../res/wordlist.txt ../../res/test.txt
int main1(int argc, char* argv[]) {
	// Output usage message if improper command line args were given.
	if (argc != 3)
    {
		cerr << "Usage: " << argv[0] << " wordlist_filename input_file\n";
		return EXIT_FAILURE;
	}

	ifstream inf(argv[2]);
	if (! inf)
    {
		cerr << "Could not open " << argv[2] << "\n";
		return EXIT_FAILURE;
	}

	// Read dictionary, but let user know what we are working on.
	cout << "Loading dictionary, this may take awhile...\n";

	Dictionary d(argv[1]);

	checkSpelling(inf, d);

	inf.close();

	return EXIT_SUCCESS;
}

int main()
{
    Dictionary dictionary;
    dictionary.insert("maxim");
    dictionary.insert("maxim");

    cout << dictionary.size() << endl;

    dictionary.remove("maxim");

    cout << dictionary.size() << endl;
}

void checkSpelling(ifstream& in, Dictionary& dict) {

	int line_number = 0;

	while (in) 
    {
		line_number++;
		set<string> totest;
		string line;
		getline(in, line);

  	    stringstream ss (stringstream::in | stringstream::out);
		ss << line;

		string word;
		while (ss >> word) 
        {
            lower(word);
            word = stripPunct(word);

            if(dict.search(word))
                continue;

            printf("line %d: '%s'\r\n", line_number, word.c_str());
            cout << "\tsuggestions:\r\n";

            set<string> suggestions;
            findTransp(word, &dict, &suggestions);
            findDelete(word, &dict, &suggestions);
            findReplace(word, &dict, &suggestions);
            findInsert(word, &dict, &suggestions);

            outputSuggestions(&suggestions, "\t\t");
		}
	}
}

void lower(string& s) {

    // Ensures that a word is lowercase
	for (int i = 0; i < s.length(); i++)
    {
		s[i] = tolower(s[i]);
	}
}

string stripPunct(const string& s) {

	// Remove any single trailing
	// punctuation character from a word.  
	if (ispunct(s[s.length() - 1]) ) 
    {
		return s.substr (0, s.length() - 1);
	}
	else 
    {
		return s;
	}
}

void findTransp(std::string& word, Dictionary* dictionary, set<string>* result)
{
    for(int i = 0; i < word.size() - 1; i++)
    {
        string transpWord = word;
        transpWord[i] = word[i + 1];
        transpWord[i + 1] = word[i];
        if(dictionary->search(transpWord))
            result->insert(transpWord);
    }
}

void findDelete(std::string& word, Dictionary* dictionary, set<string>* result)
{
    for(int i = 0; i < word.size(); i++)
    {
        string remWord = word.substr(0, i) + word.substr(i + 1, word.size() - 1);
        if(dictionary->search(remWord))
            result->insert(remWord);
    }
}

void findReplace(std::string& word, Dictionary* dictionary, set<string>* result)
{
    for(int i = 0; i < word.size(); i++)
    {
        for(char symb = 'a'; symb <= 'z'; symb++)
        {
            string newWord = word;
            newWord[i] = symb;
            if(dictionary->search(newWord))
                result->insert(newWord);
        }
    }
}

void findInsert(std::string& word, Dictionary* dictionary, set<string>* result)
{
    for(int i = 0; i <= word.size(); i++)
    {
        for (char symb = 'a'; symb <= 'z'; symb++)
        {
            string insertWord = word.substr(0, i) + symb + word.substr(i, word.size());
            if(dictionary->search(insertWord))
                result->insert(insertWord);
        }
    }
}

void outputSuggestions(set<string>* result, std::string indent)
{
    if(result->size() == 0)
        cout << indent << "No suggestions were found" << endl;

    for (const string& suggestion : *result)
        cout << indent << suggestion << endl;
}

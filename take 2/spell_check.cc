// YOUR NAME.
// spell_check.cc: A simple spell checker.

#include <fstream>
#include <iostream>
#include <string>
#include <cctype>     //for transform
#include <algorithm>  //for the erase, remove commands
#include <cstdlib>    //for and exit(1) call.

// You can change to quadratic probing if you
// haven't implemented double hashing.
#include "double_hashing.h"
using namespace std;

// You can add more functions here.


void ParseWord(const HashTableDouble<string>& dictionary, const string word_in_file){
  if(dictionary.Contains(word_in_file)){
    cout << word_in_file << " is CORRECT" << endl;
  }
  if(!dictionary.Contains(word_in_file)){
    cout << word_in_file << " is INCORRECT";
    //Case A - adding one character in any possible position
    for(int i = 0; i <word_in_file.length()+1; i++){
      for(char c = 'a'; c <= 'z'; c++){
      string addedWord = word_in_file.substr(0,i) + c + word_in_file.substr(i);
      if(dictionary.Contains(addedWord)){
        cout << endl << "** " << word_in_file << " -> " << addedWord << " ** case A";}
      }
    }
    //Case B - deleting one character from the word.
    for(int i=0; i < word_in_file.length(); i++){
      string deletedWord = word_in_file.substr(0, i)+ word_in_file.substr(i + 1);
      if (dictionary.Contains(deletedWord)){
        cout << endl << "** " << word_in_file << " -> " << deletedWord << " ** case B";
      }
    }

    //Case C - swapping 2 characters
    for(int i = 0; i < word_in_file.length()-1; i++) {
      string swapCharacter = word_in_file.substr(0,i) + word_in_file[i+1] + word_in_file[i] + word_in_file.substr(i+2);
      if (dictionary.Contains(swapCharacter)){
        cout << endl << "** " << word_in_file << " -> " << swapCharacter << " ** case C";
      }
    }
    cout << endl;
  }
}

//Character to check to ensure there are no numbers in the word.
bool NumberCheck(const string word){
  for (char c : word){
    if(!( (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') )) {
      return true;
    }
  }
  return false;
}


void StripWord( string &word_in_file ) {
  transform(word_in_file.begin(), word_in_file.end(), word_in_file.begin(), ::tolower);           // lowercase
  word_in_file.erase(remove(word_in_file.begin(), word_in_file.end(), ','), word_in_file.end());  // remove comma
  word_in_file.erase(remove(word_in_file.begin(), word_in_file.end(), '.'), word_in_file.end());  // remove period
  word_in_file.erase(remove(word_in_file.begin(), word_in_file.end(), ':'), word_in_file.end());  // remove ':'
  word_in_file.erase(remove(word_in_file.begin(), word_in_file.end(), ';'), word_in_file.end());  // remove ';'
  word_in_file.erase(remove(word_in_file.begin(), word_in_file.end(), '['), word_in_file.end());  // remove '['
  word_in_file.erase(remove(word_in_file.begin(), word_in_file.end(), ']'), word_in_file.end());  // remove ']'
  word_in_file.erase(remove(word_in_file.begin(), word_in_file.end(), '\"'), word_in_file.end()); // remove "
  // remove ''s'
  size_t apostrophe_s = word_in_file.find('\'');
  if (apostrophe_s != string::npos) {
    word_in_file.erase( apostrophe_s, word_in_file.length() );
  }
}


// Creates and fills double hashing hash table with all words from
// dictionary_file

HashTableDouble<string> MakeDictionary(const string &dictionary_file) {
  HashTableDouble<string> dictionary_hash;
  string word_in_file;
  ifstream input_dictionary_file(dictionary_file);
  if (input_dictionary_file.fail()) {
    cerr << "Could not open <dictionary_file>\n";
    exit(1); // 1 indicates an error occurred
  }
  while ( input_dictionary_file >> word_in_file ) { 
    dictionary_hash.Insert(word_in_file); 
  }
  input_dictionary_file.close();

  return dictionary_hash;
}

// For each word in the document_file, it checks the 3 cases for a word being
// misspelled and prints out possible corrections
void SpellChecker(const HashTableDouble<string>& dictionary, const string &document_file) {
  cout << "Running Spell Check..." << endl;
  cout << "Document filename: " << document_file << endl;

  string word_in_file;
  ifstream input_document_file(document_file);
  if (input_document_file.fail()) {
    cerr << "Could not open <document_filename>\n";
    exit(1); // 1 indicates an error occurred
  }
  while (input_document_file >> word_in_file) {
    StripWord( word_in_file );
    if(!NumberCheck(word_in_file)){  //checks for numbers in word_in_file
      size_t pos = word_in_file.find('-');  //checks word for hyphen
      if (pos!=string::npos) {    //enters only if hyphen is found
        //takes string before hyphen
        string substr1 = word_in_file.substr(0, pos); 
        string substr2; //no value yet in case word has 2 hyphens
        if (word_in_file[pos+1] == '-'){
          //makes substr2 start after 2 hyphens
          substr2 = word_in_file.substr(pos+2); 
        }
        else {
          //makes substr2 start after sole hyphen
          substr2 = word_in_file.substr(pos+1);
        }
        //sends new substrings to parser with dictionary
        ParseWord(dictionary, substr1);
        ParseWord(dictionary, substr2);
      }
      //if no hyphen is found, sends the whole word to checker
      else{
        ParseWord(dictionary, word_in_file);
      }
    }
  }
  input_document_file.close();
}


// @argument_count: same as argc in main
// @argument_list: save as argv in main.
// Implements
int testSpellingWrapper(int argument_count, char** argument_list) {
    const string document_filename(argument_list[1]);
    const string dictionary_filename(argument_list[2]);

    // Call functions implementing the assignment requirements.
    HashTableDouble<string> dictionary = MakeDictionary(dictionary_filename);
    SpellChecker(dictionary, document_filename);

    return 0;
}

// Sample main for program spell_check.
// WE WILL NOT USE YOUR MAIN IN TESTING. DO NOT CODE FUNCTIONALITY INTO THE
// MAIN. WE WILL DIRECTLY CALL testSpellingWrapper. ALL FUNCTIONALITY SHOULD BE
// THERE. This main is only here for your own testing purposes.
int main(int argc, char** argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <document-file> <dictionary-file>"
         << endl;
    return 0;
  }
  
  testSpellingWrapper(argc, argv);
  
  return 0;
}

/*
Name: Drew Bruck
Class: CSCI 335
Professor: Jaime Canizales
Assignment: Project 2
Description: This project is intended to create a basic spell checker. It will 
parse a text document and compare the words to a dictionary file.  If a 
word matches, it will move to the next word.  If a match is not found, the
program will either add or remove a letter for every space/character in the 
word. When a word is mispelled and a possible replacement found, program
will print the mispelled word and possible corrections to word.
*/

#include "double_hashing.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>    // exit(1) call
#include <algorithm>  // erase, remove
#include <cctype>     // transform
using namespace std;

// ContainsNumbers() returns true if word contains numbers, used by
// CheckWordInDocumentFilename()
//
// @word: string contains one word from the input file 
// return: boolean returns true if word contains numbers, false if contains
//         only chars
bool ContainsNumbers (const string word){
  char c;
  for (size_t i = 0; i < word.length(); i++) {
    c = word.at(i);  
    if ( !( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) ) ) { 
      return true; 
    }
  }
  return false;
}

// ContainsHashTable() provide a list of candidate corrections by adding one
// character in each possible position, removing one character from the word,
// or swapping adjacent characters in the word and prints corrections out to
// standard output
//
// @hash_table: object containing the double probing hash table 
// @word_in_file: string contains one word from the input file 
// return: none, prints to standard output
template <typename HashTableType>
void ContainsHashTable( HashTableType &hash_table, const string word_in_file) {
  if ( !hash_table.Contains(word_in_file) ){
    cout << word_in_file << ": ";// << endl;
    // Adding one character in each possible position
    for(unsigned i = 0; i < word_in_file.length()-1; ++i) {
      for(char c = 'a'; c <= 'z'; ++c) {
        string insertion = word_in_file.substr(0, i) + c + word_in_file.substr(i);
        if ( hash_table.Contains(insertion) ) { cout << insertion << ", "; }
      }
    }
    // Removing one character from the word
    // Swapping adjacent characters in the word
    for(unsigned i = 0; i < word_in_file.length()-1; ++i) {
      string swap_character = word_in_file.substr(0,i) + word_in_file[i+1] + word_in_file[i] + word_in_file.substr(i+2);
      if ( hash_table.Contains(swap_character) ) { cout << swap_character << ", "; }

      string deletion = word_in_file.substr(0, i)+ word_in_file.substr(i + 1);
      if ( hash_table.Contains(deletion) ) { cout << deletion << ", "; }
    }
    cout << endl;
  }
}

// CleanWord() removes non-alphabetic characters, excluding numbers, from
// word_in_file. Also converts all characters to lower-case. Functionality is
// only provided for ", . : ; [ ] " " as well as removing 's.
// Example Patel's becomes patel.   
//
// @word_in_file: string contains one word from the input file 
// return: none, word_in_file is contains non-alphabetic characters.
void CleanWord( string &word_in_file ) {
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

// InsertDictionaryIntoHashTable() opens dictionary_filename and inserts words
// into double probing hash table. 
//
// @hash_table: object containing the double probing hash table 
// @dictionary_filename: user supplied file containing dictionary words
// return: none, hash_table contains dictionary words. 
template <typename HashTableType>
void InsertDictionaryIntoHashTable( HashTableType &hash_table, const string &dictionary_filename ) {
  string word_in_file;
  ifstream input_dictionary_filename(dictionary_filename);
  if (input_dictionary_filename.fail()) {
    cerr << "Could not open <dictionary_filename>\n";
    exit(1); // 1 indicates an error occurred
  }
  while ( input_dictionary_filename >> word_in_file ) { 
    hash_table.Insert(word_in_file); 
  }
  input_dictionary_filename.close();
}

// CheckWordInDocumentFilename() opens and parces document_filename and calls
// ContainsHashTable(). Parses words with '-' symbol in them by splitting them
// up into two words. 
//
// @hash_table: object containing the double probing hash table 
// @document_filename: user supplied file containing a paragraph with spelling
//                     mistakes
// return: none, passes words to ContainsHashTable()
template <typename HashTableType>
void CheckWordInDocumentFilename( HashTableType &hash_table, const string &document_filename ) {
  string word_in_file;
  ifstream input_document_filename(document_filename);
  if (input_document_filename.fail()) {
    cerr << "Could not open <document_filename>\n";
    exit(1); // 1 indicates an error occurred
  }
  while ( input_document_filename >> word_in_file ) {
    CleanWord( word_in_file );
    if (!ContainsNumbers(word_in_file)) {
  
  //for hyphenated words, this will find "-" and remove it. It will also
  //separate the two halves into "substr1" and "substr2".
      size_t pos = word_in_file.find('-');
      if (pos!=string::npos) {
        string substr1 = word_in_file.substr(0, pos);
        string substr2;
        // there could be words with '--'
        if (word_in_file[pos+1] == '-'){
          substr2 = word_in_file.substr(pos+2);
        }
        else {
          substr2 = word_in_file.substr(pos+1);
        }
        ContainsHashTable(hash_table, substr1);
        ContainsHashTable(hash_table, substr2);
      }
      else{
        ContainsHashTable(hash_table, word_in_file);
      }
    }
  }
  input_document_filename.close();
}

// CheckSpelling() initializes the hash table by calling
// InsertDictionaryIntoHashTable() and checks spelling of the words by calling
// CheckWordInDocumentFilename()
//
// @hash_table: object containing the double probing hash table 
// @document_filename: user supplied file containing a paragraph with spelling
//                     mistakes
// @dictionary_filename: user supplied file containing dictionary words
// return: none, calls InsertDictionaryIntoHashTable() and 
//         CheckWordInDocumentFilename()
template <typename HashTableType>
void CheckSpelling( HashTableType &hash_table, const string &document_filename, const string &dictionary_filename ) {
  cout << "CheckSpelling..." << endl;
  cout << "Document filename: " << document_filename << endl;
  cout << "Dictionary filename: " << dictionary_filename << endl;
  hash_table.MakeEmpty();  
  
  InsertDictionaryIntoHashTable( hash_table, dictionary_filename );
  CheckWordInDocumentFilename( hash_table, document_filename );
}

// Sample main for program SpellChecker
int main(int argc, char **argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <document file> <dictionary file>" << endl;
    return 0;
  }
  
  const string document_filename(argv[1]);
  const string dictionary_filename(argv[2]);

  HashTableDouble<string> double_probing_table;
  CheckSpelling(double_probing_table, document_filename, dictionary_filename);
  return 0;
}

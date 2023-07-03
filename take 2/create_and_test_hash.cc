/*
Name: Drew Bruck
Class: CSCI 335
Professor: Jaime Canizales
Assignment: Project 2
Description: This project is intended to create a hash table that can solve 
collisions through three various methods: Quadratic, Linear, and Double
Hashing.  The program will take three inputs: 2 text files and a flag to
engage the specific hashing method of choice.
*/

#include "quadratic_probing.h"
#include "linear_probing.h"
#include "double_hashing.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // so we can do an exit(1) call
using namespace std;



template <typename HashTableType>
void InsertWordsIntoHashTable( HashTableType &hash_table, const string &words_filename ) {
  string word_in_file;
  ifstream input_words_filename(words_filename);
  if (input_words_filename.fail()) {
    cerr << "Could not open <words_filename>\n";
    exit(1); // 1 indicates an error occurred
  }
  while ( input_words_filename >> word_in_file ) {
    hash_table.Insert(word_in_file);
  }
  input_words_filename.close();
}

/*
This function opens input file "query filename", parses through the file and compares
it to the newly created hash table. It will print the words that match the words in the
hash_table. It also keeps track of and prints the total number of collisions 

@hash_table: object containing either linear, quadratic, or double probing
             hash table
@query_filename: user supplied file containing words to be searched in
                 hash_table
return: none, prints the word in query_filename that matches word in 
        hash_table

*/
template <typename HashTableType>
void CheckWordInQueryFile( HashTableType &hash_table, const string &query_filename ) {
  string word_in_file;
  ifstream input_query_filename(query_filename);
  if (input_query_filename.fail()) {
    cerr << "Could not open <query_filename>\n";
    exit(1); // 1 indicates an error occurred
  }
  size_t total_number_of_collisions;
  while ( input_query_filename >> word_in_file ) {
    hash_table.resetCollisions();
    if ( hash_table.Contains(word_in_file) ) {
      hash_table.getTotalNumberOfCollisions( total_number_of_collisions );
      cout << word_in_file << " Found " << total_number_of_collisions+1 << endl;
    }
    else if ( !hash_table.Contains(word_in_file) ){
      hash_table.getTotalNumberOfCollisions( total_number_of_collisions );
      cout << word_in_file << " Not Found " << total_number_of_collisions+1 << endl;
    }
  }
  input_query_filename.close();
}

// TestFunctionForHashTable() inserts words from words_filename into hash_table
// outputs total number of collisions, total number of elements in the table
// size of table, load factor, average number of collisions, and calls 
// CheckWordInQueryFile()
//
// @hash_table: object containing either linear, quadratic, or double probing
//              hash table
// @words_filename: user supplied file containing words to be inserted into
//                  hash_table
// @query_filename: user supplied file containing words to be searched in
//                  hash_table
// return: none, prints the word in query_filename that matches word in 
//         hash_table
template <typename HashTableType>
void TestFunctionForHashTable(HashTableType &hash_table, const string &words_filename, const string &query_filename) {
  cout << "TestFunctionForHashTables..." << endl;
  cout << "Words filename: " << words_filename << endl;
  cout << "Query filename: " << query_filename << endl;
  hash_table.MakeEmpty();  
  //..Insert your own code
  // PART 1
  InsertWordsIntoHashTable( hash_table, words_filename );

  double total_number_of_elements_in_the_table = hash_table.getTotalNumberOfElements( );
  cout << "Number of items: " << total_number_of_elements_in_the_table << endl;

  double size_of_table = hash_table.getTotalSizeOfTable( );
  cout << "Size of hash table: " << size_of_table << endl;
  cout << "Load factor: " << total_number_of_elements_in_the_table / size_of_table << endl;
  size_t collision_count_;
  hash_table.getTotalNumberOfCollisions( collision_count_ );
  cout << "Collisions: " << collision_count_ << endl;
  cout << "Avg. number of collisions: " << collision_count_ / total_number_of_elements_in_the_table << endl;
  cout << " " << endl;

  // PART 2
  CheckWordInQueryFile( hash_table, query_filename );
}

// Sample main for program CreateAndTestHash
int main(int argc, char **argv) {
  if (argc != 4) {
    cout << "Usage: " << argv[0] << " <wordsfilename> <queryfilename> <flag>" << endl;
    return 0;
  }
  
  const string words_filename(argv[1]);
  const string query_filename(argv[2]);
  const string param_flag(argv[3]);

  if (param_flag == "linear") {
    HashTableLinear<string> linear_probing_table;
    TestFunctionForHashTable(linear_probing_table, words_filename, query_filename);    
  } else if (param_flag == "quadratic") {
    HashTable<string> quadratic_probing_table;
    TestFunctionForHashTable(quadratic_probing_table, words_filename, query_filename);    
  } else if (param_flag == "double") {
    HashTableDouble<string> double_probing_table;
    TestFunctionForHashTable(double_probing_table, words_filename, query_filename);    
  } else {
    cout << "Uknown tree type " << param_flag << " (User should provide linear, quadratic, or double)" << endl;
  }
  return 0;
}

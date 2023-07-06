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

#include <fstream>
#include <iostream>
#include <string>
#include "linear_probing.h"
#include "double_hashing.h"
#include "quadratic_probing.h"

using namespace std;

//Function to input each word from .txt file into hash_table
template <typename HashTableType>
void InsertWordsIntoHashTable( HashTableType &hash_table, 
            const string &words_filename){
  string word_in_file;
  ifstream input_words_filename(words_filename);  //input file
  if (input_words_filename.fail()) {    //test to ensure proper input from file
    cerr << "Could not open <words_filename>\n";
    exit(1); // 1 indicates an error occurred
  }
  //create hash_table from input
  while (input_words_filename >> word_in_file) {
    hash_table.Insert(word_in_file);
  }
  input_words_filename.close(); //close input file
}

/*
Function should read in the file, checks to see if it's present and print 
the amount of collisions that occurred.
*/
template <typename HashTableType>
void CheckQueryFile(HashTableType &hash_table, const string &query_filename) {
  string word_in_file;
  ifstream input_query_filename(query_filename);
  if (input_query_filename.fail()) {
    cerr << "Could not open <query_filename>\n";
    exit(1); // 1 indicates an error occurred
  }
  size_t total_number_of_collisions;
  while (input_query_filename >> word_in_file) {
    hash_table.resetCollisions();
    if ( hash_table.Contains(word_in_file) ) {
      hash_table.getTotalNumberOfCollisions( total_number_of_collisions );
      cout << word_in_file << " FOUND " << total_number_of_collisions+1 << " collision(s)."
      << endl;
    }
    else if (!hash_table.Contains(word_in_file)){
      hash_table.getTotalNumberOfCollisions( total_number_of_collisions );
      cout << word_in_file << " NOT FOUND " << total_number_of_collisions+1 << " collision(s)."
      << endl;
    }
  }
  input_query_filename.close(); //close input file
}

// @hash_table: a hash table (can be linear, quadratic, or double)
// @words_filename: a filename of input words to construct the hash table
// @query_filename: a filename of input words to test the hash table
template <typename HashTableType>
void TestFunctionForHashTable(HashTableType &hash_table, 
                  const string &words_filename, 
                  const string &query_filename) {
    hash_table.MakeEmpty();
  
    cout << "Words filename: " << words_filename << endl;
    cout << "Query filename: " << query_filename << endl;
    cout << endl;
      
    
    InsertWordsIntoHashTable( hash_table, words_filename );

    //Print number of collisions that occurred.
    size_t collision_count_;
    hash_table.getTotalNumberOfCollisions( collision_count_ );
    cout << "Collisions: " << collision_count_ << endl;

    //Print number of elements in table.
    double total_number_of_elements_in_the_table = hash_table.getTotalNumberOfElements( );
    cout << "Number of elements: " << total_number_of_elements_in_the_table << endl;

    //Print size of hash table, the load factor and the avg collisions
    double size_of_table = hash_table.getTotalSizeOfTable( );
    cout << "Size of hash table: " << size_of_table << endl;
    cout << "Load factor: " << total_number_of_elements_in_the_table / size_of_table << endl;
    cout << "Avg. number of collisions: " << collision_count_ / total_number_of_elements_in_the_table 
    << endl << endl;

    CheckQueryFile( hash_table, query_filename ); //For Part 2
}

// @argument_count: argc as provided in main
// @argument_list: argv as provided in imain
// Calls the specific testing function for hash table (linear, quadratic, or double).
int testHashingWrapper(int argument_count, char **argument_list) {
    const string words_filename(argument_list[1]);
    const string query_filename(argument_list[2]);
    const string param_flag(argument_list[3]);
    int R = 89;
    if (argument_count == 5) {
      const string rvalue(argument_list[4]);
      R = stoi(rvalue);
    }

    if (param_flag == "linear") {
      // Uncomment below when you have implemented linear probing.
    HashTableLinear<string> linear_probing_table;
    TestFunctionForHashTable(linear_probing_table, words_filename,
      			 query_filename);
    } else if (param_flag == "quadratic") {
    HashTable<string> quadratic_probing_table;
    TestFunctionForHashTable(quadratic_probing_table, words_filename,
				 query_filename);
    } else if (param_flag == "double") {
	  cout << "R Value: " << R << endl;
        // Uncomment below when you have implemented double hashing.
    HashTableDouble<string> double_probing_table;
    TestFunctionForHashTable(double_probing_table, words_filename,
	 			 query_filename);
    } else {
	  cout << "Unknown tree type " << param_flag
	     << " (Sorry, there, Bucko: you need to provide linear, quadratic, or double)" << endl;
    }
    return 0;
}

// Sample main for program create_and_test_hash. DO NOT CHANGE IT.
// WE WILL NOT USE YOUR MAIN IN TESTING. DO NOT CODE FUNCTIONALITY INTO THE
// MAIN. WE WILL DIRECTLY CALL testHashingWrapper. ALL FUNCTIONALITY SHOULD BE
// THERE. This main is only here for your own testing purposes.
int main(int argc, char **argv) {
    if (argc != 4 and argc != 5) {
	cout << "Usage: " << argv[0]
	     << " <wordsfilename> <queryfilename> <flag>" << endl;
	cout << "or Usage: " << argv[0]
	     << " <wordsfilename> <queryfilename> <flag> <rvalue>" << endl;
	return 0;
    }

    testHashingWrapper(argc, argv);
    return 0;
}

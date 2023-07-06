Drew Bruck - 24242807
Project 2 Readme File

This project is (hopefully) a functional, fully working program that can create 
hash tables and handle collisions through the three following methods: quadratic probing,
linear probing, and double hashing.  Furthermore, this program also includes a very
basic spell checker.

The hashing functions seemed to be quite a problem for me initially.  I spent a good deal
of time just following all the function calls to actually figure out what I needed to
implement and what was already provided for.  Using the quadratic function as a base idea,
I then created the double hashing and linear probing implementations from that file.  It
took several glances through the textbook, quite a few websites and several several 
youtube videos to polish up some of my, at first, unnecessarily complicated functions.

The spell checker I found a little more intuitive as to where and how to begin once I 
had the hash table implementation functioning.  In order to properly parse the word, I 
first had to strip it of any symbols or non-letter characters.  In doing so, I reduced 
the entire word to characters only available between a and z on the ASCII table.  This 
allowed for an easier, less-cumbersome scan of the query word.  From there, it was simply
creating functions that meet the three provided ways to correct a misspelled word: adding
a character, removing a character, or swapping two characters.

This project was a beast in the condensed format.  I really hope you enjoy it and use it
every morning with your coffee.  You're welcome.
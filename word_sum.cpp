#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <vector>
using namespace std;

//structure to keep track of a letter and its corresponding decimal value
struct relation{
    char letter;
    int value;
};

class WordSum
{
    private:
        

    public:
        //vector<vector<char> > number_relation; //2D vector to store number relations

        WordSum(vector<string> problem_statement, string answer)
        {
            vector<char> letters; //stores all unique letters in the problem
            num_of_chars(problem_statement, answer, letters); //int unique_chars = 
            if(letters.size() >= 10) //means there are more letters than numbers to be assigned
            {
                cout << "Invalid strings: There are more than 10 unique letters.  " << endl;
                //would be good it constructer would terminate here
            }

            relation num_relation[letters.size()]; //creates array of structures to store each unique letter and number in
            for(int i=0; i<letters.size(); i++) //adds all unique letters to each num_relation struct
            {
                num_relation[i].letter = letters[i];
            }
            
            if(cycle(0, num_relation, letters.size(), problem_statement, answer) == 0) //this function keeps calling itself until 
            {
                //print number relations and return attempts
                for(int i=0; i<letters.size(); i++) //prints number relations to user
                {
                    cout << num_relation[i].letter << " = " << num_relation[i].value << ",";
                }
                cout << endl;
            }
            else
            {
                cout << "No combinations could be found for this question" << endl;
            }
            
            /*
            if(right_most_col(problem_statement, answer) == 0)
            {
                //set first letter of answer to 1 in relations vector
                for(int i=0; i)
                new_vec.push_back(answer[0]);
                new_vec.push_back('1');
                number_relation.push_back(new_vec);
                //debug - maybe delete new_vec now
            }*/
        }

        //checks if there is only one letter in the right most column
        int right_most_col(vector<string> problem_statement, string answer)
        {
            for(int i=0; i<problem_statement.size(); i++)
            {
                if(problem_statement[i].size() == answer.size())
                {
                    return 1;
                }
            }
            return 0;
        }

        int num_of_chars(vector<string> problem_statement, string answer, vector<char>& unique_chars) //checks how many unique chars there are in the problem
        {
            //vector<char> unique_chars;
            for(int i=0; i<problem_statement.size(); i++) //for every word in problem statement/question
            {
                for(int j=0; j<problem_statement[i].size(); j++) //for every letter in the each word
                {
                    if(find(unique_chars.begin(), unique_chars.end(), problem_statement[i][j]) == unique_chars.end()) //if letter is not already in unique chars vector
                    {
                        unique_chars.push_back(problem_statement[i][j]); //then add the letter to the list
                    }
                }
            }

            for(int i=0; i<answer.length(); i++) //checks if any letters included in the answer string are not in the question strings/vector
            {
                if(find(unique_chars.begin(), unique_chars.end(), answer[i]) == unique_chars.end())
                {
                    unique_chars.push_back(answer[i]); //add letter to list 
                }
            }
            
            return unique_chars.size(); //returns the amount of unique letters
        }

        int different_numbers(relation* num_relation, int size) //checks if all letters have unique/different numbers related to them
        {
            for(int i=0; i<size; i++) //for every unique letter 
            {
                for(int j=i+1; j<size; j++) //for every other letter to right of i index/position
                {
                    if(num_relation[i].value == num_relation[j].value) //if letter contains the same int/decimal value
                    {
                        return 1; //means that list contains duplicate values
                    }
                }
            }
            return 0; //found no duplicate values
        }

        int get_value(relation* num_relation, int size, char letter) //returns value of letter 
        {
            for(int i=0; i<size; i++) //for every unique letter - loops until it finds letter
            {
                if(letter == num_relation[i].letter) //if letter is found
                {
                    return num_relation[i].value; //return letter's value
                }
            }

            cout << "Could not find value. Letter not included in number relations array." << endl;
        }

        //function to check if this combination of numbers is a solution
        int test_relations(relation* num_relation, vector<string> question, string answer, int size)
        {
            //final_total = sum of all words in question, word_total = total value of full words, tenths = keeps track of position a value should be in the word
            int final_total = 0, word_total = 0, tenths = 1;

            for(int row = 0; row< question.size(); row++) //for every word in the question statement
            {
                for(int i = question[row].length()-1; i>=0 ; i--) //for each letter in each word in question
                {
                    char letter = question[row][i]; //individual letter in word
                    word_total = word_total + (get_value(num_relation, size, letter) * tenths); //calculates current decimal value of word
                    tenths = tenths*10; //changes to next position - 1*10, 10*10, 100*10, 1000*10,......
                }
                final_total = final_total + word_total; //adds word to final total value
                word_total = 0; //resets word value to 0 to allow the next word to be calculated 
                tenths = 1; //resets so word can start at the one's position
            }

            for(int i = answer.length()-1; i>0; i++) // finds value of the answer
            {
                char letter = question[row][i]; //individual letter in word
                word_total = word_total + (get_value(num_relation, size, letter) * tenths); //calculates current decimal value of word
                tenths = tenths*10; //changes to next position - 1*10, 10*10, 100*10, 1000*10,......
            }

            if(word_total == final_total) //if word sum equals answer word value
            {
                return 0; //means this unique combination of numbers is a solution
            }
            else
            {
                return 1; //means this unique combination of numbers does not work
            }
        }

        //recursive function that works like a for loop for the amount of letters in the problem:
        //changes letter each time it calls the function
        int cycle(int index, relation* num_relation, int unique_letters, vector<string> question, string answer)
        {
            if(index < unique_letters) //flag to make sure index hasnt increased over amount of letters in problem
            {
                for(num_relation[index].value = 0; num_relation[index].value < 10; num_relation[index].value++) //sets value of letter to 0-9
                {
                    if(cycle(index+1, num_relation, unique_letters, question, answer) == 0) //answer was found, the recursion can finish
                    {
                        return 0;
                    }
                    else //soluion has not been found yet
                    {
                        if(different_numbers(num_relation, unique_letters) == 0) //checks if each unique letter has a unique number related to it
                        {
                            if(test_relations(num_relation, question, answer, unique_letters) == 0) //checks if this combination of number relations is a solution
                            {
                                return 0; //exit function if solution was found
                            }
                        }
                    }
                }
            }

            return 1; //this means no solution has been found yet
        }
};

int main()
{
    WordSum ws1({"SEND", "MORE"}, "MONEY");
    cout << ws1 << endl;
    WordSum ws2({"EARTH", "AIR", "FIRE", "WATER"}, "NATURE");
    cout << ws2 << endl;
    WordSum ws3({"SATURN", "URANUS", "NEPTUNE", "PLUTO"}, "PLANETS");
    cout << ws3 << endl;

    return 0;
}
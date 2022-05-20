//
// Created by babnishvyas on 2/19/2022.
//

#ifndef PROGRAMMING_USER_H
#define PROGRAMMING_USER_H
#include <string>
#include "Question.h"
#include <unordered_map>
using namespace std;

class User{
public:
    string username;

    unordered_map<string,Question> MyQuestions;
    unordered_map<string, Question> MyInterests;
    vector<string> MyVotingInterest;
};


#endif //PROGRAMMING_USER_H

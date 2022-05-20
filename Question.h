//
// Created by babnishvyas on 2/19/2022.
//

#ifndef PROGRAMMING_QUESTION_H
#define PROGRAMMING_QUESTION_H
#include <string>
#include "Comment.h"
#include "VoteType.h"
#include <vector>
#include <unordered_map>
using namespace std;

class Question{
public:
    string questionId;
    string userId;
    string questiontext;
    unordered_map<string ,Comment> commentsOnQuestion;
    unordered_map<string, VoteType> votes;
    void AddComment(string commentText, string userId){
        Comment* comment = new Comment();
        comment->commentString = commentText;
        commentsOnQuestion[userId] = *comment;
    }
    int getUpvotes(){
        int upvotes = 0;
        for(auto it = votes.begin();it!=votes.end();it++) {
            if (it->second == UpVote) {
                upvotes++;
            }
        }
        return upvotes;
    }
    int getDownvotes(){
        int downvotes = 0;
        for(auto it = votes.begin();it!=votes.end();it++) {
            if (it->second == DownVote) {
                downvotes++;
            }
        }
        return downvotes;
    }
    string toString(){
        int upvotes = getUpvotes();
        int downvotes = getDownvotes();

        string result = "";
        result += "id: ";
        result += questionId + "\n";
        result += "by: ";
        result += userId;
        result += "\nUpVotes: ";
        result += to_string(upvotes);
        result += "\nDownVotes: ";
        result += to_string(downvotes);
        result += "\nQuestion: ";
        result += questiontext;
        result += "\n";
        if (commentsOnQuestion.size() > 0) {
            result += "Comments:\n";
        }
        for(auto it=commentsOnQuestion.begin();it!=commentsOnQuestion.end();it++){
            result += "by: ";
            result += it->first;
            result  += "\n";
            result += it->second.commentString;
            result += "\n ";
        }
        return result;
    }
};

#endif //PROGRAMMING_QUESTION_H

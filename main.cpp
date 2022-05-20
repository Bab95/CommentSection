#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "User.h"
#include "Question.h"
#include "VoteType.h"
using namespace std;


unordered_map<string, User> userDatabase;
unordered_map<string, Question> questionDB;
static int questionCounter = 0;
class PostService{
public:
    void Post(string userId, string questiontext){
        Question *question = new Question();
        question->questiontext = questiontext;
        questionCounter++;
        question->questionId = to_string(questionCounter);
        question->userId = userId;
        string qid = question->questionId;
        if (userDatabase.find(userId) != userDatabase.end()){
            userDatabase[userId].MyQuestions[qid] = *question;
        }else{
            User* user = new User();
            user->username = userId;
            user->MyQuestions[question->questionId] = (*question);
            userDatabase[userId] = *user;
        }
        questionDB[qid] = *question;
        cout << "id : " << question->questionId << endl;
    }
    void Vote(string userId, VoteType vote, string questionId){
        if (userDatabase.find(userId) != userDatabase.end()){
            if (questionDB[questionId].votes.find(userId) == questionDB[questionId].votes.end()){
                questionDB[questionId].votes[userId] = vote;
                userDatabase[userId].MyVotingInterest.push_back(questionId);
            }else if (questionDB[questionId].votes[userId] != vote){
                // may be user is trying to change it's vote from up to down or down to up
                // fix this only change from up to down vote.
                questionDB[questionId].votes[userId] = vote;
            }
        }
    }
    void Comment(string userId, string questionId, string commentText){
        if (userDatabase.find(userId) == userDatabase.end()){
            cout << "User Doesn't exists!!!!";
            return;
        }
        User _user = userDatabase[userId];
        Question _question = questionDB[questionId];
        _question.AddComment(commentText, userId);
        questionDB[questionId] = _question;
        if (userDatabase[userId].MyQuestions.find(questionId) == userDatabase[userId].MyQuestions.end()){
            userDatabase[userId].MyInterests[questionId] = _question;
        }else{
            userDatabase[userId].MyQuestions[questionId] = _question;
        }
    }
};
enum SortingCriteria{
    COMMENTS,
    UPVOTES,
    DOWNVOTES
};
enum FilterCriteria{
    POSTED,
    COMMENTED,
    VOTED
};
bool compareComments(const Question& a, const Question& b){
    return a.commentsOnQuestion.size() - b.commentsOnQuestion.size();

}
bool compareUpVotes(const Question& a, const Question& b) {
    int upvotesA = 0;
    for(auto it = a.votes.begin();it!=a.votes.end();it++) {
        if (it->second == UpVote) {
            upvotesA++;
        }
    }
    int upvoteB = 0;
    for(auto it = b.votes.begin();it!=b.votes.end();it++) {
        if (it->second == UpVote) {
            upvoteB++;
        }
    }
    return upvotesA - upvoteB;
}
bool compareDownVotes(const Question& a, const Question& b){
    int downVotesA = 0;
    for(auto it = a.votes.begin();it!=a.votes.end();it++) {
        if (it->second == DownVote) {
            downVotesA++;
        }
    }
    int downVotesB = 0;
    for(auto it = b.votes.begin();it!=b.votes.end();it++) {
        if (it->second == DownVote) {
            downVotesB++;
        }
    }
    return downVotesA - downVotesB;
}
class GetService{
private:

    vector<Question> GetQuestions(string userId){
        vector<Question> relevantQuestions;

        for(auto it=questionDB.begin(); it!=questionDB.end();it++){
            relevantQuestions.push_back(it->second);
            // cout << it->second.toString();
        }
        /*
        for(auto it=userDatabase[userId].MyInterests.begin(); it!=userDatabase[userId].MyInterests.end();it++){
            // cout << it->second.toString();
            relevantQuestions.push_back(it->second);
        }
*/

        return relevantQuestions;
    }
public:
    void PrintQuestions(vector<Question>& questions){
        for(int i=0;i<questions.size(); i++){
            cout << questions[i].toString() << endl;
        }
    }
    void GetQuestionFeed(string userId){
        vector<Question> questions = GetQuestions(userId);
        PrintQuestions(questions);
    }
    void GetQuestionFeed(string userId, SortingCriteria sortingCriteria){
        vector<Question> questions = GetQuestions(userId);
        if (sortingCriteria == COMMENTS) {
            sort(questions.begin(), questions.end(), compareComments);
        }else if(sortingCriteria == UPVOTES){
            sort(questions.begin(), questions.end(), compareUpVotes);
        }else{
            sort(questions.begin(), questions.end(), compareDownVotes);
        }
        PrintQuestions(questions);
    }
    void GetQuestionFeed(string userId, SortingCriteria sortingCriteria, FilterCriteria filterCriteria){
        vector<Question> questions;
        if (filterCriteria == POSTED){
            for(auto it = userDatabase[userId].MyQuestions.begin();it != userDatabase[userId].MyQuestions.end();it++){
                questions.push_back(it->second);
            }
        }else if (filterCriteria == COMMENTED){
            for(auto it = userDatabase[userId].MyInterests.begin();it != userDatabase[userId].MyInterests.end();it++){
                questions.push_back(it->second);
            }
        }else if(filterCriteria == VOTED){
            for(int i=0;i<userDatabase[userId].MyVotingInterest.size();i++){
                string _quesId = userDatabase[userId].MyVotingInterest[i];
                questions.push_back(questionDB[_quesId]);
            }
        }
        PrintQuestions(questions);
    }

    void GetQuestionFeed(string userId, FilterCriteria filterCriteria){
        if (filterCriteria == POSTED){
            for(auto it = userDatabase[userId].MyQuestions.begin();it != userDatabase[userId].MyQuestions.end();it++){
                cout << it->second.toString()<<endl;
            }
        }else if (filterCriteria == COMMENTED){
            for(auto it = userDatabase[userId].MyInterests.begin();it != userDatabase[userId].MyInterests.end();it++){
                cout << it->second.toString()<<endl;
            }
        }else if(filterCriteria == VOTED){
            for(int i=0;i<userDatabase[userId].MyVotingInterest.size();i++){
                string _quesId = userDatabase[userId].MyVotingInterest[i];
                cout << questionDB[_quesId].toString()<<endl;
            }
        }
    }
};
int main() {
    PostService* postService = new PostService();
    GetService* getService = new GetService();
    /*
    string command;
    string username;
    string question;
    while (command != "end"){
        cin >> command;
        if (command == "post"){
            cin>>username>>question;
            postService->Post(username, question);
        }else if(command == "upvote"){
            cin>> username>>question;
            postService->Vote(username, UpVote, question);
        }else if(command =="downvote"){
            cin>> username>>question;
            postService->Vote(username, DownVote, question);
        }else if(command == "comment"){
            string text;
            cin>> username>>question >>text ;
            postService->Comment(username, question, text);
        }
        else if (command == "view"){
            cin>> username;
            string sorting, filter;
            cin>>sorting>>filter;
            if (sorting == "none" && filter == "none") {
                getService->GetQuestionFeed(username);
            }
        }
    }
     */
    //getService->GetQuestionFeed("u1", COMMENTS, POSTED);
    postService->Post("u1", "Why is earth flat?");
    postService->Post("u2", "hjdkjsbca");
    postService->Post("u3", "hjsadsfdkjsbca");
    postService->Comment("u1", "2", "this is comment");
    postService->Vote("u1", DownVote, "2");
    postService->Vote("u1", UpVote, "3");
    postService->Vote("u2", UpVote, "1");
    postService->Vote("u3", UpVote, "2");
    postService->Vote("u2", UpVote, "3");
    postService->Vote("u3", DownVote, "2");
    postService->Vote("u1", DownVote, "3");
    postService->Comment("u1", "1", "this is commenton1");
    postService->Comment("u2", "3", "this is commentcnkajljsc");
    postService->Comment("u3", "3", "this is caflkjdabscomment");
    getService->GetQuestionFeed("u1");
    cout<<"-------"<<endl;
    getService->GetQuestionFeed("u1", DOWNVOTES, VOTED);
    //postService->Comment("u1", "2", "this is comment");
    return 0;
}

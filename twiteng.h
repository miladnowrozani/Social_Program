#ifndef TWITENG_H
#define TWITENG_H
#include <map>
#include <unordered_map>
#include <string>
#include <set>
#include <stack>
#include <vector>
#include "user.h"
#include "datetime.h"
#include "heap.h"
#include "tagdata.h"
#include "hash.h"

class TwitEng
{
 public:
  TwitEng();
  ~TwitEng();
  /**
   * Parses the Twitter database and populates internal structures
   * @param filename of the database file
   * @return true if there is an error, false if successful
   */
  bool parse(char* filename);

  /**
   * Allocates a tweet from its parts and adds it to internal structures
   * @param username of the user who made the tweet
   * @param timestamp of the tweet
   * @param text is the actual text of the tweet as a single string
   */
  void addTweet(DateTime& time, std::string& text);

  /**
   * Searches for tweets with the given words and strategy
   * @param words is the hashtag terms in any case without the '#'
   * @param strategy 0=AND, 1=OR
   * @return the tweets that match the search
   */
  std::vector<Tweet*> search(std::vector<std::string>& terms, int strategy);
   
  /**
   * Dump feeds of each user to their own file
   */
  void dumpFeeds();


  /* You may add other member functions */

  /**
  * create a following connection from one user to another
  * @param user is the user whos following, following is the user who is being following by user
  */
  void addToFollowing(const std::string& following);


  /*
  * Saves database instance into a new file location
  * @param filename is the name of the new database location
  */
  void saveData(std::ofstream& outFile);

  /**
   * Dumps strongly connected components of users in a specified output stream
   */
  void dumpSCC(std::ofstream& outFile);

  /**
   * Prints the most trending in descending order up to the last rank
   */
  void printTrending(int lastRank);

  /**
   * Evaluates password and sees if it matches username's hashKey
   */
  void loginStatus(std::string name, std::string password);

  /**
   * Checks if user is logged in
   */
  bool isLoggedIn();


  /**
   * Sets the string currentUser to nothing which signals no user is logged in
   */
  void logout();

 private:
  /* Add any other data members or helper functions here  */

  // Current user thats logged in
  std::string currentUser;

  // Contains username keys and the respective User types
  std::map<std::string, User*> userMap;

  // Vector of all the tweets in the file database
  std::vector<Tweet*> tweetVect;

  // Contains the hashword and a set of all the tweets the hash phrase exists in
  std::map<std::string, std::set<Tweet*> > hashTagMap;

  // Contains all tweets for every mention
  std::map<std::string, std::vector<Tweet*> > mentionMap;

  // Contains all the hashtags in order of most trending (hashtags called most time)
  Heap<TagData, TagStringEqual, TagIntGreater, TagStringHasher> trending_;

  // hash map of user names to their hashed password
  std::unordered_map<std::string, unsigned long long> hashKeyMap;

  // Used for finding SCC's in our database
  struct vertex
  {
    std::string user;
    int index;
    int lowlink;
    vertex(std::string name) : user(name), index(-1), lowlink(-1) {}
    vertex() {}
  };
  
  // Used for storing in a set
  struct sortVertex
  {
    bool operator()(const vertex& v1, const vertex& v2)
    {
      return v1.user < v2.user;
    }
  };

  // Stack of vertices used for strongConnect (DFS)
  std::stack<vertex> vStack;

  // Stack of vertices used for strongConnect (DFS)
  std::set<vertex, sortVertex> vSet;

   /**
   * fills the users in map and the following list
   * @param inFile is the input file stream read from parse
   */
  void fillUsers(std::ifstream& inFile);

   /**
   * fills the users in map with corresponding tweets
   * @param inFile is the input file stream read from parse
   */
  void tweetToUsers(std::ifstream& inFile);

   /**
   * gathers each hashtag and mentions and collects all twitter posts its related to
   */
  void fillKeyMaps();

   /**
   * gathers each hashtag and mentions and collects all twitter posts its related to
   */
  void strongConnect(vertex& v, std::set<std::set<std::string> >& results, int& index, std::map<std::string, vertex>& vertexMap);
};


#endif

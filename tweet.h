#ifndef TWEET_H
#define TWEET_H
#include <iostream>
#include <string>
#include <string>
#include <vector>
#include <list>
#include <set>
#include "datetime.h"

/* Forward declaration */
class User;

/**
 * Models a tweet and provide comparison and output operators
 */
class Tweet
{
 public:
  /**
   * Default constructor 
   */
  Tweet();

  /**
   * Constructor 
   */
  Tweet(User* user, DateTime& time, std::string& text);

  /**
   * Gets the timestamp of this tweet
   *
   * @return timestamp of the tweet
   */
  DateTime const & time() const;

  /**
   * Gets the actual text of this tweet
   *
   * @return text of the tweet
   */
  std::string const & text() const;

  /**
   * Returns the hashtagged words without the '#' sign
   *
   */
  std::set<std::string> hashTags() const;

  /**
   * Return true if this Tweet's timestamp is less-than other's
   *
   * @return result of less-than comparison of tweet's timestamp
   */
  bool operator<(const Tweet& other) const; 

  /**
   * Outputs the tweet to the given ostream in format:
   *   YYYY-MM-DD HH::MM::SS username tweet_text
   *
   * @return the ostream passed in as an argument
   */
  friend std::ostream& operator<<(std::ostream& os, const Tweet& t);

  /* Create any other public or private helper functions you deem 
     necessary */

  User* user() const;

  /* You may add other member functions */

  /**
  * Determines if the first word in the tweet is an @ call
  * @return true if the startName is not empty, otherwise false
  */
  bool startsWithMentioned();

  /**
  * Gives the username which is the starting word in message
  */
  std::string getStartName();

  /**
  * Gets the list of mentioned users
  * @return mentionList
  */
  std::list<std::string> getMentions();

 private:
  /* Add any other data members or helper functions here  */

  // Pointer to the user that posted this tweet
  User* userThatPosted;

  // Time tweet is posted
  DateTime timestamp;

  // Message of the tweet
  std::string message;

  // Set of all hashtags in the tweet
  std::set<std::string> hashSet;

  // List of all users mentioned in list (empty if the first word in tweet is an @mention)
  std::list<std::string> mentionList;

  // Starting word which is an @mention otherwise empty
  std::string startName;

};

/***********************************/
/* Leave this alone --             */
/*   it will help sort your tweets */
/***********************************/
struct TweetComp
{
  bool operator()(Tweet* t1, Tweet* t2)
  {
    return (*t2 < *t1);
  }
};
#endif

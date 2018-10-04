#include "user.h"
#include "tweet.h"
#include <iostream>
// #include <string>
// #include <set>
// #include <list>
// #include <vector>
using namespace std;

User::User(std::string name) : username(name) {}

User::~User() {};

std::string User::name() const
{
	return username;
}

std::set<User*> User::followers() const
{
	return followerSet;
}

std::set<User*> User::following() const
{
	return followingSet;
}

std::list<Tweet*> User::tweets() const
{
	return tweetList;
}

void User::addFollower(User* u)
{
	followerSet.insert(u);
}

void User::addFollowing(User* u)
{
	followingSet.insert(u);
}

void User::addTweet(Tweet* t)
{
	tweetList.push_back(t);
}

std::vector<Tweet*> User::getFeed()
{
	std::vector<Tweet*> result;

	// initialize vector with current object's tweet list first
	for (auto ts : tweetList)
	{
		result.push_back(ts);
	}

	// add tweets from following users
	for (auto userList :followingSet)
	{
		addFollowingTweets(result, userList->tweets());
	}

	return result;
}

void User::addFollowingTweets(std::vector<Tweet*>& tVect, const std::list<Tweet*>& tList)
{
	for (auto ts : tList)
	{
		// if a name is mentioned at beginning of tweet check if its the user's name
		if (ts->startsWithMentioned())
		{
			// if its user's name add it to feed, else don't
			if (ts->getStartName() == username)
			{
				tVect.push_back(ts);
			}
		}
		else
		{
			tVect.push_back(ts);
		}
	}
}
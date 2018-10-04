#include "tweet.h"
#include "user.h"
#include <sstream>

Tweet::Tweet() : message("") 
{
	userThatPosted = nullptr;
	// Datetime is initialized to all 0's
}


Tweet::Tweet(User* user, DateTime& time, std::string& text) : userThatPosted(user), timestamp(time), message(text) 
{
	// Create a string and input it into a stringstream
	std::string word;
	std::stringstream ss;
	ss << text;
	bool itsFirst = true;
	// Read the stringstream to find all hashtags and input into set member
	while(ss >> word)
	{
		if (word[0] == '#')
		{
			word = word.substr(1);
			hashSet.insert(word);
		}
		else if (word[0] == '@')
		{
			word = word.substr(1);
			if (!itsFirst)
			{
				mentionList.push_back(word);
			}
			else
			{
				startName = word;
			}
		}
		itsFirst = false;
	}
}

DateTime const& Tweet::time() const
{
	return timestamp;
}

std::string const& Tweet::text() const
{
	return message;
}

std::set<std::string> Tweet::hashTags() const
{
	return hashSet;
}

bool Tweet::operator<(const Tweet&other) const
{
	return timestamp < other.timestamp;
}

std::ostream& operator<<(std::ostream& os, const Tweet& t)
{
	os << t.timestamp << " " << t.userThatPosted->name() << " " << t.message;
	return os;
}

User* Tweet::user() const
{
	return userThatPosted;
}

bool Tweet::startsWithMentioned()
{
	if (startName.empty())
	{
		return false;
	}

	return true;
}

std::string Tweet::getStartName()
{
	if (startName.empty())
	{
		return NULL;
	}
	return startName;
}

std::list<std::string> Tweet::getMentions()
{
	return mentionList;
}
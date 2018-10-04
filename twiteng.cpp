#include <fstream>
#include <sstream>
#include "msort.h"
#include "twiteng.h"
#include "tweet.h"
#include "util.h"

using namespace std;

TwitEng::TwitEng() : currentUser("") , trending_(2)
{

}

TwitEng::~TwitEng()
{
	// delete userMap values
	for (auto i : userMap)
	{
		delete i.second;
	}
	for (unsigned i = 0; i < tweetVect.size(); i++)
	{
		delete tweetVect[i];
	}
}

bool TwitEng::parse(char* filename)
{

	std::ifstream inFile(filename);

	if (inFile.is_open())
	{
		fillUsers(inFile);
		tweetToUsers(inFile);
		fillKeyMaps();
		return false;
	}
	return true;
}

void TwitEng::addTweet(DateTime& time, std::string& text)
{
	Tweet* newTweet = new Tweet(userMap[currentUser], time, text);
	userMap[currentUser]->addTweet(newTweet);
	tweetVect.push_back(newTweet);

	std::string hashWord;

	std::set<std::string> hSet = newTweet->hashTags();
	for (auto itr : hSet)
	{
		hashWord = itr;
		convUpper(hashWord);
		if (hashTagMap.find(hashWord) != hashTagMap.end())
		{
			int oldAmount = hashTagMap[hashWord].size();
			hashTagMap[hashWord].insert(newTweet);
			int newAmount = oldAmount + 1;
			convLower(hashWord);
			TagData oldTag(hashWord, oldAmount);
			TagData newTag(hashWord, newAmount);
			trending_.decreaseKey(oldTag, newTag);
		}
		else
		{
			hashTagMap[hashWord].insert(newTweet);
			convLower(hashWord);
			trending_.push(TagData(hashWord, 1));
		}
	}
	if (newTweet->startsWithMentioned())
	{
		mentionMap[newTweet->getStartName()].push_back(newTweet);
	}
	else
	{
		std::list<std::string> mentionList = newTweet->getMentions();
		if (mentionList.size() > 0)
		{
			for (auto i : mentionList)
			{
				mentionMap[i].push_back(newTweet);
			}
		} 
	}
}

std::vector<Tweet*> TwitEng::search(std::vector<std::string>& terms, int strategy)
{
	std::vector<Tweet*> results;

	if (strategy == 1)
	{
		for (unsigned i = 0; i < terms.size(); i++)
		{
			convUpper(terms[i]);
			auto itr = hashTagMap.find(terms[i]);
			if (itr != hashTagMap.end())
			{
				for (auto tweets : hashTagMap[terms[i]])
				{
					results.push_back(tweets);
				}
			}
		}
	}

	else if (strategy == 0)
	{

		convUpper(terms[0]);
		auto itr = hashTagMap.find(terms[0]);

		if (itr != hashTagMap.end())
		{
			std::set<Tweet*> setCheck = hashTagMap[terms[0]];
			for (auto itr = setCheck.begin(); itr != setCheck.end(); itr++)
			{
				for (unsigned i = 1; i < terms.size(); i++)
				{
					convUpper(terms[i]);
					auto itr2 = hashTagMap[terms[i]].find(*itr);
					if (itr2 != hashTagMap[terms[i]].end())
					{
						if (i == terms.size() - 1)
						{
							results.push_back(*itr);
						}
					}

					else
					{
						mergeSort(results, TweetComp());
						return results;
					}
				}
			}
		}
	}
	return results;
}

void TwitEng::dumpFeeds()
{
	User* currUser = nullptr;
	std::vector<Tweet*> tempVect;
	std::string mainFeed;
	std::string mentionFeed;

	for (auto itr : userMap)
	{
		// storing the value of current user of userMap into currUser
		currUser = itr.second;
		// creating the filename mainFeed
		mainFeed = currUser->name() + ".feed";
		std::ofstream feedOutput(mainFeed);
		feedOutput << currUser->name() << std::endl;

		tempVect = currUser->getFeed();
		mergeSort(tempVect, TweetComp());
		for (unsigned i = 0; i < tempVect.size(); i++)
		{
			feedOutput << *tempVect[i] << std::endl;
		}

		// do the same thing for mention feed using the mentionMap
		mentionFeed = currUser->name() + ".mentions";
		std::ofstream mentionOutput(mentionFeed);
		mentionOutput << currUser->name() << std::endl;
		tempVect = mentionMap[currUser->name()];
		for (unsigned i = 0; i < tempVect.size(); i++)
		{
			mentionOutput << *tempVect[i] << std::endl;
		}
	}
}


void TwitEng::fillUsers(std::ifstream& inFile)
{
	// number of users in file
	int numOfUsers;
	unsigned long long hashKey;
	std::stringstream ss;
	std::string name, nextName, line;

	inFile >> numOfUsers;

	// read numOfUsers lines and fill the userMap with users and each of their followers and following lists
	for (int i = 0; i < numOfUsers; i++)
	{
		// read the first name on line, and store all relevant data in this user
		inFile >> name;

		// add user and hashkey pair in database to hashKeyMap
		inFile >> hashKey;
		hashKeyMap[name] = hashKey;


		// add first name user on line if map doesn't have it
		auto itr = userMap.find(name);
		if (itr == userMap.end())
		{
			userMap[name] = new User(name);
		}

		// read rest of line and store in ss
		getline(inFile, line);
		ss << line;

		while (ss >> nextName)
		{
			// if nextName not in map, add to map
			auto itr = userMap.find(nextName);

			if (itr == userMap.end())
			{
				userMap[nextName] = new User(nextName);
			}

			//add nextName to name's following and name to nextName's followers
			userMap[name]->addFollowing(userMap[nextName]);
			userMap[nextName]->addFollower(userMap[name]);
		}
	}
}

void TwitEng::tweetToUsers(std::ifstream& inFile)
{
	// variables used to create the tweet
	std::stringstream ss;
	std::string text, username;
	DateTime time;

	while (!inFile.eof())
	{
		// continue reading so long as time can be read
		if (inFile >> time)
		{
			inFile >> username;
			// must first skip past the space between name and text 
			std::getline(inFile, text, ' ');
			std::getline(inFile, text);
			Tweet* newTweet = new Tweet(userMap[username], time, text);
			userMap[username]->addTweet(newTweet);
			tweetVect.push_back(newTweet);
		}
	}
}

void TwitEng::fillKeyMaps()
{
	std::string hashWord;
	std::string mentionName;
	std::list<std::string> mList;
	std::set<std::string> hSet;
	for (unsigned i = 0; i < tweetVect.size(); i++)
	{
		// filling hashMap here
		hSet = tweetVect[i]->hashTags();
		for (auto itr : hSet)
		{
			hashWord = itr;
			transform(hashWord.begin(), hashWord.end(), hashWord.begin(), ::toupper);
			hashTagMap[hashWord].insert(tweetVect[i]);
		}

		// filling mentionMap here
		if (tweetVect[i]->startsWithMentioned())
		{
			mentionName = tweetVect[i]->getStartName();
			mentionMap[mentionName].push_back(tweetVect[i]);
		}
		else
		{
			mList = tweetVect[i]->getMentions();
			if (mList.size() > 0)
			{
				for (auto itr : mList)
				{
					mentionMap[itr].push_back(tweetVect[i]);
				}
			}
		}
	}

	// filling trending heap
	for (auto hash : hashTagMap)
	{
		int occ = hash.second.size();
		std::string tag = hash.first;
		convLower(tag);
		trending_.push(TagData(tag, occ));
	}
}

void TwitEng::addToFollowing(const std::string& following)
{
	auto itr = userMap.find(following);
	if (itr != userMap.end())
	{
		userMap[currentUser]->addFollowing(itr->second);
	}
	else
	{
		std::cout << "No user in database that can be followed" << std::endl;
	}
}

void TwitEng::saveData(std::ofstream& outFile)
{
	User* currUser;
	std::set<User*> followingSet;

	outFile << userMap.size() << endl;

	for (auto itr : userMap)
	{
		outFile << itr.first + " ";
		outFile << hashKeyMap[itr.first];

		currUser = itr.second;
		followingSet = currUser->following();
		for (auto itr2 : followingSet)
		{
			outFile << " " << itr2->name();
		}

		outFile << std::endl;
	}

	for (unsigned i = 0; i < tweetVect.size(); i++)
	{
		outFile << *tweetVect[i] << std::endl;
	}

}

void TwitEng::dumpSCC(std::ofstream& outFile)
{
	std::set<std::set<std::string> > results;

	int index = 0;
	std::map<std::string, vertex> vertexMap;
	// Fill the vertex list with all users in userMap
	for (auto u : userMap)
	{
		vertex v(u.first);
		vertexMap[u.first] = v;
	}

	// Run through all vertices and fill results set.
	for (auto node : vertexMap)
	{
		// If vertex hasn't been visisted, perform DFS
		if ((node.second).index == -1)
		{
			strongConnect(node.second, results, index, vertexMap);
		}

	}
	// Read through all the component sets 1 to n into file output stream outFile
	int i = 1;
	for (auto set = results.begin(); set != results.end(); set++)
	{
		outFile << "Component " << i << std::endl;
		for (auto v : *set)
		{
			outFile << v << endl;
		}
		outFile << endl;
		i++;
	}

}

void TwitEng::printTrending(int last)
{
	Heap<TagData, TagStringEqual, TagIntGreater, TagStringHasher> temp = trending_;
	for (int i = 0; i < last; i++)
	{
		if (temp.empty())
		{
			break;
		}
		cout << temp.top().tag << " : " << temp.top().num << endl;
		temp.pop();
	}
}

void TwitEng::loginStatus(std::string name, std::string password)
{
	unsigned long long hashVal = calculateHash(password);
	if (hashVal == hashKeyMap[name])
	{
		cout << "Login successful." << endl;
		currentUser = name;
	}
	else
	{
		cout << "Invalid username/password." << endl;
	}
}

bool TwitEng::isLoggedIn()
{
	if (currentUser.length() > 0)
	{
		return true;
	}
	return false;
}

void TwitEng::logout()
{
	// std::cout << currentUser << " is logged out." << std::endl;
	currentUser = "";
}
void TwitEng::strongConnect(vertex& v, std::set<std::set<std::string> >& results, int& index, std::map<std::string, vertex>& vertexMap)
{
	vertexMap[v.user].index = index;
	v.lowlink = index;
	index++;
	vStack.push(v);
	vSet.insert(v);
	vertex loopVertex;
	loopVertex.index = -1;

	auto itr = userMap.find(v.user);
	std::set<User*> successors = (itr->second)->following();

	// for all successors of vertex v
	for (auto u : successors)
	{
		vertex w = vertexMap[u->name()];
		if (v.user == "Mark")
		{	
			cout << vStack.size() << endl;
			cout << w.user << w.index << endl;
		}
		if (w.index == -1)
		{

			// cout << w.user << w.index << endl;
			strongConnect(w, results, index, vertexMap);
		}

		else if (vSet.find(w) != vSet.end())
		{
			if (w.index < v.lowlink)
			{
				loopVertex = w;
				vertexMap[v.user].lowlink = w.index;
				break;
			}
		}
	}

	std::set<std::string> scc;
	vertex sccVertex;
	if (loopVertex.index != -1)
	{
		while ((vStack.top()).user != loopVertex.user)
		{
			sccVertex = vStack.top();
			scc.insert(sccVertex.user);
			vStack.pop();
			vSet.erase(sccVertex);
		}
		sccVertex = vStack.top();
		scc.insert(sccVertex.user);
		vStack.pop();
		vSet.erase(sccVertex);
		results.insert(scc);

	}
	if (!vStack.empty())
	{

		sccVertex = vStack.top();
		scc.insert(sccVertex.user);
		vStack.pop();
		vSet.erase(sccVertex);
		results.insert(scc);

	}
}
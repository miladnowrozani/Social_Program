#include "cmdhandler.h"
#include "util.h"
#include <ctime>
#include <fstream>

using namespace std;

QuitHandler::QuitHandler()
{

}

QuitHandler::QuitHandler(Handler* next)
  : Handler(next)
{

}

bool QuitHandler::canHandle(const std::string& cmd) const
{
	return cmd == "QUIT";

}

Handler::HANDLER_STATUS_T QuitHandler::process(TwitEng* eng, std::istream& instr) const
{
	eng->dumpFeeds();
	return HANDLER_QUIT;
}

//---------------------------------------------------------------------------------------------------------------------------//
AndHandler::AndHandler()
{

}

AndHandler::AndHandler(Handler* next)
  : Handler(next)
{

}

bool AndHandler::canHandle(const std::string& cmd) const
{
	return cmd == "AND";
}

Handler::HANDLER_STATUS_T AndHandler::process(TwitEng* eng, std::istream& instr) const
{
	std::vector<std::string> terms;
	std::string val;
	int strategy = 0;
	while (instr >> val)
	{
		terms.push_back(val);
	}
	if (terms.size() == 1)
	{
		strategy = 1;
	}

	std::vector<Tweet*> results = eng->search(terms, strategy);
	if (results.size() > 0)
	{
		std::cout << results.size() << " matches:" << std::endl;
		for (unsigned i = 0; i < results.size(); i++)
		{
			std::cout << *results[i] << std::endl;
		}
		return HANDLER_OK;
	}
	else
	{
		std::cout << "No matches." << std::endl;
	}

	return HANDLER_ERROR;
}
//---------------------------------------------------------------------------------------------------------------------------//
OrHandler::OrHandler()
{

}

OrHandler::OrHandler(Handler* next)
  : Handler(next)
{

}

bool OrHandler::canHandle(const std::string& cmd) const
{
	return cmd == "OR";
}

Handler::HANDLER_STATUS_T OrHandler::process(TwitEng* eng, std::istream& instr) const
{
	std::vector<std::string> terms;
	std::string val;
	while (instr >> val)
	{
		terms.push_back(val);
	}

	std::vector<Tweet*> results = eng->search(terms, 1);
	if (results.size() > 0)
	{
		std::cout << results.size() << " matches:" << std::endl;
		for (unsigned i = 0; i < results.size(); i++)
		{
			std::cout << *results[i] << std::endl;
		}
		return HANDLER_OK;
	}
	else
	{
		std::cout << "No matches." << std::endl;
	}


	return HANDLER_ERROR;
}
//---------------------------------------------------------------------------------------------------------------------------//
TweetHandler::TweetHandler()
{

}

TweetHandler::TweetHandler(Handler* next)
  : Handler(next)
{

}

bool TweetHandler::canHandle(const std::string& cmd) const
{
	return cmd == "TWEET";
}

Handler::HANDLER_STATUS_T TweetHandler::process(TwitEng* eng, std::istream& instr) const
{
	if (eng->isLoggedIn())
	{
		time_t rawtime;
		struct tm* timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		int hour = timeinfo->tm_hour;
		if (hour > 12)
		{
			hour -= 12;
		}
		DateTime currTime(hour, timeinfo->tm_min, timeinfo->tm_sec, (1900+timeinfo->tm_year), (1+timeinfo->tm_mon), timeinfo->tm_mday);

		std::string text;
		// if (instr >> username)
		// {
		// 	std::getline(instr,text, ' ');
			std::getline(instr, text);
		// }
		if (text.length() == 0)
		{
			std::cout << "Error, no user inputted for tweet" << std::endl;
			return HANDLER_ERROR;
		}

		eng->addTweet(currTime, text);

		return HANDLER_OK;	
	}
	else
	{
		std::cout << "No user logged in." << std::endl;
		return HANDLER_OK;
	}
	return HANDLER_ERROR;

}
//---------------------------------------------------------------------------------------------------------------------------//
FollowHandler::FollowHandler()
{

}

FollowHandler::FollowHandler(Handler* next)
  : Handler(next)
{

}

bool FollowHandler::canHandle(const std::string& cmd) const
{
	return cmd == "FOLLOW";
}

Handler::HANDLER_STATUS_T FollowHandler::process(TwitEng* eng, std::istream& instr) const
{
	if (eng->isLoggedIn())
	{
		std::string following;
		if (instr >> following)
		{
			eng->addToFollowing(following);
		}
		else
		{
			return HANDLER_ERROR;
		}
	}
	else
	{
		std::cout << "No user logged in." << std::endl;
		return HANDLER_OK;	
	}

	return HANDLER_OK;
}
//---------------------------------------------------------------------------------------------------------------------------//
SaveHandler::SaveHandler()
{

}

SaveHandler::SaveHandler(Handler* next)
  : Handler(next)
{

}

bool SaveHandler::canHandle(const std::string& cmd) const
{
	return cmd == "SAVE";
}

Handler::HANDLER_STATUS_T SaveHandler::process(TwitEng* eng, std::istream& instr) const
{

	char filename[20];

	if (instr >> filename)
	{
		std::ofstream outFile(filename);
		eng->saveData(outFile);
		return HANDLER_OK;
	}

	return HANDLER_ERROR;
}
//---------------------------------------------------------------------------------------------------------------------------//
SCCHandler::SCCHandler(Handler* next)
  : Handler(next)
{

}

bool SCCHandler::canHandle(const std::string& cmd) const
{
	return cmd == "SCC";
}

Handler::HANDLER_STATUS_T SCCHandler::process(TwitEng* eng, std::istream& instr) const
{
	char filename[20];

	if (instr >> filename)
	{
		std::ofstream outFile(filename);
		eng->dumpSCC(outFile);
		return HANDLER_OK;
	}
	return HANDLER_ERROR;
	
}
//---------------------------------------------------------------------------------------------------------------------------//
TrendingHandler::TrendingHandler(Handler* next)
  : Handler(next)
{

}

bool TrendingHandler::canHandle(const std::string& cmd) const
{
	return cmd == "TRENDING";
}

Handler::HANDLER_STATUS_T TrendingHandler::process(TwitEng* eng, std::istream& instr) const
{
	int val = 0;
	if (instr >> val)
	{
		eng->printTrending(val);
		return HANDLER_OK;
	}
	return HANDLER_ERROR;
	
}
//---------------------------------------------------------------------------------------------------------------------------//
LoginHandler::LoginHandler(Handler* next)
  : Handler(next)
{

}

bool LoginHandler::canHandle(const std::string& cmd) const
{
	return cmd == "LOGIN";
}

Handler::HANDLER_STATUS_T LoginHandler::process(TwitEng* eng, std::istream& instr) const
{
	std::string name;
	std::string password;

	if (instr >> name)
	{
		if (instr >> password)
		{
			eng->loginStatus(name, password);
			return HANDLER_OK;
		}
		else
		{
			// std::cout << "No password inputted" << std::endl;
		}
	}
	return HANDLER_ERROR;
}
//---------------------------------------------------------------------------------------------------------------------------//
LogoutHandler::LogoutHandler(Handler* next)
  : Handler(next)
{

}

bool LogoutHandler::canHandle(const std::string& cmd) const
{
	return cmd == "LOGOUT";
}

Handler::HANDLER_STATUS_T LogoutHandler::process(TwitEng* eng, std::istream& instr) const
{
	if (eng->isLoggedIn())
	{
		eng->logout();
	}
	return HANDLER_OK;
}
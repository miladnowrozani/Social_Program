#include "datetime.h"
#include <iostream>
#include <iomanip>

using namespace std;


DateTime::DateTime() : hour(0), min(0), sec(0), year(0), month(0), day(0) {}

DateTime::DateTime(int hh, int mm, int ss, int year, int month, int day) : hour(hh), min(mm), sec(ss), year(year), month(month), day(day) {}

bool DateTime::operator<(const DateTime& other) const
{
	// check months if years are equal
	if (year == other.year)
	{
		// check days if months are equal
		if (month == other.month)
		{
			// check days if months are equal
			if (day == other.day)
			{
				// ...
				if (hour == other.hour)
				{
					// ...
					if (min == other.min)
					{
						// if seconds are equal here, return false
						if (sec == other.sec)
						{
							return false;
						}
						else
						{
							return sec < other.sec;
						}
					}
					else
					{
						return min < other.min;
					}
				}
				else
				{
					return hour < other.hour;
				}
			}
			else
			{
				return day < other.day;
			}
		}

		else
		{
			return month < other.month;
		}
	}
	else
	{
		return year < other.year;
	}
}

std::ostream& operator<<(std::ostream& os, const DateTime& other)
{
	os << setfill('0') << setw(4) << other.year << "-" 
	<< setfill('0') << setw(2) << other.month << "-" 
	<< setfill('0') << setw(2) << other.day << " " 
	<< setfill('0') << setw(2) << other.hour << ":" 
	<< setfill('0') << setw(2) << other.min << ":" 
	<< setfill('0') << setw(2) << other.sec;

	return os;

}

std::istream& operator>>(std::istream& is, DateTime& dt)
{
	// ignore variable
	string ignore;

	//read in vals and ignore -'s and :'s
	is >> dt.year;
	getline(is,ignore,'-');
	is >> dt.month;
	getline(is,ignore,'-');
	is >> dt.day;
	is >> dt.hour;
	getline(is,ignore,':');
	is >> dt.min;
	getline(is,ignore,':');
	is >> dt.sec;

	return is;

}

#include "timeSheetPersistency.hpp"
#include <fstream>

namespace punchr
{

    timeSheetPersistency::timeSheetPersistency(void)
	{

	}

    timeSheetPersistency::~timeSheetPersistency(void)
	{
	}

    timeSheetPersistencyStates timeSheetPersistency::syncJsonIn()
	{
		return timeSheetPersistencyStates::persistencyReadOK;
	}
    timeSheetPersistencyStates timeSheetPersistency::syncJsonOut()
	{
    	//docRoot.append(session);
    	//Json::FastWriter fW;
    	Json::StyledWriter sW;
    	session["years"] = sheet;
    	docRoot["punchr"] = session;
    	std::ofstream jsonOut(jsonFileName);
    	jsonOut << sW.write(docRoot);
    	std::cout << docRoot.toStyledString() << std::endl;
		return timeSheetPersistencyStates::persistencyReadOK;
	}

	timeSheetPersistencyStates timeSheetPersistency::readSessionJson(boost::posix_time::ptime &pStart)
	{
		std::ifstream jsonIn(jsonFileName);
		Json::Reader readJson;
		if(!readJson.parse(jsonIn,docRoot,false))
		{
			return timeSheetPersistencyStates::persistencyReadNOK;
		}

		pStart = boost::posix_time::time_from_string(docRoot["punchr"]["last-punch-in"].asString());
		return timeSheetPersistencyStates::persistencyReadOK;
	}

	timeSheetPersistencyStates timeSheetPersistency::writeSessionJson(boost::posix_time::ptime pnow, yearReport &report)
	{
		session["last-punch-in"]=boost::posix_time::to_simple_string(pnow);

		std::for_each(report.begin(),report.end(),
				[&](yearReport::value_type &year){
			Json::Value mths;
			std::for_each(year.second.begin(),year.second.end(),
					[&](monthSheet::value_type &month){
				Json::Value dys;
				std::for_each(month.second.begin(),month.second.end(),
						[&](minutesPerDay::value_type &mins){
					//std::cout << year.first << "-" << month.first << "-" << mins.first << "=" <<  mins.second<< std::endl;
					std::stringstream day;
					day << mins.first;
					dys[day.str()] = mins.second;
					//docRoot["punchr"][2015][3][mins.second];
				}
				);
			std::stringstream mont;
			mont << month.first;
			mths[mont.str()] = dys;
			}
			);
			std::stringstream yea;
			yea << year.first;
			sheet[yea.str()] = mths;
		}
		);

		return timeSheetPersistencyStates::persistencyWriteOK;
	}


}

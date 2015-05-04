#include "timeSheetPersistency.hpp"
#include <fstream>
#include <string>

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
    	std::ifstream jsonIn(jsonFileName);
    	Json::Reader readJson;
    	if(!readJson.parse(jsonIn,docRoot,false))
    	{
    		return timeSheetPersistencyStates::persistencyReadNOK;
    	}

		return timeSheetPersistencyStates::persistencyReadOK;
	}
    timeSheetPersistencyStates timeSheetPersistency::syncJsonOut()
	{
    	Json::StyledWriter sW;
    	session[timeSheetId] = sheet;
    	docRoot[punchrId] = session;
    	std::ofstream jsonOut(jsonFileName);
    	jsonOut << sW.write(docRoot);
    	std::cout << docRoot.toStyledString() << std::endl;
		return timeSheetPersistencyStates::persistencyReadOK;
	}

	timeSheetPersistencyStates timeSheetPersistency::readSessionJson(boost::posix_time::ptime &pStart, yearReport &report)
	{
		pStart = boost::posix_time::time_from_string(docRoot[punchrId][lastPunchInId].asString());

		Json::Value years = docRoot[punchrId][timeSheetId];
	    Json::Value::Members yrs = years.getMemberNames();
	    std::for_each(yrs.begin(),yrs.end(),
	    		[&](std::string &yr){
	    	            Json::Value::Members mths = docRoot[punchrId][timeSheetId][yr].getMemberNames();
                        std::for_each(mths.begin(),mths.end(),
                        		[&](std::string &mths){
                        		 Json::Value::Members dys = docRoot[punchrId][timeSheetId][yr][mths].getMemberNames();
                        		 std::for_each(dys.begin(),dys.end(),
                        				 [&](std::string &d){
                        			 	 report[std::stoi(yr)][std::stoi(mths)][std::stoi(d)]= docRoot[punchrId][timeSheetId][yr][mths][d].asInt();
                        		 });
                        });
	    });

		return timeSheetPersistencyStates::persistencyReadOK;
	}

	timeSheetPersistencyStates timeSheetPersistency::writeSessionJson(boost::posix_time::ptime pnow, yearReport &report)
	{
		session[lastPunchInId]=boost::posix_time::to_simple_string(pnow);

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

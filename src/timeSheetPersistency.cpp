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
		return timeSheetPersistencyStates::persistencyReadOK;
	}

	timeSheetPersistencyStates timeSheetPersistency::readSessionJson()
	{
		std::ifstream jsonIn(jsonFileName);
		Json::Reader readJson;
		if(!readJson.parse(jsonIn,docRoot,false))
		{
			return timeSheetPersistencyStates::persistencyReadNOK;
		}

		std::cout << "READ_JSON_IN" << docRoot["punchr"]["last-punch-in"] << std::endl;


		return timeSheetPersistencyStates::persistencyReadOK;
	}

	timeSheetPersistencyStates timeSheetPersistency::writeSessionJson(boost::posix_time::ptime pnow)
	{
		session["last-punch-in"]=boost::posix_time::to_simple_string(pnow);
		return timeSheetPersistencyStates::persistencyWriteOK;
	}


}

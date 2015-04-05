#include "timeSheetPersistency.hpp"

namespace punchr
{
    timeSheetPersistencyStates timeSheetPersistency::syncJsonIn()
	{
		return timeSheetPersistencyStates::persistencyReadOK;
	}
    timeSheetPersistencyStates timeSheetPersistency::syncJsonOut()
	{

    	docRoot.append(session);
    	Json::FastWriter fW;
    	std::string str = fW.write(docRoot);
    	std::cout << "JSON output:" << str << std::endl;
		return timeSheetPersistencyStates::persistencyReadOK;
	}

    timeSheetPersistency::timeSheetPersistency(void)
	{

	}

    timeSheetPersistency::~timeSheetPersistency(void)
	{
	}

	timeSheetPersistencyStates timeSheetPersistency::readSessionJson()
	{
		return timeSheetPersistencyStates::persistencyReadOK;
	}

	timeSheetPersistencyStates timeSheetPersistency::writeSessionJson(boost::posix_time::ptime pnow)
	{
		session["sessionBegin"]=boost::posix_time::to_simple_string(pnow);
		return timeSheetPersistencyStates::persistencyWriteOK;
	}


}

#include "timesheet.hpp"


namespace punchr
{
using namespace boost::gregorian;
using namespace boost::posix_time;

    timeSheet::timeSheet(void)
    {
    }

    timeSheet::~timeSheet(void)
    {

    }

	timeSheetStates timeSheet::punchIn()
	{
		boost::posix_time::ptime pStart;

		timeSheetPersister.syncJsonIn();
		if(timeSheetPersister.readSessionStart(pStart) == timeSheetSessionStates::sessionStarted)
		{
			std::cout << "TIME IN:" << boost::posix_time::to_simple_string(pStart) << std::endl;
			return timeSheetStates::punchInFail;
		}else
		{
			std::cout << "NO SESSION STARTED - STARTING" << std::endl;
			ptime pnow = second_clock::local_time();
			timeSheetPersister.writeSessionStart(pnow);
			timeSheetPersister.syncJsonOut();
			return timeSheetStates::punchInOK;
		}
		return timeSheetStates::punchInOK;
	}


	timeSheetStates timeSheet::punchOut()
	{


		/*timeReport[2015][4][20]=15;
		timeReport[2015][4][17]=12;
		timeReport[2015][3][30]=9;
		timeReport[2014][12][10]=87;
		timeReport[2014][6][1]=23;
		timeReport[2013][1][27]=19;*/
		//timeSheetPersister.writeSessionJson(pnow,timeReport);
		//timeSheetPersister.syncJsonOut();

	timeSheetPersister.readSessionJson(timeReport);

	std::cout << "TIME REPORT:" << timeReport[2015][4][20] << std::endl;
	std::cout << "TIME REPORT:" << timeReport[2015][4][17] << std::endl;
	std::cout << "TIME REPORT:" << timeReport[2015][3][30] << std::endl;
	std::cout << "TIME REPORT:" << timeReport[2014][12][10] << std::endl;
	std::cout << "TIME REPORT:" << timeReport[2014][6][1] << std::endl;
	std::cout << "TIME REPORT:" << timeReport[2013][1][27] << std::endl;

		return timeSheetStates::punchInOK;
	}

}

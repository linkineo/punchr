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
		ptime pnow = second_clock::local_time();

		timeReport[2015][4][20]=15;
		timeReport[2015][4][17]=12;
		timeReport[2015][3][30]=9;
		timeReport[2014][12][10]=87;
		timeReport[2014][6][1]=23;
		timeReport[2013][1][27]=19;

		//if(!sessionBegin.is_not_a_date_time())
		{
			timeSheetPersister.writeSessionJson(pnow,timeReport);
			timeSheetPersister.syncJsonOut();
		}

		// JUST FOR TEST
		boost::posix_time::ptime pStart;
		timeSheetPersister.readSessionJson(pStart);
		std::cout << "TIME IN: " << boost::posix_time::to_simple_string(pStart) << std::endl;



		/*Json::Value root;
		Json::Value toJ;
		Json::Value months;
		months.append("Jan");
		months.append("Feb");
		months.append("Mar");

		toJ["Month"]=months;
        root["Collec"] = toJ;*/

        //timeReport[2015][3][2]=25;
		return timeSheetStates::punchInOK;
	}
}

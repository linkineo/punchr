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

		//if(!sessionBegin.is_not_a_date_time())
		{
			timeSheetPersister.writeSessionJson(pnow);
			timeSheetPersister.syncJsonOut();
		}

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

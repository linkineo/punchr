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
			std::cout << "/!\\ Punched in already --> " << boost::posix_time::to_simple_string(pStart) << std::endl;
			std::cout << "Punch out first !" << std::endl;
			return timeSheetStates::punchInFail;
		}else;
		{
			ptime pnow = second_clock::local_time();
			std::cout << "--> Punch-in @ " << boost::posix_time::to_simple_string(pnow) << std::endl;
			timeSheetPersister.writeSessionStart(pnow);
			timeSheetPersister.syncJsonOut();
			return timeSheetStates::punchInOK;
		}
		return timeSheetStates::punchInOK;
	}


	timeSheetStates timeSheet::punchOut()
	{
		boost::posix_time::ptime pStart;
		boost::posix_time::ptime pNow = second_clock::local_time();

				timeSheetPersister.syncJsonIn();
		if(timeSheetPersister.readSessionStart(pStart) == timeSheetSessionStates::sessionStarted)
		{
			boost::posix_time::time_duration punchDuration;
			punchDuration = pNow - pStart;

			if(punchDuration.hours() > maxPunchTimeHours)
			{
				std::cout << "Error - A session can only last for 24 contiguous hours. This session started -->"
				<< boost::posix_time::to_simple_string(pStart) << std::endl;

			}else {
				timeSheetPersister.readSessionJson(timeReport);

				if(pNow.date().day() == pStart.date().day())
				{
					timeReport[pStart.date().year()]
					[pStart.date().month()]
					[pStart.date().day()]
							+= punchDuration.total_seconds()/60;
				}

				//untested
				if(pNow.date().day() == pStart.date().day() + 1)
				{
					boost::posix_time::ptime endOfDay(date(pStart.date().year(),pStart.date().month(),pStart.date().day()+1));
					boost::posix_time::time_duration toEndOfDay = endOfDay - pStart;
					boost::posix_time::time_duration fromBeginOfDay = pNow - endOfDay;

					timeReport[pStart.date().year()]
					[pStart.date().month()]
					[pStart.date().day()]
							+= toEndOfDay.total_seconds()/60;

					timeReport[pNow.date().year()]
					[pNow.date().month()]
					[pNow.date().day()]
							+= fromBeginOfDay.total_seconds()/60;
				}

				std::cout << "Punched out --> Duration = " << boost::posix_time::to_simple_string(punchDuration) << std::endl;

			}

			    timeSheetPersister.writeSessionJson(pStart,timeReport);
				timeSheetPersister.resetSessionStart();
				timeSheetPersister.syncJsonOut();

		} else
		{
			std::cout << " /!\\ Cannot punch-out. Punch-in first !" << std::endl;
		}

		return timeSheetStates::punchOutOK;
	}

}

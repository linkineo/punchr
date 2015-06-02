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

	timeConversions timeSheet::validateAndConvertTime(std::string inputTime,int &hoursIn, int &minutesIn)
	{
		int totalInp = std::stoi(inputTime);
		if(totalInp == -1)
		{
			return timeConversions::inputIsInvalidTime;
		}
		if(!(totalInp > 0 && totalInp < 2359))
		{
			std::cout << "Invalid time input format." << std::endl;
			return timeConversions::inputIsInvalidTime;
		}else
		{
			hoursIn = std::stoi(inputTime.substr(0,2));
			minutesIn = std::stoi(inputTime.substr(2,2));
			return timeConversions::inputIsValidTime;
		}

	}

	timeSheetStates timeSheet::punchIn(std::string inputTime)
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

			int hoursIn = 0, minutesIn = 0;
			if(validateAndConvertTime(inputTime,hoursIn,minutesIn) == timeConversions::inputIsValidTime)
			{
				ptime pConstructed(date(pnow.date().year(),pnow.date().month(),pnow.date().day()),hours(hoursIn)+minutes(minutesIn));
				pnow = pConstructed;
			}

			std::cout << "--> Punch-in @ " << boost::posix_time::to_simple_string(pnow) << std::endl;
			timeSheetPersister.writeSessionStart(pnow);
			timeSheetPersister.syncJsonOut();
			return timeSheetStates::punchInOK;
		}

	}


	timeSheetStates timeSheet::punchOut(std::string inputTime)
	{
		boost::posix_time::ptime pStart;
		boost::posix_time::ptime pNow = second_clock::local_time();

		int hoursIn = 0, minutesIn = 0;
		if(validateAndConvertTime(inputTime,hoursIn,minutesIn) == timeConversions::inputIsValidTime)
		{
			ptime pConstructed(date(pNow.date().year(),pNow.date().month(),pNow.date().day()),hours(hoursIn)+minutes(minutesIn));
			pNow = pConstructed;
		}

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

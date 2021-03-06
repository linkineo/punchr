#include "timesheet.hpp"
#include "termcolor.hpp"
#include <iomanip>


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
		if(inputTime == "now")
		{
			return timeConversions::inputIsInvalidTime;
		}
		int totalInp = std::stoi(inputTime);
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

	timeSheetStates timeSheet::punchList(int month)
	{
		using namespace std;
		boost::posix_time::ptime pNow;
		pNow = boost::posix_time::second_clock::local_time();
		yearReport yR;
		timeSheetPersister.syncJsonIn();
		timeSheetPersister.readSessionJson(yR);

		if(month == 0)
		{
			month = pNow.date().month();
		}

		boost::posix_time::ptime pMon(date(pNow.date().year(),month,pNow.date().day()));
		cout << termcolor::green << termcolor::bold <<
				setw(3) << "" <<
				setw(7) << left << "Time report - " << pMon.date().month() <<
				endl;
		cout  <<  termcolor::red << termcolor::bold <<
				setw(3) << left << "" <<
				setw(7) << "Day" <<
				setw(10) << "Hours" <<
				setw(10) << "Decimal"<<
		        termcolor::reset << endl;

		minutesPerDay mpD = yR[pNow.date().year()][month];
		int lineCount = 0;
		for_each(mpD.begin(),mpD.end(),[&](minutesPerDay::value_type &mins){

			lineCount++;
			if(lineCount%2)
			{
				cout << termcolor::cyan << termcolor::bold;
			}else
			{
				cout << termcolor::magenta << termcolor::bold;
			}
			double fracTime = (double) mins.second / 60;
			boost::posix_time::time_duration td(0, mins.second, 0, 0);
			//boost::posix_time::ptime pDay(date(pNow.date().year(),pNow.date().month(),mins.first));
			cout <<
					setw(3) << "" <<
					setw(7) << mins.first <<
					setw(10) << boost::posix_time::to_simple_string(td).substr(0,5) <<
					setw(10) << setprecision(3) << fracTime <<
					termcolor::reset <<
					endl;

		});


	}

	timeSheetStates timeSheet::punchStatus()
	{
		boost::posix_time::ptime pStart,pNow;
		pNow = boost::posix_time::second_clock::local_time();
		yearReport yR;
		timeSheetPersister.syncJsonIn();
		timeSheetPersister.readSessionJson(yR);

		if(timeSheetPersister.readSessionStart(pStart) == timeSheetSessionStates::sessionStarted)
		{
			std::cout << "/!\\ Punched in already --> " << boost::posix_time::to_simple_string(pStart) << std::endl;
		}else
		{
			std::cout << "No punch-in yet" << std::endl;
		}

		int timeToday = yR[pNow.date().year()][pNow.date().month()][pNow.date().day()];
		if(timeToday != 0) {
			double fracTime = (double) timeToday / 60;
			boost::posix_time::time_duration td(0, timeToday, 0, 0);
			std::cout << "Session today --> " << boost::posix_time::to_simple_string(td)
			<< " / " << std::setprecision(3) << fracTime <<
			"hrs" << std::endl;
		}else
		{
			std::cout << "No session today" << std::endl;
		}

		return timeSheetStates::punchStatusOK;
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

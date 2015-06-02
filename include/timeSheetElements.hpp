#include <map>
namespace punchr
{
	enum class timeSheetStates {punchInOK,punchInFail, punchOutOK, punchOutFail};
	enum class timeSheetPersistencyStates {persistencyWriteOK,persistencyReadOK,persistencyWriteNOK,persistencyReadNOK};
	enum class timeSheetSessionStates {sessionStarted,noSession};
	enum class timeConversions {inputIsValidTime,inputIsInvalidTime};

	const std::string jsonFileName = "punchr.json";
	const std::string timeSheetId = "time";
	const std::string punchrId = "punchr";
	const std::string lastPunchInId = "last-punch-in";

	const int maxPunchTimeHours = 24;

	typedef std::map<int,int> minutesPerDay;
	typedef std::map<int,minutesPerDay> monthSheet;
	typedef std::map<int,monthSheet> yearReport;

}

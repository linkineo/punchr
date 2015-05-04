#include <map>
namespace punchr
{
	enum class timeSheetStates {punchInOK,punchInFail};
	enum class timeSheetPersistencyStates {persistencyWriteOK,persistencyReadOK,persistencyWriteNOK,persistencyReadNOK};

	const std::string jsonFileName = "punchr.json";
	const std::string timeSheetId = "time";
	const std::string punchrId = "punchr";
	const std::string lastPunchInId = "last-punch-in";

	typedef std::map<int,int> minutesPerDay;
	typedef std::map<int,minutesPerDay> monthSheet;
	typedef std::map<int,monthSheet> yearReport;

}

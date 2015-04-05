#include <map>
namespace punchr
{
	enum class timeSheetStates {punchInOK,punchInFail};
	enum class timeSheetPersistencyStates {persistencyWriteOK,persistencyReadOK};

	typedef std::map<int,int> minutesPerDay;
	typedef std::map<int,minutesPerDay> monthSheet;
	typedef std::map<int,monthSheet> yearReport;

}

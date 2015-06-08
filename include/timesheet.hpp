#include <string>

#include "timeSheetPersistency.hpp"


namespace punchr{

	class timeSheet{
		public:
		     timeSheet();
		     ~timeSheet();
        
		     timeSheetStates punchIn(std::string inputTime);
		     timeSheetStates punchOut(std::string inputTime);
		     timeSheetStates punchStatus();
		     timeSheetStates punchList(int month);

            
		private:
		     timeConversions validateAndConvertTime(std::string inputTime,int &hoursIn, int &minutesIn);
		     yearReport timeReport;
		     boost::posix_time::ptime sessionBegin;
		     timeSheetPersistency timeSheetPersister;
	};

	

}

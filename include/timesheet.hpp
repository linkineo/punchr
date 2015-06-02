#include <string>

#include "timeSheetPersistency.hpp"


namespace punchr{

	class timeSheet{
		public:
		     timeSheet();
		     ~timeSheet();
        
		     timeSheetStates punchIn(std::string inputTime);
		     timeSheetStates punchOut(std::string inputTime);
		     timeConversions validateAndConvertTime(std::string inputTime,int &hoursIn, int &minutesIn);
            
		private:
		     yearReport timeReport;
		     boost::posix_time::ptime sessionBegin;
		     timeSheetPersistency timeSheetPersister;
	};

	

}

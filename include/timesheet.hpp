#include <string>

#include "timeSheetPersistency.hpp"


namespace punchr{

	class timeSheet{
		public:
		     timeSheet();
		     ~timeSheet();
        
		     timeSheetStates punchIn();
		     timeSheetStates punchOut();
            
		private:
		     yearReport timeReport;
		     boost::posix_time::ptime sessionBegin;
		     timeSheetPersistency timeSheetPersister;
	};

	

}

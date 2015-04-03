#include <string>
#include "timeSheetElements.hpp"

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

namespace punchr{

	class timeSheet{
		public:
		     timeSheet();
		     ~timeSheet();
        
		     timeSheetStates punchIn();
		     timeSheetStates punchOut();
            
		private:
	};

	

}

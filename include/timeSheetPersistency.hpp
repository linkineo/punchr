#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <jsoncpp/json/json.h>

#include "timeSheetElements.hpp"
namespace punchr
{
	class timeSheetPersistency
	{
		public:
		timeSheetPersistency();
		~timeSheetPersistency();

		timeSheetPersistencyStates syncJsonIn();
		timeSheetPersistencyStates syncJsonOut();

		timeSheetPersistencyStates readSessionJson();
		timeSheetPersistencyStates writeSessionJson(boost::posix_time::ptime pnow);
		private:
		Json::Value docRoot;
		Json::Value session;
		Json::Value sheet;

	};

}

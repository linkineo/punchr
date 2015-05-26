#include <iostream>

#include "boost/program_options.hpp"

#include "timesheet.hpp"



void handleOptions(int argc, char ** argv)
{
	namespace po = boost::program_options;
	po::options_description desc("Punchr options");
	desc.add_options()
			("lines,l","displays all recorded entries")
			("in,i","start a new job session")
			("out,o","stop an ongoing job session")
			("status,s","get current session status")
			;

	po::variables_map vm;
	po::store(po::parse_command_line(argc,argv,desc),vm);
	po::notify(vm);

	punchr::timeSheet ts;

	if(vm.count("lines"))
	{
		std::cout << "lines - not yet implemented" << std::endl;
	}
	if(vm.count("in"))
	{
		ts.punchIn();
	}
	if(vm.count("out"))
	{
		ts.punchOut();
	}
	if(vm.count("status"))
	{
		std::cout << "status - not yet implemented" << std::endl;
	}
}


int main(int argc, char** argv)
{
	handleOptions(argc,argv);
}



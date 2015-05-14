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

if(vm.count("lines"))
{
	std::cout << "lines" << std::endl;
}
if(vm.count("in"))
{
	std::cout << "in" << std::endl;
}
if(vm.count("out"))
{
	std::cout << "out" << std::endl;
}
if(vm.count("status"))
{
	std::cout << "status" << std::endl;
}
}


int main(int argc, char** argv)
{
   handleOptions(argc,argv);
   punchr::timeSheet ts;
   ts.punchIn();

}



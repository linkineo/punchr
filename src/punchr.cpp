#include <iostream>

#include "boost/program_options.hpp"

#include "timesheet.hpp"



void handleOptions(int argc, char ** argv)
{
	std::string timeInput("");
	int month = 0;

	namespace po = boost::program_options;
	po::options_description desc("Punchr options");
	desc.add_options()
			("lines,l",po::value<int>(&month)->implicit_value(0),"displays all recorded entries")
			("in,i",po::value<std::string>(&timeInput)->implicit_value("now"),"start a new job session")
			("out,o",po::value<std::string>(&timeInput)->implicit_value("now"),"stop an ongoing job session")
			("status,s","get current session status")
			;

	po::variables_map vm;
	po::store(po::parse_command_line(argc,argv,desc),vm);
	po::notify(vm);

	punchr::timeSheet ts;


	if(vm.count("lines"))
	{
		ts.punchList(vm["lines"].as<int>());
	}
	if(vm.count("in"))
	{
		ts.punchIn(vm["in"].as<std::string>());
	}
	if(vm.count("out"))
	{
		ts.punchOut(vm["out"].as<std::string>());
	}
	if(vm.count("status"))
	{
		ts.punchStatus();
	}
	if(vm.count("help") || argc == 1)
	{
		std::cout << desc << std::endl;
	}
}


int main(int argc, char** argv)
{
	handleOptions(argc,argv);
}



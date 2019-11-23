/*
    OpenParkingManager - An open source parking manager and parking finder.
    Copyright (C) 2019 Louis van der Walt

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <string>
#include <cstring>
#include <uv.h>

#include "CommandLineLicences.hpp"
#include "CommandLine.hpp"

void CommandLine::print_license_brief()
{
	write(license_brief_text.c_str(), license_brief_text.size());
}

void CommandLine::print_license_nw()
{
	write(license_nw_text.c_str(), license_nw_text.size());
}

void CommandLine::print_license_terms()
{
	write(terms_text.c_str(), terms_text.size());
}

void CommandLine::print_license()
{
	write(license_text.c_str(), license_text.size());
}

void CommandLine::print_help()
{
	std::string help_text = R"(
	help
	license
	licence-terms
	licence-nw
	licence-brief
)";
	write(help_text.c_str(), help_text.size());
}

void CommandLine::process_text(std::string text)
{
	std::vector<std::string> command_words;
	std::string delimiter = " ";

	int pos = 0;
	while((pos = text.find("\n")) != std::string::npos)
	{
		text.erase(pos,pos);
	}

	for(int i = 0; i < text.size(); i++)
	{
		text[i] = tolower(text[i]);
	}

	pos = 0;
	std::string word;
	while ((pos = text.find(delimiter)) != std::string::npos)
	{
    	word = text.substr(0, pos);
		command_words.push_back(word);
    	text.erase(0, pos + delimiter.length());
	}
	command_words.push_back(text);
	switch(hash(command_words[0].c_str()))
	{
		case hash("help"):
			std::cout << "a" << std::endl;
			break;
		default:
			std::cout << "b" << std::endl;
			break;
	}
	text = "";
}

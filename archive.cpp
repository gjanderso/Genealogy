#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "brother.h"
#include "archive.h"

archive::~archive(){
	for(auto b : brothers){
		if(b != nullptr){
			delete b;
		}
	}
}

void archive::readFile(std::ifstream& in){
	// Temp storage variables for reading
	std::string name, year, bg, tmp;
	std::getline(in, tmp);		// Discards header row
	
	while(!in.eof()){
		std::getline(in, year, ',');	// Grabs: semester initiated
		std::getline(in, name, ','); // name
		std::getline(in, bg, ',');	// big brother
		std::getline(in, tmp);	// littles

		addBrother(year, name, bg, tmp);
	}
	in.close();
}

brother* archive::searchBrother(const std::string& n){
	for(int i = 0; i < brothers.size(); ++i){
		if(brothers[i]->getName().find(n) != std::string::npos)
			return brothers[i];
	}
	return nullptr;
}

void archive::printInfo(brother* b) const{ 
	if(b == nullptr || !isBro(b)){
		std::cout << "Brother not found" << std::endl;
		return;
	} else{
		printBanner();
		b->printInfo();
	}
}

void archive::printLine(brother* b) const{
	if(!isBro(b)){
		std::cout << "Brother not found" << std::endl;
		return;
	} else
		b->printLine(b);
}

void archive::printFamily(brother* b) const{
	if(!isBro(b)){
		std::cout << "Brother not found" << std::endl;
		return;
	} else
		b->printTree(b, 0);
}

void archive::printBanner() const{
	// Header row for bros
	std::cout << std::left << std::setw(6) << "Year" << std::setw(25)
		  << "Name" << std::setw(25) << "Big Bro" << std::right
		  << std::setw(25) << "Little" << std::endl;
	std::cout << std::left << std::setw(6) << "-----" << std::setw(25) 
		  << "----" << std::setw(25) << "-------" << std::right 
		  << std::setw(25) << "------" << std::endl;
}
	
void archive::printList() const{
	bool empty = true;

	// Prints out brothers info
	for(int i = 0; i < brothers.size(); ++i){
		if(empty){
			printBanner();
			empty = false;
		}

		brothers[i]->printInfo();
	}
	if(empty)
		std::cout << "No brothers found" << std::endl;
}

void archive::printClass(const std::string& c){
	bool empty = true;
	
	std::istringstream strs(c);
    	std::string sem, yr, n;
    	strs >> sem;
    	strs >> yr;

    	if(sem == "fa" || sem == "Fa" || sem == "Fall" || sem == "fall")
        	sem = "Fa";
	else if(sem == "sp" || sem == "Sp" || sem == "Spring" || sem == "spring")
        	sem = "Sp";
	if(yr.length() > 2)
		yr = yr.substr(2,4);

	n = sem + ' ' + yr;

	for(int i = 0; i < brothers.size(); ++i){
		if(n == brothers[i]->getYear()){
			if(empty){
				printBanner();
				empty = false;
			}
			brothers[i]->printInfo();
		}
	}

	if(!empty)
		return;
	else
		empty = false;

	if(c == "Rabbit" || c == "Rabbs" || c == "Rabbits"){
		std::cout << "Rabbits\n-------" << std::endl;
		printFamily(searchBrother("Tony Geronimos"));
	} else if(c == "Mohican" || c == "Mohicans" || c == "Mo"){
		std::cout << "Mohicans\n--------" << std::endl;
		printFamily(searchBrother("Jeff Chamlis"));
	} else if(c == "Zebs" || c == "Zebras" || c == "zebs"){
		std::cout << "Zebs\n----" << std::endl;
		printFamily(searchBrother("Troy Paolantonio"));
	} else if(c == "Pandas" || c == "Platinum"){
		std::cout << "Pandas\n------" << std::endl;
		printFamily(searchBrother("Michael Weintraub"));
	} else
		empty = true;
	
	if(empty)
		std::cout << "No brothers found" << std::endl;
}

/* PRIVATE HELPER FUNCTIONS */

// Checks if brother exists in brothers
bool archive::isBro(const std::string& name){
	if(name.size() == 0)
		return false;

	for(int i = 0; i < brothers.size(); ++i){
		if(name == brothers[i]->getName()){
			big = i;
			return true;
		}
	}
	return false;
}

bool archive::isBro(const brother* b) const{
	if(b == nullptr)
		return false;

	for(auto bro : brothers){
		if(b->getName() == bro->getName())
			return true;
	}
	return false;
}

// Checks if brother exists as a little
bool archive::isLittle(const std::string& name){
	for(int i = 0; i < brothers.size(); ++i){
		if(brothers[i]->isLittle(name, lil)){
			big = i;
			return true;
		}
	}
	return false;
}

std::vector<std::string> archive::getLils(std::string& LL){
	std::istringstream ss(LL);
	std::vector<std::string> littles;
	std::string little;

	while(std::getline(ss, little, ','))
		littles.push_back(little);
	return littles;
}

std::string archive::format(std::string& x){
	std::string tmp;
	bool isSpace = false;
	bool isChar = false;

	for(int ch = 0; ch < x.size(); ++ch){
		// Handles names with more than 1 space in between
		if(isChar && ch+1 < x.size()){
			if(isSpace && isspace(x[ch]) && isalnum(x[ch+1]))
				isSpace = false;
		}
		// Checks if end of name
		if(isChar && isSpace && (!isalnum(x[ch]) 
		&& x[ch] != '-' && x[ch] != '.'))
			break;
		// Checks for name inbetween name
		if(isChar && isspace(x[ch])){
			isSpace = true;
			tmp += x[ch];
		}
		else if(isChar || isalnum(x[ch])
		|| x[ch] == '-' || x[ch] == '.'){
			isChar = true;
			tmp += x[ch];
		}
	}
	return tmp;
}

void archive::addBrother(std::string& year, std::string& name, 
std::string& bg, std::string& tmp){
	year = format(year);
	name = format(name);
	bg = format(bg);
	std::vector<std::string> littles = getLils(tmp);
	
	if(year.size() == 0)
		return;

	// New Brother	
	brother* newB = new brother(year, name);

	// Sets big		
	if(isBro(bg))
	{	// Sets big, sets bigs little to this
		if(!brothers[big]->replaceLittle(newB))
			brothers[big]->pushLittle(newB);
	}
	else if(bg.size() > 0)	// Catches if big is not bro
		newB->setBig(new brother(bg));

	// Adds littles to bro
	for(auto little : littles){
		little = format(little);
		if(little.size() == 0)
			continue;

		// Checks if little is already a bro
		if(isBro(little)){
			if(!newB->replaceLittle(brothers[big]))
				newB->pushLittle(brothers[big]);
		}
		else{
			newB->pushLittle(new brother(little));
		}
	}
	brothers.push_back(newB);
}

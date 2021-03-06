/**
	@author Marc Pierre Marc_Pierre@student.uml.edu
	@version 1.0 10/01/18
*/
#include "shipment.h"
#include <boost/regex.hpp>
#include <fstream>
#include <stdlib.h>
#include <stdexcept>
#include <exception>
#include <math.h>

using std::cerr;
using std::ifstream;
using std::runtime_error;
using boost::regex;
using boost::smatch;
using boost::gregorian::date_period;
using boost::gregorian::from_us_string;
using boost::gregorian::neg_infin;
using boost::gregorian::pos_infin;

SHIPMENT::SHIPMENT() {
	// Declaring variables
	string date;
	today = boost::gregorian::day_clock::local_day();
	int num;
	bool exception = true;
	while (exception) {  // making sure user entered a valid date
		try {
			exception = false;
			cout << "Initial date (mm/dd/yyyy): ";
			cin >> date;
			this->date = from_us_string(date);
		}
		catch (std::exception& e) {
			exception = true;
			cout << "Exception: " << e.what() << endl;
		}
	}

	cout << "How many isotopes? ";
	cin >> num;
	while (num <= 0) {  // making sure user enters valid number
		cout << "Number of isotopes must be greater than 0.\n\nHow many isotopes? ";
		cin >> num;
	}
	cout << "Mass (grams for solids) (liters for liquids): ";
	cin >> mass;
	while (mass <= 0) {  // making sure user enters valid number
		cout << "Mass of shipment must be greater than 0.\n\nMass (grams for solids) (liters for liquids): ";
		cin >> mass;
	}
	cout << "Nature *See DOT 173.425 Table 4* [Regular, Instrument or Article] (Type R, I, or A): ";
	cin >> nature;
	while (nature != 'R' && nature != 'r' && nature != 'I' && nature != 'i' && nature != 'A' && nature != 'a') {  // making sure user enters valid nature
		cout << "Please enter R, I or A.\n\nNature *See DOT 173.425 Table 4* [Regular, Instrument or Article] (Type R, I, or A): ";
		cin >> nature;
	}
	cout << "State *See DOT 173.425 Table 4* [Solid, Liquid or Gas] (Type S, L, or G): ";
	cin >> state;
	while (state != 'S' && state != 's' && state != 'L' && state != 'l' && state != 'G' && state != 'g') {  // making sure user enters valid nature
		cout << "Please enter S, L or G.\n\nState *See DOT 173.425 Table 4* [Solid, Liquid or Gas] (Type S, L, or G): ";
		cin >> state;
	}
	switch (state) {
	case 'S':
	case 's':
		cout << "Form [Special/Normal] (Type S or N): ";
		cin >> form;
		while (form != 'S' && form != 's' && form != 'N' && form != 'n') {  // making sure user enters valid nature
			cout << "Please enter S or N.\n\nForm [Special/Normal] (Type S or N): ";
			cin >> form;
		}
		break;
	case 'L':
	case 'l':
		cout << "Form [Tritiated/Other] (Type T or N): ";
		cin >> form;
		while (form != 'T' && form != 't' && form != 'N' && form != 'n') {  // making sure user enters valid nature
			cout << "Please enter T or N.\n\nForm [Tritiated/Other] (Type T or N): ";
			cin >> form;
		}
		// checking if invalid input
		if (((nature == 'I' || nature == 'i') || (nature == 'A' || nature == 'a')) && (form == 'T' || form == 't')) {
			cout << "Invalid Input: Tritiated Water Instrument or article\n **See DOT 173.425 Table 4**\n";
			exit(1);  // exiting program
		}
		break;
	case 'G':
	case 'g':
		cout << "Form [Tritium/Special/Normal] (Type T, S, or N): ";
		cin >> form;
		while (form != 'T' && form != 't' && form != 'S' && form != 's' && form != 'N' && form != 'n') {  // making sure user enters valid nature
			cout << "Please enter T, S or N.\n\nForm [Tritium/Special/Normal] (Type T, S, or N): ";
			cin >> form;
		}
		break;
	default:
		cout << "SHIPMENT Constructor: State Invalid\n";
		exit(1);
		break;
	}
	cout << endl;

	//  getting number of days since t0
	date_period dp(this->date, today);  // today - date
	decayTime = dp.length();  // updating decay time

	// adding isotopes to shipment
	for (int i = 0; i < num; i++)
		addIso(i);

	// finding classification of isotopes in shipment
	bool lim;
	for (iter = isos.begin(); iter != isos.end(); iter++) {
		if (exemptClass(iter->first)) {
			iter->second.isoClass = 0;
		}
		else {
			if (lim = limitedClass(iter->first))
				iter->second.isoClass = 1;
			else if (typeAClass(iter->first) && lim == false)
				iter->second.isoClass = 2;
			else if (HRCQClass(iter->first))
				iter->second.isoClass = 8;
			else if (typeBClass(iter->first))
				iter->second.isoClass = 4;
		}
	}

	// finding the classification of the shipment
	sClassOut = findClass();

	// Asking user if they want to find the date the shipment can be classified at a lower level
	if (sClass != 0) {  // only asking if shipment classification is greater than Exempt
		char userInput;
		cout << "Would you like to find the date this shipment can be classified at a lower classification? (Type Y or N): ";
		cin >> userInput;
		while (userInput != 'Y' && userInput != 'y' && userInput != 'N' && userInput != 'n') {
			cout << "Please enter Y or N\n\nWould you like to find the date this shipment can be classified at a lower classification? (Type Y or N): ";
			cin >> userInput;
		}
		if (userInput == 'Y' || userInput == 'y') {
			cout << "How many days from today would you like to check?: ";
			cin >> days2chk;
			while (days2chk <= 0) {
				cout << "Please enter a number greater than zero\n\nHow many days from today would you like to check?: ";
				cin >> days2chk;
			}
		} else {
			days2chk = -1;
		}
	}
}

void SHIPMENT::addIso(int num) {
	Data data;
	string name;
	char uAns;
	bool validName = false;

	while (validName == false) {  // making sure user enters a valid isotope name
		if (num == -1 || num == 0) {
			cout << "Name of isotope: ";
		} else {
			cout << "Name of isotope " << num << ": ";
		}
		cin >> name;
		try {
			validName = validIso(name);
		}
		catch (runtime_error e) {
			cerr << "\nAddIso:\nRuntime_Error: " << e.what() << endl;
			exit(1);
		}
	}

	cout << "Initial Activity (microCuries) of isotope: ";
	cin >> data.A0;
	while (data.A0 <= 0) {  // making sure user enters valid initial activity
		if (num == -1 || num == 0) {
			cout << "Initial Activity must be greater than 0.\n\nInitial Activity (microCuries) of isotope: ";
		} else {
			cout << "Initial Activity must be greater than 0.\n\nInitial Activity (microCuries) of isotope " << num << ": ";
		}
		cin >> data.A0;
	}

	if (!_strcmpi(name.c_str(), "U-230") || !_strcmpi(name.c_str(), "U-232") || !_strcmpi(name.c_str(), "U-233") 
		|| !_strcmpi(name.c_str(), "U-234") || !_strcmpi(name.c_str(), "U-236")) {
		cout << "Lung Absorption *See DOT 173.435 A1&A2 Values* [fast, medium, slow] (Type F, M, or S): ";
		cin >> absorption;
		while (absorption[0] != 'F' && absorption[0] != 'f' && absorption[0] != 'M' && absorption[0] != 'm' && absorption[0] != 'S' && absorption[0] != 's') {  // making sure user enters valid initial activity
			cout << "Please enter F, M or S.\n\nLung Absorption *See DOT 173.435 A1&A2 Values* [fast, medium, slow] (Type F, M, or S): ";
			cin >> absorption;
		}
	}

	if (!_strcmpi(name.c_str(), "Eu-150") || !_strcmpi(name.c_str(), "In-110") || !_strcmpi(name.c_str(), "Nb-89") 
		|| !_strcmpi(name.c_str(), "Np-236") || !_strcmpi(name.c_str(), "Re-182") || !_strcmpi(name.c_str(), "Sb-120") 
		|| !_strcmpi(name.c_str(), "Sb-128") || !_strcmpi(name.c_str(), "Ta-178") || !_strcmpi(name.c_str(), "Tb-156m")) {
		cout << "Would you like to access the short or the long half life version of this isotope (Type S or L): ";
		cin >> uAns;
		while (uAns != 'S' && uAns != 's' && uAns != 'L' && uAns != 'l') {  // making sure user enters valid initial activity
			cout << "Please enter S or L.\n\nWould you like to access the short or the long half life version of this isotope (Type S or L): ";
			cin >> uAns;
		}
		if (uAns == 'S' || uAns == 's') {
			data.isLong = false;
		} else {
			data.isLong = true;
		}
	}

	// Getting isotope data from files
	try {
		data.halflife = HalfLife(name);  // in days
		data.Atoday = data.A0 * exp((-log(2.0) / data.halflife) * decayTime.days());  // microCurie
		data.CToday = data.Atoday / mass;  // grams for solids liters for liquids
		data.dpm = data.A0 * 2220000 * exp(-(log(2.0) / data.halflife) * decayTime.days()); 
		data.A1 = A1(name);  // TBq
		data.A2 = A2(name);  // TBq
		// Updating ALim to be A1 if Special form or A2 if normal form
		if (form == 'S' || form == 's') {
			data.ALim = data.A1;
		} else {
			data.ALim = data.A2;
		}
		data.exLim = ExLimit(name); // Bq
		data.exCon = ExCon(name);  // Bq/gram
		data.licLim = LicensingLimit(name);  // microCurie
		if ((nature == 'R' || nature == 'r') && (state == 'L' || state == 'l') && (form == 'T' || form == 't')) {
			data.limMult = TritiatedLimQuanLimit(data.Atoday);
			if (data.limMult == -2101) {
				throw runtime_error("AddIso: Something went wrong when calculating the Limited Quantity Package Limit for Tritiated Water.\n");
			}
		}
		else {
			data.limMult = LimQuanMultiplier(state, form);
		}
		data.iaMult = IAMultiplier(state, form);
		data.reportQ = ReportableQ(name);  // Curie
		data.decayC = DecayConstant(name);
		if (form == 'S' || form == 's') {
			data.iaPLim = IAPackageMultiplier(state, form) * data.A1;
		} else {
			data.iaPLim = IAPackageMultiplier(state, form) * data.A2;
		}

		if (data.Atoday  >= (data.reportQ * 1000000.0)) {  // converting Ci to microCurie and comparing with AToday(microCuirie)
			data.isRQ = true;
		} else {
			data.isRQ = false;
		}
		data.RQFrac = data.Atoday / ((data.reportQ * 1000000.0));
		if (data.licLim == -2101) {
			data.licPer = -INFINITY;
			licExempt = boost::gregorian::date(neg_infin);
		} else {
			data.licPer = data.Atoday / data.licLim;
			date_duration dd1(log(data.A0 / data.licLim) / data.decayC);
			if (dd1.days() >= 2913065) {
				licExempt = boost::gregorian::date(9999, 12, 31);
			}
			else if (dd1.days() <= 0) {
				licExempt = today;
			}
			else {
				licExempt = date + dd1;
			}
		}
		date_duration dd2(log(data.A0 * 2220000 / 500) / (log(2)/data.halflife));
		if (dd2.days() >= 2913065)
			decayRelease = boost::gregorian::date(9999, 12, 31);
		else if (dd2.days() <= -618)
			decayRelease = boost::gregorian::date(1400, 1, 1);
		else {
			decayRelease = date + dd2;
		}
	} catch (runtime_error e) {
		cerr << "\nAddIso:\nRuntime_Error: " << e.what() << endl;
	} catch (...) {
		cerr << "\nAddIso: Unknown error in try catch block.\n" << endl;
	}

	if (!_strcmpi(name.c_str(), "U-232") || !_strcmpi(name.c_str(), "U-235") ||
		!_strcmpi(name.c_str(), "Pu-239") || !_strcmpi(name.c_str(), "Pu-241"))
		data.isFissle = true;

	cout << endl;
	isos[name] = data;  // adding isotope to map
}

bool SHIPMENT::validIso(string name) {
	// Declaring variabels
	ifstream inputfile;
	string currentL, fileName = "files/ValidIsotopes.txt";
	regex isotope(name, boost::regex::icase);  // creating a regular expression to search for the isotope
	smatch sm;
	double failure = false;

	//opening file for reading
	inputfile.open(fileName);
	if (!inputfile.is_open())  // making sure file opened correctly
		throw runtime_error("ValidIso: Failed to open " + fileName + " file.\n");

	// searching for isotope in text file list
	for (int line = 1; getline(inputfile, currentL); line++) {
		if (regex_match(currentL, sm, isotope)) {  // if we find the isotope name
			inputfile.close();  // closing file
			return true;
		}
	}
	cout << "Isotope " << name << " is not in the file. Name given is not a valid isotope name for this program.\n\n";
	inputfile.close();  // closing file
	return failure;
}

string SHIPMENT::getClass(string name) {
	switch (isos[name].isoClass) {
	case 0:
		return name + " Classification: Exempt\n";
		break;
	case 1:
		return name + " Classification: Excepted\n";
		break;
	case 2:
		return name + " Classification: Type A\n";
		break;
	case 4:
		return name + " Classification: Type B\n";
		break;
	case 8:
		return name + " Classification: Type B: Highway Route Control Quantity\n";
		break;
	default:
		return "getClass: Error finding" + name + " Classification.\n";
		break;
	}
}

double SHIPMENT::HalfLife(string name){
	// Declaring variabels
	ifstream inputfile;
	string currentL, search = name, fileName = "files/Half_Life(days).txt";
	search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
	regex isotope(search, boost::regex::icase);  // creating a regular expression to search for the isotope
	smatch sm;
	double failure = -2101;

	//opening file for reading
	inputfile.open(fileName);
	if (!inputfile.is_open())  // making sure file opened correctly
		throw runtime_error("HalfLife: Failed to open " + fileName + " file.\n");

	// searching for isotope in text file list
	//cout << "\tFrom HalfLife table:\n\t\t";
	for (int line = 1; getline(inputfile, currentL); line++) {
		if (regex_search(currentL, sm, isotope)) {  // if we find the isotope name
			//cout << "Name: " << name << "\n\t\tHalfLife: " << sm[1] << " days\n";
			inputfile.close();  // closing file
			return stod(sm.str(1));
		}
	}
	cout << "Isotope " << name << "'s halflife is not in the file.\n";
	inputfile.close();  // closing file
	return failure;
}

double SHIPMENT::DecayConstant(string name) {
	// Declaring variabels
	ifstream inputfile;
	string currentL, search = name, fileName = "files/Decay_Constant.txt";
	search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
	regex isotope(search, boost::regex::icase);  // creating a regular expression to search for the isotope
	smatch sm;
	double failure = -2101;

	//opening file for reading
	inputfile.open(fileName);
	if (!inputfile.is_open()) // making sure file opened correctly
		throw runtime_error("DecayConstant: Failed to open " + fileName + " file.\n");

	// searching for isotope in text file list
	//cout << "\tFrom Decay Constant table:\n\t\t";
	for (int line = 1; getline(inputfile, currentL); line++) {
		if (regex_search(currentL, sm, isotope)) {  // if we find the isotope name
			//cout << "Name: " << name << "\n\t\tDecay Constant : " << sm[1] << " days\n";
			inputfile.close();  // closing file
			return stod(sm.str(1));
		}
	}
	cout << "Isotope " << name << "'s decay constant is not in the file.\n";
	inputfile.close();  // closing file
	return failure;
}

double SHIPMENT::A1(string name) {
	// Declaring variabels
	ifstream inputfile;
	string currentL, search = name, fileName = "files/A1(TBq).txt";
	if (!_strcmpi(name.c_str(), "U-230") || !_strcmpi(name.c_str(), "U-232") || !_strcmpi(name.c_str(), "U-233") || !_strcmpi(name.c_str(), "U-234") || !_strcmpi(name.c_str(), "U-236")) {
		search.append(absorption);
		search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
	}
	else if (!_strcmpi(name.c_str(), "Eu-150") || !_strcmpi(name.c_str(), "Np-236") || !_strcmpi(name.c_str(), "Ta-178")) {  // checking if there are two versions of isotope in file (long and short)
		if (iter->second.isLong) {
			search.append(" (long)");
			search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
		} else {
			search.append(" (short)");
			search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
		}
	}
	else {
		search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
	}
	regex isotope(search, boost::regex::icase);  // creating a regular expression to search for the isotope
	smatch sm;
	double failure = -2101;

	//opening file for reading
	inputfile.open(fileName);
	if (!inputfile.is_open()) // making sure file opened correctly
		throw runtime_error("A1: Failed to open " + fileName + " file.\n");

	// searching for isotope in text file list
	//cout << "\tFrom A1 table:\n\t\t";
	for (int line = 1; getline(inputfile, currentL); line++) {
		if (regex_search(currentL, sm, isotope)) {  // if we find the isotope name
			//cout << "Name: " << name << "\n\t\tA1: " << sm[1] << " TBq\n";
			inputfile.close();  // closing file
			return stod(sm.str(1));
		}
	}
	cout << "Isotope " << name << "'s A1 is not in the file.\n";
	inputfile.close();  // closing file
	return failure;
}

double SHIPMENT::A2(string name) {
	// Declaring variabels
	ifstream inputfile;
	string currentL, search = name, fileName = "files/A2(TBq).txt";
	if (!_strcmpi(name.c_str(), "U-230") || !_strcmpi(name.c_str(), "U-232") || !_strcmpi(name.c_str(), "U-233") || !_strcmpi(name.c_str(), "U-234") || !_strcmpi(name.c_str(), "U-236")) {
		search.append(absorption);
		search.append("[\\s]+([\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
	}
	else if (!_strcmpi(name.c_str(), "Eu-150") || !_strcmpi(name.c_str(), "Np-236") || !_strcmpi(name.c_str(), "Ta-178")) {  // checking if there are two versions of isotope in file (long and short)
		if (iter->second.isLong) {
			search.append(" (long)");
			search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
		}
		else {
			search.append(" (short)");
			search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
		}
	}
	else {
		search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
	}
	regex isotope(search, boost::regex::icase);  // creating a regular expression to search for the isotope
	smatch sm;
	double failure = -2101;

	//opening A2 file for reading
	inputfile.open(fileName);
	if (!inputfile.is_open()) // making sure file opened correctly
		throw runtime_error("A2: Failed to open " + fileName + " file.\n");

	// searching for isotope in text file list
	//cout << "\tFrom A2 table:\n\t\t";
	for (int line = 1; getline(inputfile, currentL); line++) {
		if (regex_search(currentL, sm, isotope)) {  // if we find the isotope name
			//cout << "Name: " << name << "\n\t\tA2: " << sm[1] << " TBq\n";
			inputfile.close();  // closing file
			return stod(sm.str(1));
		}
	}
	cout << "Isotope " << name << "'s A2 is not in the file.\n";
	inputfile.close();  // closing file
	return failure;
}

double SHIPMENT::ExLimit(string name) {
	// Declaring variabels
	ifstream inputfile;
	string currentL, search = name, fileName = "files/Exempt_limit(Bq).txt";
	if (!_strcmpi(name.c_str(), "U-230") || !_strcmpi(name.c_str(), "U-232") || !_strcmpi(name.c_str(), "U-233") || !_strcmpi(name.c_str(), "U-234") || !_strcmpi(name.c_str(), "U-236")) {
		search.append(absorption);
		search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
	}
	else if (!_strcmpi(name.c_str(), "Eu-150") || !_strcmpi(name.c_str(), "Np-236") || !_strcmpi(name.c_str(), "Ta-178")) {  // checking if there are two versions of isotope in file (long and short)
		if (iter->second.isLong) {
			search.append(" (long)");
			search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
		}
		else {
			search.append(" (short)");
			search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
		}
	}
	else {
		search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
	}
	regex isotope(search, boost::regex::icase);  // creating a regular expression to search for the isotope
	smatch sm;
	double failure = -2101;

	//opening Exempt Limit file for reading
	inputfile.open(fileName);
	if (!inputfile.is_open()) // making sure file opened correctly
		throw runtime_error("ExLimit: Failed to open " + fileName + " file.\n");

	// searching for isotope in text file list
	//cout << "\tFrom Exempt Limit table:\n\t\t";
	for (int line = 1; getline(inputfile, currentL); line++) {
		if (regex_search(currentL, sm, isotope)) {  // if we find the isotope name
			//cout << "Name: " << name << "\n\t\tExempt Limit: " << sm[1] << " Bq\n";
			inputfile.close();  // closing file
			return stod(sm.str(1));
		}
	}
	cout << "Isotope " << name << "'s Exempt Limit is not in the file.\n";
	inputfile.close();  // closing file
	return failure;
}

double SHIPMENT::ExCon(string name) {
	// Declaring variabels
	ifstream inputfile;
	string currentL, search = name, fileName = "files/Exempt_concentration(Bq-g).txt";
	if (!_strcmpi(name.c_str(), "U-230") || !_strcmpi(name.c_str(), "U-232") || !_strcmpi(name.c_str(), "U-233") || !_strcmpi(name.c_str(), "U-234") || !_strcmpi(name.c_str(), "U-236")) {
		search.append(absorption);
		search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
	}
	else if (!_strcmpi(name.c_str(), "Eu-150") || !_strcmpi(name.c_str(), "Np-236") || !_strcmpi(name.c_str(), "Ta-178")) {  // checking if there are two versions of isotope in file (long and short)
		if (iter->second.isLong) {
			search.append(" (long)");
			search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
		}
		else {
			search.append(" (short)");
			search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
		}
	}
	else {
		search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
	}
	regex isotope(search, boost::regex::icase);  // creating a regular expression to search for the isotope
	smatch sm;
	double failure = -2101;

	//opening Exempt Concentration file for reading
	inputfile.open(fileName);
	if (!inputfile.is_open()) // making sure file opened correctly
		throw runtime_error("ExCon: Failed to open " + fileName + " file.\n");

	// searching for isotope in text file list
	//cout << "\tFrom Exempt Concentration table:\n\t\t";
	for (int line = 1; getline(inputfile, currentL); line++) {
		if (regex_search(currentL, sm, isotope)) {  // if we find the isotope name
			//cout << "Name: " << name << "\n\t\tExempt Concentration: " << sm[1] << " Bq/g\n";
			inputfile.close();  // closing file
			return stod(sm.str(1));
		}
	}
	cout << "Isotope " << name << "'s Exempt Concentration is not in the file.\n";
	inputfile.close();  // closing file
	return failure;
}

double SHIPMENT::LicensingLimit(std::string name) {
	// Declaring variabels
	ifstream inputfile;
	string currentL, search = name, fileName = "files/Licensing_Limit(microCi).txt";
	search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
	regex isotope(search, boost::regex::icase);  // creating a regular expression to search for the isotope
	smatch sm;
	double failure = -2101;

	//opening Exempt Concentration file for reading
	inputfile.open(fileName);
	if (!inputfile.is_open()) // making sure file opened correctly
		throw runtime_error("LicensingLimit: Failed to open " + fileName + " file.\n");

	// searching for isotope in text file list
	//cout << "\tFrom Licensing Limit table:\n\t\t";
	for (int line = 1; getline(inputfile, currentL); line++) {
		if (regex_search(currentL, sm, isotope)) {  // if we find the isotope name
			//cout << "Name: " << name << "\n\t\tLicensing Limit: " << sm[1] << " microCi\n";
			inputfile.close();  // closing file
			return stod(sm.str(1));
		}
	}
	//if(!_strcmpi(name.c_str(), "U-230"))
	cout << "Isotope " << name << "'s licensing limit is not in the file.\n";
	inputfile.close();  // closing file
	return failure;
}

double SHIPMENT::ReportableQ(string name) {
	// Declaring variabels
	ifstream inputfile;
	string currentL, search = name, fileName = "files/Reportable_Quantity(Ci).txt";
	if (!_strcmpi(name.c_str(), "U-230") || !_strcmpi(name.c_str(), "U-232") || !_strcmpi(name.c_str(), "U-233") || !_strcmpi(name.c_str(), "U-234") || !_strcmpi(name.c_str(), "U-236")) {
		search.append(absorption);
		search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
	}
	else if (!_strcmpi(name.c_str(), "Eu-150") || !_strcmpi(name.c_str(), "In-110") || !_strcmpi(name.c_str(), "Nb-89")
		|| !_strcmpi(name.c_str(), "Np-236") || !_strcmpi(name.c_str(), "Re-182") || !_strcmpi(name.c_str(), "Sb-120")
		|| !_strcmpi(name.c_str(), "Sb-128") || !_strcmpi(name.c_str(), "Tb-156m")) {
		if (iter->second.isLong) {
			search.append(" (long)");
			search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
		}
		else {
			search.append(" (short)");
			search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
		}
	}
	else {
		search.append("[\\s]+([-]?[\\d]+(\\.[\\d]+)?+(e[+-][\\d]{1,2})?)");
	}
	regex isotope(search, boost::regex::icase);  // creating a regular expression to search for the isotope
	smatch sm;
	double failure = -2101;

	//opening Reportable Quantity file for reading
	inputfile.open(fileName);
	if (!inputfile.is_open()) // making sure file opened correctly
		throw runtime_error("ReportableQ: Failed to open " + fileName + " file.\n");

	// searching for isotope in text file list
	//cout << "\tFrom Reportable Quantity table:\n\t\t";
	for (int line = 1; getline(inputfile, currentL); line++) {
		if (regex_search(currentL, sm, isotope)) {  // if we find the isotope name
			//cout << "Name: " << name << "\n\t\tReportable Quantity: " << sm[1] << unit << endl;
			inputfile.close();  // closing file
			return stod(sm.str(1));
		}
	}
	cout << "Isotope " << name << "'s Reportable Quantity is not in the file.\n";
	inputfile.close();  // closing file
	return failure;
}

double SHIPMENT::IAMultiplier(char state, char form) {
	// Declaring variabels
	ifstream inputfile;
	string currentL, fileName = "files/IA_Limited_limit.txt";
	string search = "";
	search.insert(0, 1, state);
	search.append(", ");
	search.insert(search.end(), 1, form);
	search.append(" (\\.[\\d]+|[\\d]+|[\\d]+|[\\d]\\.[\\d])");
	regex multiplier(search, boost::regex::icase);  // creating a regular expression to search for the multiplier
	smatch sm;
	double failure = -2101;

	//opening IA Limited limit file for reading
	inputfile.open(fileName);
	if (!inputfile.is_open()) // making sure file opened correctly
		throw runtime_error("IAMultiplier: Failed to open " + fileName + " file.\n");

	// searching for multiplier in text file list
	//cout << "\tFrom Instuments and Articles Limit table:\n\t\t";
	for (int line = 1; getline(inputfile, currentL); line++) {
		if (regex_search(currentL, sm, multiplier)) {  // if we find the multiplier name
			//cout << "State: " << state << " Form: " << form << " Multiplier: " << sm[1] << " TBq\n";
			inputfile.close();  // closing file
			return stod(sm[1]);
		}
	}
	cout << "State: " << state << " and Form: " << form << " could not be found in the file.\n";
	inputfile.close();  // closing file
	return failure;
}

double SHIPMENT::IAPackageMultiplier(char state, char form) {
	// Declaring variabels
	ifstream inputfile;
	string currentL, fileName = "files/IA_Package_limit.txt";
	string search = "";
	search.insert(0, 1, state);
	search.append(", ");
	search.insert(search.end(), 1, form);
	search.append(" (\\.[\\d]+|[\\d]+|[\\d]+|[\\d]\\.[\\d])");
	regex multiplier(search, boost::regex::icase);  // creating a regular expression to search for the multiplier
	smatch sm;
	double failure = -2101;

	//opening IA Limited limit file for reading
	inputfile.open(fileName);
	if (!inputfile.is_open()) // making sure file opened correctly
		throw runtime_error("IAPackageMultiplier: Failed to open " + fileName + " file.\n");

	// searching for multiplier in text file list
	//cout << "\tFrom Instuments and Articles Limit table:\n\t\t";
	for (int line = 1; getline(inputfile, currentL); line++) {
		if (regex_search(currentL, sm, multiplier)) {  // if we find the multiplier name
			//cout << "State: " << state << " Form: " << form << " Multiplier: " << sm[1] << " TBq\n";
			inputfile.close();  // closing file
			return stod(sm[1]);
		}
	}
	cout << "State: " << state << " and Form: " << form << " could not be found in the file.\n";
	inputfile.close();  // closing file
	return failure;
}

double SHIPMENT::LimQuanMultiplier(char state, char form) {
	// Declaring variabels
	ifstream inputfile;
	string currentL, fileName = "files/Limited_limit.txt";
	string search = "";
	search.insert(0, 1, state);
	search.append(", ");
	
	search.insert(search.end(), 1, form);
	search.append(" (\\.[\\d]+|[\\d]+|[\\d]+|[\\d]\\.[\\d])");

	regex multiplier(search, boost::regex::icase);  // creating a regular expression to search for the multiplier
	smatch sm;
	double failure = -2101;

	//opening IA Limited limit file for reading
	inputfile.open(fileName);
	if (!inputfile.is_open()) // making sure file opened correctly
		throw runtime_error("LimQuanMultiplier: Failed to open " + fileName + " file.\n");

	// searching for multiplier in text file list
	//cout << "\tFrom Limited Quantities Limit table:\n\t\t";
	for (int line = 1; getline(inputfile, currentL); line++) {
		if (regex_search(currentL, sm, multiplier)) {  // if we find the multiplier name
			//cout << "State: " << state << " Form: " << form << " Multiplier: " << sm[1] << endl;
			inputfile.close();  // closing file
			return stod(sm[1]);
		}
	}
	cout << "State: " << state << " and Form: " << form << " could not be found in the file.\n";
	inputfile.close();  // closing file
	return failure;
}

double SHIPMENT::TritiatedLimQuanLimit(double AToday) {
	double TBqperL = (AToday * 3.7e-8) / mass;
	if (TBqperL < 0.0037) {
		return 37;  
	}
	else if (TBqperL > 0.037) {
		return 0.037;
	}
	else {
		return 3.7;
	}

	return -2101;
}

bool SHIPMENT::exemptClass(string name) {
	// Declaring variabels
	double convertedA, percent = 0;

	// cout << "\n### Testing for exempt classification ###\n";

	//converting microCurie to Bq
	convertedA = isos[name].Atoday * 37000;

	// checking if isotope exceeds limit
	if (convertedA > isos[name].exLim) {
		/*cout << "\t\tAToday (Bq): " << convertedA << " > Actvity Limit (Bq): " << isos[name].exLim << endl;
		cout << "\t**Isotope " << name << " exceeds limit to be classified as exempt**\n";*/
		return false;
	}
	else {
		//cout << "\t\tAToday (Bq): " << convertedA << " =< Actvity Limit (Bq): " << isos[name].exLim << endl;
		isos[name].limPer = convertedA / isos[name].exLim;
		/*cout << "\t\tPercentage of Exempt Limit: " << isos[name].limPer * 100 << " %\n";
		cout << "\t\t**Isotope " << name << " falls under the exempt limit.**\n\n";*/

		// checking if isotope exceeds concentration
		percent = (convertedA / mass);
		if (percent > isos[name].exCon) {
			/*cout << "\t\tConcentration: " << percent << "% > Concentration Limit: " << isos[name].exCon 
				<< "%\n\n\t\t**Isotope " << name
				<< " falls under the exempt limit but**\n\t\t**does not fall below the concentration limit**\n\t\t"
				<< "**therefore cannot be classified as exempt.**\n\n";*/
			return false;
		}
		else {
			isos[name].conPer = convertedA / mass;
			/*cout << "\t\tPercentage of concentration: " << isos[name].conPer * 100 << "%\n\t\t"
				<< isos[name].conPer * 100 << "% <= Concentration Limit: " << isos[name].exCon << "%\n\n\t\t**Isotope " << name
				<< " falls under both the exempt limit and**\n\t\t"
				<< "**the concentration limit and can be classified as Exempt.**\n\n";*/
			return true;
		}
	}
}

bool SHIPMENT::limitedClass(string name) {
	// Declaring variabels
	double limit, multiplier, A, convertedA;

	//cout << "\n### Testing for limited classification ###\n";

	// converting A0 to TBq
	convertedA = isos[name].Atoday * 3.7e-8;

	// Getting A1 or A2 based on form
	A = isos[name].ALim;

	// Getting limit multiplier based on nature
	if (nature == 'R' || nature == 'r')
		multiplier = isos[name].limMult;
	else {
		multiplier = isos[name].iaMult;
	}

	// calculating limit and saving it
	if ((nature == 'R' || nature == 'r') && (state == 'L' || state == 'l') && (form == 'T' || form == 't')) {
		limit = multiplier;
	}
	else {
		limit = A * multiplier;
	}
	isos[name].limLim = limit;

	// checking if isotope exceeds limit
	if (convertedA > limit) {
		if (nature == 'R' || nature == 'r') {
			//cout << "\t\tAToday: " << convertedA << " TBq > Activity Limit: " << limit << " TBq\n";
			//cout << "\t**Isotope " << name << " exceeds limit to be classified as Limited Quantity**\n";
			return false;
		}
		else {
			//cout << "\t\tAToday: " << convertedA << " TBq > Activity Limit: " << limit << " TBq\n";
			//cout << "\t**Isotope " << name << " exceeds limit to be classified as Limited Instrument & Article**\n";
			return false;
		}
	}
	else {
		isos[name].limPer = convertedA / limit;
		if (nature == 'R' || nature == 'r') {
			//cout << "\t\tAToday: " << convertedA << " TBq =< Activity Limit: " << limit << " TBq\n" 
			//	<< "\t\tPercentage of limit: " << isos[name].limPer << "%\n\t\t**Isotope "
			//	<< name << " falls under the limit and**\n\t\t"
			//	<< "**can be classified as a limited quantity and packaged as Excepted**\n";
			return true;
		}
		else {
			//cout << "\t\tAToday: " << convertedA << " TBq =< Actvity Limit: " << limit << " TBq\n" 
			//	<< "\t\tPercentage of limit: " << isos[name].limPer << "%\n\t\t**Isotope "
			//	<< name << " falls under the limit and**\n\t\t"
			//	<< "**can be classified as limited instrument & article and packaged as Excepted**\n";
			return true;
		}
	}
}

bool SHIPMENT::typeAClass(string name) {
	// Declaring variables
	double A, convertedA;

	//cout << "\n### Testing for Type A classification ###\n";

	// Getting A1 or A2 based on form
	A = isos[name].ALim;

	// converting A0 to TBq
	convertedA = isos[name].Atoday * 3.7e-8;

	// Checking if isotope is type A 
	if (convertedA > A) {
		//cout << "\t\tAToday: " << convertedA << " TBq > A Limit: " << A << " TBq\n";
		//cout << "\t**Isotope " << name << " exceeds the limit to be classified as Type A**\n";
		return false;
	}
	else {
		isos[name].limPer = convertedA / A;
		//cout << "\t\tPercentage of limit: " << isos[name].limPer << "%\n\t\t"
		//	 << "AToday: " << convertedA << " TBq =< A Limit: " << A << " TBq\n\t\t**Isotope "
		//	<< name << " falls under the limit and**\n\t\t"
		//	<< "**can be classified as Type A.**";
		return true;
	}
}

bool SHIPMENT::typeBClass(string name) {
	// Declaring variables
	double A, convertedA;

	//cout << "\n### Testing for Type B classification ###\n";

	// Getting A1 or A2 based on form
	A = isos[name].ALim;

	// converting A0 to TBq
	convertedA = isos[name].Atoday * 3.7e-8;

	// Checking if isotope is type B 
	if (convertedA <= A) {
		/*cout << "\t\tPercentage of limit: " << percent << "%\n\t\t"
			<< "\t\t" << convertedA << " TBq <= " << A << " TBq\n\t**Isotope "
			<< name << " falls under the minimum limit to be classified as Type B**\n";*/
		return false;
	}
	else {
		/*cout << "\t\tPercentage of limit: " << percent << "%\n\t\t"
			<< "\t\t" << convertedA << " TBq > " << A << " TBq\n\t\t**Isotope "
			<< name << " is over the A limit and can be classified as Type B.**\n";*/
		return true;
	}
}

bool SHIPMENT::HRCQClass(string name) {
	// Declaring variables
	double A, convertedA, percent, 
		   hrcqLim = 1000; // hrcqLim is in TBq

	//cout << "\n### Testing for Highway Route Control Quantity classification ###\n";

	// Getting A1 or A2 based on form
	A = isos[name].ALim;

	// Getting HRCQ limit (whichever is the least of (3000 * A1 or A2) and (1000 TBq))
	if ((3000 * A) < hrcqLim) {
		hrcqLim = 3000 * A;
	}

	// saving HRCQ limit
	isos[name].hrcqLim = hrcqLim;

	// converting A0 to TBq
	convertedA = isos[name].Atoday * 3.7e-8;

	// Checking if isotope is a highway route control quantity 
	percent = convertedA / hrcqLim;  // percentage of the hrcq limit
	if (convertedA <= hrcqLim) {  // Activity is under limit
		return false;
	}
	else {
		return true;
	}
}

string SHIPMENT::findClass() {
	int hClass = 0;
	double pLimit = 0, pCon = 0, aCon = 0, pAToday = 0, pRQ = 0,
		           aFracEx = 0, aFracLim = 0, aFracA = 0, aFracB = 0, 
		           aFracHRCQ = 0, lFrac = 0, pLicLimit = 0, typeALim = 0;
	string retVal;

	// Getting highest Classification between all of the isotopes in the shipment
	for (iter = isos.begin(); iter != isos.end(); iter++) {
		if (iter->second.isoClass > hClass)
			hClass = iter->second.isoClass;
	}

	// Getting shipment limit percentage and shipment concentration and shipment licencing exemption
	for (iter = isos.begin(); iter != isos.end(); iter++) {
		pLimit += iter->second.limPer;  // adding each isotipe's percentage of the activity limit
		pCon += iter->second.CToday * 37000;  // adding each isotope's concentration today (in Bq)
		pAToday += iter->second.Atoday * 37000;  // adding each isotope's activity today (in Bq)
		lFrac += (iter->second.Atoday * 37000) / ((iter->second.licLim * 1000) * 37000);  // isotope activity in Bq / (1000 * licensing limit) in Bq 
	}

	// calculating package limits for Exempt, Excepted, Type A Classification
	for (iter = isos.begin(); iter != isos.end(); iter++) {
		iter->second.aCon = (iter->second.CToday * 37000) / pCon;  // fraction of activity concentration of isotope (Bq)
		iter->second.aFrac = (iter->second.Atoday * 37000) / pAToday;  // fraction of activity of isotope (Bq)
		aCon += iter->second.aCon / iter->second.exCon;  // fraction of activity concentration of isotope (Bq) / Exempt concentration limit (Bq/g)
		aFracEx += iter->second.aFrac / iter->second.exLim;  // fraction of activity of isotope (Bq) / Exempt Activity limit (Bq)
		aFracLim += iter->second.aFrac / (iter->second.ALim * pow(10.0, 12.0));  // package activity limit for Excepted Classification
		typeALim += (iter->second.Atoday * 37000) / (iter->second.ALim * 1e+12); // converting Activity today to Bq then dividing by A1 or A2 converted to Bq
		pRQ += iter->second.RQFrac;  // sum(individual nuclide activity (in microCi) / individual nuclide reportable quantity (in microCi))
	}

	if (pRQ < 1) {  // finding out if package is a reportable quantity
		isPackageRC = false;
	} else {
		isPackageRC = true;
	}

	aCon = 1 / aCon;  // Exempt Activity Concentration Limit for Package
	aFracEx = 1 / aFracEx;  // Exempt activity limit for Package
	aFracLim = 1 / aFracLim;  // Excepted activity limit for Package

	if (pLimit > 1 || typeALim > 1) {  // if package limit or typeALim is greater than 1 then move to next classification
		if (hClass == 0)
			hClass = 1;
		else {
			hClass = hClass << 1;
		}
	}

	if (lFrac > 1) {  // checking if shipment is exempt from licensing
		retVal = "Shipment exempt from licensing?: No\n";
	} else {
		retVal = "Shipment exempt from licensing?: Yes\n";
	}

	if (isPackageRC) {
		retVal.append("Shipemnt Reportable Quantity?: Yes\n");
	} else {
		retVal.append("Shipemnt Reportable Quantity?: No\n");
	}

	switch (hClass) {
	case 0:
		sClass = 0;  // saving the shipment class
		retVal.append("Shipment Classification: Exempt:\n\tShipment Activity Limit: ");
		retVal.append(std::to_string(aFracEx));
		retVal.append(" Bq (Actual Activity: " + std::to_string(pAToday) + ")\n\tShipment Activity Concentration Limit: ");
		retVal.append(std::to_string(aCon));
		retVal.append(" Bq/g (Actual Concentraion: " + std::to_string(pCon) + ")\n\tShipment License Percentage: ");
		retVal.append(std::to_string(lFrac * 100) + "%\n");
		return retVal;
		break;
	case 1:
		sClass = 1;  // saving the shipment class
		retVal.append("Shipment Classification: Excepted:\n\tShipment Activity Limit: ");
		retVal.append(std::to_string(aFracLim));
		retVal.append(" Bq (Actual Activity: " + std::to_string(pAToday) + ")\n\tShipment License Percentage: ");
		retVal.append(std::to_string(lFrac * 100) + "%\n");
		return retVal;
		break;
	case 2:
		sClass = 2;  // saving the shipment class
		retVal.append("Shipment Classification: Type A:\n\tShipment Activity: ");
		retVal.append(std::to_string(pAToday));
		retVal.append(")\n\tShipment License Percentage: ");
		retVal.append(std::to_string(lFrac * 100) + "%\n");
		return retVal;
		break;
	case 4:
		sClass = 4;  // saving the shipment class
		retVal.append("Shipment Classification: Tyep B:\n\tShipment Activity: ");
		retVal.append(std::to_string(pAToday));
		retVal.append(")\n\tShipment License Percentage: ");
		retVal.append(std::to_string(lFrac * 100) + "%\n");
		return retVal;
		break;
	case 8:
		sClass = 8;  // saving the shipment class
		retVal.append("Shipment Classification: Type B: Highway Route Control:\n\tShipment Activity: ");
		retVal.append(std::to_string(pAToday));
		retVal.append(")\n\tShipment License Percentage: ");
		retVal.append(std::to_string(lFrac * 100) + "%\n");
		return retVal;
		break;
	default:
		return "FindClass: Error finding Shipment Classification.\n";
		break;
	}
}

date SHIPMENT::dateOfExempt(int limit) {
	double  oldA, oldDPM, oldLP, oldC, oldRQFrac;
	date_duration oldDT;
	int oldClass = sClass;
	if(sClass == 0)
		return today;
	else {
		int incr = 1;
		// saving original isotope data
		for (iter = isos.begin(); iter != isos.end(); iter++) {
			oldA = iter->second.Atoday;  // in microCuries
			oldC = iter->second.CToday;
			oldDPM = iter->second.dpm;
			oldLP = iter->second.licPer;
			oldRQFrac = iter->second.RQFrac;
		}

		//  saving original 
		oldDT = decayTime;

		// calculating the class for days until 
		while (sClass > 0 && incr <= limit) {
			//  getting number of days since t0
			date_duration dd(incr);
		    newT = today + dd;
			date_period dp(this->date, newT);  // (today + incr) - date
			decayTime = dp.length();  // updating decay time

			// changing isotope data for new day
			for (iter = isos.begin(); iter != isos.end(); iter++) {
				iter->second.Atoday = iter->second.A0 * exp((-log(2.0) / iter->second.halflife) * decayTime.days());
				iter->second.CToday = iter->second.Atoday / mass;
				iter->second.dpm = iter->second.A0 * 2220000 * exp((-log(2.0) / iter->second.halflife) * decayTime.days());
				if (iter->second.Atoday >= (iter->second.reportQ * 1000000.0)) {  // Activity Today (in microCi) >= Reportable Quantity (in microCi)
					iter->second.isRQ = true;
				} else {
					iter->second.isRQ = false;
				}
				iter->second.RQFrac = iter->second.Atoday / (iter->second.reportQ * 1000000.0);
				if (oldLP == -INFINITY) {
					iter->second.licPer = -INFINITY;
				} else {
					iter->second.licPer = iter->second.Atoday / iter->second.licLim;
				}
			}

			// finding classification of isotopes in shipment
			bool lim;
			for (iter = isos.begin(); iter != isos.end(); iter++) {
				if (exemptClass(iter->first)) {
					iter->second.isoClass = 0;
				} else {
					if (lim = limitedClass(iter->first))
						iter->second.isoClass = 1;
					else if (typeAClass(iter->first) && lim == false)
						iter->second.isoClass = 2;
					else if (HRCQClass(iter->first))
						iter->second.isoClass = 8;
					else if (typeBClass(iter->first))
						iter->second.isoClass = 4;
				}
			}

			// finding classification of shipment
			findClass();
			incr++;
		}
		
		// resetting isotope data
		for (iter = isos.begin(); iter != isos.end(); iter++) {
			iter->second.Atoday = oldA;
			iter->second.CToday = oldC;
			iter->second.dpm = oldDPM;
			iter->second.licPer = oldLP;
			if (oldA >= (iter->second.reportQ * 1000000.0)) {  // converting Ci to microCurie and comparing with AToday(microCuirie)
				iter->second.isRQ = true;
			} else {
				iter->second.isRQ = false;
			}
			iter->second.RQFrac = oldA / (iter->second.reportQ * 1000000.0);
			if (oldLP == -INFINITY) {
				iter->second.licPer = -INFINITY;
			} else {
				iter->second.licPer = iter->second.Atoday / iter->second.licLim;
			}
		}
		decayTime = oldDT;
		sClass = oldClass;
		
		if (incr > limit && sClass != 0) {
			return boost::gregorian::date(pos_infin);
		} else {
			return newT;
		}
	}
}

date SHIPMENT::dateOfExcepted(int limit) {
	double  oldA, oldDPM, oldLP, oldC, oldRQFrac;
	date_duration oldDT;
	int oldClass = sClass;
	if (sClass == 1)
		return today;
	else if (sClass < 1)
		return boost::gregorian::date(neg_infin);
	
	else {
		int incr = 1;
		// saving original isotope data
		for (iter = isos.begin(); iter != isos.end(); iter++) {
			oldA = iter->second.Atoday;
			oldC = iter->second.CToday;
			oldDPM = iter->second.dpm;
			oldLP = iter->second.licPer;
			oldRQFrac = iter->second.RQFrac;
		}

		//  saving original 
		oldDT = decayTime;

		while (sClass > 1 || incr > limit) {
			//  getting number of days since t0
			date_duration dd(incr);
			newT = today + dd;
			date_period dp(this->date, newT);  // (today + incr) - date
			decayTime = dp.length();  // updating decay time

			// changing isotope data for new day
			for (iter = isos.begin(); iter != isos.end(); iter++) {
				iter->second.Atoday = iter->second.A0 * exp((-log(2.0) / iter->second.halflife) * decayTime.days());
				iter->second.CToday = iter->second.Atoday / mass;
				iter->second.dpm = iter->second.A0 * 2220000 * exp((-log(2.0) / iter->second.halflife) * decayTime.days());
				iter->second.licPer = iter->second.Atoday / iter->second.licLim;
				if (iter->second.Atoday >= (iter->second.reportQ * 1000000.0)) {
					iter->second.isRQ = true;
				} else {
					iter->second.isRQ = false;
				}
				iter->second.RQFrac = iter->second.Atoday / (iter->second.reportQ * 1000000.0);
				if (oldLP == -INFINITY) {
					iter->second.licPer = -INFINITY;
				} else {
					iter->second.licPer = iter->second.Atoday / iter->second.licLim;
				}
			}

			// finding classification of isotopes in shipment
			bool lim;
			for (iter = isos.begin(); iter != isos.end(); iter++) {
				if (exemptClass(iter->first)) {
					iter->second.isoClass = 0;
				}
				else {
					if (lim = limitedClass(iter->first))
						iter->second.isoClass = 1;
					else if (typeAClass(iter->first) && lim == false)
						iter->second.isoClass = 2;
					else if (HRCQClass(iter->first))
						iter->second.isoClass = 8;
					else if (typeBClass(iter->first))
						iter->second.isoClass = 4;
				}
			}

			// finding classification of shipment
			findClass();
			incr++;
		}
		
		// resetting isotope data
		for (iter = isos.begin(); iter != isos.end(); iter++) {
			iter->second.Atoday = oldA;
			iter->second.CToday = oldC;
			iter->second.dpm = oldDPM;
			iter->second.licPer = oldLP;
			if (oldA >= (iter->second.reportQ * 1000000.0)) {
				iter->second.isRQ = true;
			}
			else {
				iter->second.isRQ = false;
			}
			iter->second.RQFrac = oldA / (iter->second.reportQ * 1000000.0);
			if (oldLP == -INFINITY) {
				iter->second.licPer = -INFINITY;
			}
			else {
				iter->second.licPer = iter->second.Atoday / iter->second.licLim;
			}
		}
		decayTime = oldDT;
		sClass = oldClass;

		if (incr > limit && sClass != 1)
			return boost::gregorian::date(pos_infin);
		else {
			return newT;
		}
	}
}

date SHIPMENT::dateOfTypeA(int limit) {
	double  oldA, oldDPM, oldLP, oldC, oldRQFrac;
	date_duration oldDT;
	int oldClass = sClass;
	if (sClass == 2)
		return today;
	else if (sClass < 2)
		return boost::gregorian::date(neg_infin);
	 
	else {
		int incr = 1;
		// saving original isotope data
		for (iter = isos.begin(); iter != isos.end(); iter++) {
			oldA = iter->second.Atoday;
			oldC = iter->second.CToday;
			oldDPM = iter->second.dpm;
			oldLP = iter->second.licPer;
			oldRQFrac = iter->second.RQFrac;
		}

		//  saving original 
		oldDT = decayTime;
		while (sClass > 2 || incr > limit) {
			//  getting number of days since t0
			date_duration dd(incr);
			newT = today + dd;
			date_period dp(this->date, newT);  // (today + incr) - date
			decayTime = dp.length();  // updating decay time

			// changing isotope data for new day
			for (iter = isos.begin(); iter != isos.end(); iter++) {
				iter->second.Atoday = iter->second.A0 * exp((-log(2.0) / iter->second.halflife) * decayTime.days());
				iter->second.CToday = iter->second.Atoday / mass;
				iter->second.dpm = iter->second.A0 * 2220000 * exp((-log(2.0) / iter->second.halflife) * decayTime.days());
				iter->second.licPer = iter->second.Atoday / iter->second.licLim;
				if (iter->second.Atoday >= (iter->second.reportQ * 1000000.0)) {
					iter->second.isRQ = true;
				} else {
					iter->second.isRQ = false;
				}
				iter->second.RQFrac = iter->second.Atoday / (iter->second.reportQ * 1000000.0);
				if (oldLP == -INFINITY) {
					iter->second.licPer = -INFINITY;
				} else {
					iter->second.licPer = iter->second.Atoday / iter->second.licLim;
				}
			}

			// finding classification of isotopes in shipment
			bool lim;
			for (iter = isos.begin(); iter != isos.end(); iter++) {
				if (exemptClass(iter->first)) {
					iter->second.isoClass = 0;
				} 
				else {
					if (lim = limitedClass(iter->first))
						iter->second.isoClass = 1;
					else if (typeAClass(iter->first) && lim == false)
						iter->second.isoClass = 2;
					else if (HRCQClass(iter->first))
						iter->second.isoClass = 8;
					else if (typeBClass(iter->first))
						iter->second.isoClass = 4;
				}
			}

			// finding classification of shipment
			findClass();
			incr++;
		}

		// resetting isotope data
		for (iter = isos.begin(); iter != isos.end(); iter++) {
			iter->second.Atoday = oldA;
			iter->second.CToday = oldC;
			iter->second.dpm = oldDPM;
			iter->second.licPer = oldLP;
			if (oldA >= (iter->second.reportQ * 1000000.0)) {
				iter->second.isRQ = true;
			}
			else {
				iter->second.isRQ = false;
			}
			iter->second.RQFrac = oldA / (iter->second.reportQ * 1000000.0);
			if (oldLP == -INFINITY) {
				iter->second.licPer = -INFINITY;
			}
			else {
				iter->second.licPer = iter->second.Atoday / iter->second.licLim;
			}
		}
		decayTime = oldDT;
		sClass = oldClass;

		if (incr > limit && sClass != 2)
			return boost::gregorian::date(pos_infin);
		else {
			return newT;
		}
	}
}

date SHIPMENT::dateOfTypeB(int limit) {
	double  oldA, oldDPM, oldLP, oldC, oldRQFrac;
	date_duration oldDT;
	int oldClass = sClass;
	if (sClass == 4)
		return today;
	else if (sClass < 4)
		return boost::gregorian::date(neg_infin);

	else {
		int incr = 1;
		// saving original isotope data
		for (iter = isos.begin(); iter != isos.end(); iter++) {
			oldA = iter->second.Atoday;
			oldC = iter->second.CToday;
			oldDPM = iter->second.dpm;
			oldLP = iter->second.licPer;
			oldRQFrac = iter->second.RQFrac;
		}

		//  saving original 
		oldDT = decayTime;
		while (sClass > 4 || incr > limit) {
			//  getting number of days since t0
			date_duration dd(incr);
			newT = today + dd;
			date_period dp(this->date, newT);  // (today + incr) - date
			decayTime = dp.length();  // updating decay time

			// changing isotope data for new day
			for (iter = isos.begin(); iter != isos.end(); iter++) {
				iter->second.Atoday = iter->second.A0 * exp((-log(2.0) / iter->second.halflife) * decayTime.days());
				iter->second.CToday = iter->second.Atoday / mass;
				iter->second.dpm = iter->second.A0 * 2220000 * exp((-log(2.0) / iter->second.halflife) * decayTime.days());
				iter->second.licPer = iter->second.Atoday / iter->second.licLim;
				if (iter->second.Atoday >= (iter->second.reportQ * 1000000.0)) {
					iter->second.isRQ = true;
				} else {
					iter->second.isRQ = false;
				}
				iter->second.RQFrac = iter->second.Atoday / (iter->second.reportQ * 1000000.0);
				if (oldLP == -INFINITY) {
					iter->second.licPer = -INFINITY;
				} else {
					iter->second.licPer = iter->second.Atoday / iter->second.licLim;
				}
			}

			// finding classification of isotopes in shipment
			bool lim;
			for (iter = isos.begin(); iter != isos.end(); iter++) {
				if (exemptClass(iter->first)) {
					iter->second.isoClass = 0;
				}
				else {
					if (lim = limitedClass(iter->first))
						iter->second.isoClass = 1;
					else if (typeAClass(iter->first) && lim == false)
						iter->second.isoClass = 2;
					else if (HRCQClass(iter->first))
						iter->second.isoClass = 8;
					else if (typeBClass(iter->first))
						iter->second.isoClass = 4;
				}
			}

			// finding classification of shipment
			findClass();
			incr++;
		}

		// resetting isotope data
		for (iter = isos.begin(); iter != isos.end(); iter++) {
			iter->second.Atoday = oldA;
			iter->second.CToday = oldC;
			iter->second.dpm = oldDPM;
			iter->second.licPer = oldLP;
			if (oldA >= (iter->second.reportQ * 1000000.0)) {
				iter->second.isRQ = true;
			}
			else {
				iter->second.isRQ = false;
			}
			iter->second.RQFrac = oldA / (iter->second.reportQ * 1000000.0);
			if (oldLP == -INFINITY) {
				iter->second.licPer = -INFINITY;
			}
			else {
				iter->second.licPer = iter->second.Atoday / iter->second.licLim;
			}
		}
		decayTime = oldDT;
		sClass = oldClass;

		if (incr > limit && sClass != 4)
			return boost::gregorian::date(pos_infin);
		else {
			return newT;
		}
	}
}

date SHIPMENT::dateOfHRCQ(int limit) {
	// Calculations under the assumtion that HRCQ is the highest shipment classification
	if (sClass == 8)  // Either shipment is a Highway Route Control Quantity
		return today;
	else  // or the shipment is not
		return boost::gregorian::date(neg_infin);
}


void SHIPMENT::printData(string name) {
	cout << "********** Isotope Data **********\n";
	if (isos.find(name)->first == name) {
		cout << "Name: " << name
			<< "\nInitial Activity: " << getA0(name) 
			<< " microCi\nActivity Today (" << today << "): " 
			<< getAToday(name) * 37000 << " Bq (Decay Time: " 
			<< decayTime.days() << " days)\n";
		switch (getNat(name)) {
		case 'R':
		case 'r':
			cout << "Nature: Regular\n";
			break;
		case 'I':
		case 'i':
			cout << "Nature: Instrument\n";
			break;
		case 'A':
		case 'a':
			cout << "Nature: Article\n";
			break;
		default:
			cout << "Nature Invalid\n";
			break;
		}
		switch (getState(name)) {
		case 'S':
		case 's':
			cout << "State: Solid\n";
			switch (getForm(name)) {
			case 'S':
			case 's':
				cout << "Form: Special\n";
				break;
			case 'N':
			case 'n':
				cout << "Form: Normal\n";
				break;
			default:
				cout << "Form invalid\n";
				break;
			}
			break;
		case 'L':
		case 'l':
			cout << "State: Liquid\n";
			switch (getForm(name)) {
			case 'T':
			case 't':
				cout << "Form: Tritiated\n";
				break;
			case 'O':
			case 'o':
				cout << "Form: Other\n";
				break;
			default:
				cout << "Form Invalid\n";
				break;
			}
			break;
		case 'G':
		case 'g':
			cout << "State: Gas\n";
			switch (getForm(name)) {
			case 'T':
			case 't':
				cout << "Form: Tritium\n";
				break;
			case 'S':
			case 's':
				cout << "Form: Special\n";
				break;
			case 'N':
			case 'n':
				cout << "Form: Normal\n";
				break;
			default:
				cout << "Form Invalid\n";
				break;
			}
			break;
		default:
			cout << "State Invalid\n";
			break;
		}
		cout << "Half-Life (days): " << isos.find(name)->second.halflife << endl
		     << "Decay Constant (d-1): " << isos.find(name)->second.decayC << endl
			<< "DPM: " << isos.find(name)->second.dpm << endl;
		if (!absorption.empty()) {
			if (_strcmpi(absorption.c_str(),  "f") == 0)
				cout << "Lung Absorption: Fast" << endl;
			if (_strcmpi(absorption.c_str(),  "m") == 0)
				cout << "Lung Absorption: Medium" << endl;
			if (_strcmpi(absorption.c_str(),  "s") == 0)
				cout << "Lung Absorption: Slow" << endl;
		}
		cout << getClass(name)
			<< "Reportable Quantity?: ";
		if (isos.find(name)->second.isRQ) {
			cout << "Yes" << endl << endl;
		}
		else {
			cout << "No" << endl << endl;
		}
		cout << "Percentages: \n\tPercentage of Licensing Limit: " << isos.find(name)->second.licPer * 100
			 << "%\n\tPercentage of Activity Limit: " << isos.find(name)->second.limPer * 100 << "%\n" << endl << endl
			 << "Exempt from Licensing on " << licExempt << endl << "Decay Date (< 500dpm): "
			 << decayRelease << endl;
	}
	else {
		cout << "PrintData: Couldn't find " << name << "in isotope list./n";
	}
	cout << "**********************************\n";
}

// See *https://stackoverflow.com/questions/7162457/how-to-convert-boostgregoriandate-to-mm-dd-yyyy-format-and-vice-versa*
string dateAsMonthDDYYYY(const boost::gregorian::date& date) {
	const std::locale fmt(std::locale::classic(),
		new boost::gregorian::date_facet("%B %d, %Y"));
	std::ostringstream os;
	os.imbue(fmt);
	os << date;
	return os.str();
}

string dateWithWeekday(const boost::gregorian::date& date) {
	const std::locale fmt(std::locale::classic(),
		new boost::gregorian::date_facet("%A %B %d, %Y"));
	std::ostringstream os;
	os.imbue(fmt);
	os << date;
	return os.str();
}

std::ostream& operator<<(std::ostream &out, SHIPMENT &shipment) {
	int num = 1;
	out << "********** Shipment Data **********\n";
	out << "Initial Date for A0 (t0): " << dateWithWeekday(shipment.date) << "\nMass of Source (g): "
		<< shipment.mass << endl;
	switch (shipment.nature) {
	case 'R':
	case 'r':
		out << "Nature: Regular\n";
		break;
	case 'I':
	case 'i':
		out << "Nature: Instrument\n";
		break;
	case 'A':
	case 'a':
		out << "Nature: Article\n";
		break;
	default:
		out << "Nature Invalid\n";
		break;
	}
	switch (shipment.state) {
	case 'S':
	case 's':
		out << "State: Solid\n";
		switch (shipment.form) {
		case 'S':
		case 's':
			out << "Form: Special\n";
			break;
		case 'N':
		case 'n':
			out << "Form: Normal\n";
			break;
		default:
			cout << "Form invalid\n";
			break;
		}
		break;
	case 'L':
	case 'l':
		out << "State: Liquid\n";
		switch (shipment.form) {
		case 'T':
		case 't':
			out << "Form: Tritiated\n";
			break;
		case 'O':
		case 'o':
			out << "Form: Other\n";
			break;
		default:
			out << "Form Invalid\n";
			break;
		}
		break;
	case 'G':
	case 'g':
		out << "State: Gas\n";
		switch (shipment.form) {
		case 'T':
		case 't':
			out << "Form: Tritium\n";
			break;
		case 'S':
		case 's':
			out << "Form: Special\n";
			break;
		case 'N':
		case 'n':
			out << "Form: Normal\n";
			break;
		default:
			out << "Form Invalid\n";
			break;
		}
		break;
	default:
		out << "State Invalid\n";
		break;
	}
	out << shipment.sClassOut;
	for (shipment.iter = shipment.isos.begin(); shipment.iter != shipment.isos.end(); shipment.iter++) {
		out << "\n#####Isotope " << num << ":#####\n"
			<< "Name: " << shipment.iter->first 
			<< "\nInitial Activity: " << shipment.iter->second.A0 << " microCi"
			<< "\nActivity today (" << dateAsMonthDDYYYY(shipment.today) << "): " 
			<< shipment.iter->second.Atoday * 37000 << " Bq (Decay Time: " << shipment.decayTime.days() 
			<< " days)\n";
		out << "Half-Life (days): " << shipment.iter->second.halflife << endl;
		out << "Decay Constant (d-1): " << shipment.iter->second.decayC << endl
			<< "DPM: " << shipment.iter->second.dpm << endl;
		if(!shipment.absorption.empty()){
			if (_strcmpi(shipment.absorption.c_str(),  "f") == 0)
				out << "Lung Absorption: Fast" << endl;
			if (_strcmpi(shipment.absorption.c_str(),  "m") == 0)
				out << "Lung Absorption: Medium" << endl;
			if (_strcmpi(shipment.absorption.c_str(),  "s") == 0)
				out << "Lung Absorption: Slow" << endl;
		}
		out << shipment.getClass(shipment.iter->first)
			<< "Reportable Quantity?: ";
		if (shipment.iter->second.isRQ) {
			out << "Yes" << endl << endl;
		}
		else {
			out << "No" << endl << endl;
		}
		out << "Percentages: \n\tPercentage of Licensing Limit: " << shipment.iter->second.licPer * 100
			<< "%\n\tPercentage of Activity Limit: " << shipment.iter->second.limPer * 100 << "%\n" << endl << endl
			<< "Exempt from Licensing on " << dateAsMonthDDYYYY(shipment.licExempt) << endl << "Decay Date (< 500dpm): "
			<< dateAsMonthDDYYYY(shipment.decayRelease) << endl;
		num++;
	}
	out << "**********************************\n";
	return out;
}
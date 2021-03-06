/**
	@author Marc Pierre Marc_Pierre@student.uml.edu
	@version 1.0 10/01/18
*/
#ifndef SHIPMENT_H
#define SHIPMENT_H

#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <map>
#include <string>

using std::string;
using std::map;
using std::cout;
using std::cin;
using std::endl;
using boost::gregorian::from_simple_string;
using boost::gregorian::date;
using boost::gregorian::date_duration;

/**
	converts Boost::gregorian::date to string Weekday Month DD, YYYY

	@param date to be converted
			ex: boost::gregorian::date d(2018, Dec, 25)
				cout << dateWithWeekday(d); --> Tuesday December 25, 2018
*/
string dateWithWeekday(const boost::gregorian::date& date);
/**
	converts Boost::gregorian::date to string Month DD, YYYY

	@param date to be converted
			ex: boost::gregorian::date d(2018, Dec, 25)
				cout << dateWithWeekday(d); --> December 25, 2018
*/
string dateAsMonthDDYYYY(const boost::gregorian::date& date);

class SHIPMENT {
 public:
	SHIPMENT();

	/**
		Adds one isotope to shipment

		@param OPTIONAL: numbers isotope
				ex: without param = cout << "Enter name for isotpe: ";
				    with param = cout << "Enter name for isotope " << int << " : ";
	*/
	void addIso(int = -1);
	/**
		removes one isotpe from shipment

		@param name the name of the isotope to be removed
	*/
	void removeIso(string name) { isos.erase(name);}
	/**
		prints the data of one isotope in shipment

		@param name the name of the isotope whose data
		you want to print
	*/
	void printData(string name);
	/**
		accessor function to get initial activity of one isotope in shipment

		@param name the name of the isotope
		@return the initial activity (microCuries) of the isotope
	*/
	double getA0(string name) {return isos[name].A0;}
	/**
		accessor function to get today's activity of one isotope in shipment

		@param name the name of the isotope
		@return today's activity (microCuries) of the isotope
	*/
	double getAToday(string name) { return isos[name].Atoday;}
	/**
		accessor function to get the nature of one isotope in shipment

		@param name the name of the isotope
		@return the nature of the isotope
	*/
	char getNat(string name) { return nature;}
	/**
		accessor function to get the state of one isotope in shipment

		@param name the name of the isotope
		@return the state of the isotope
	*/
	char getState(string name) { return state;}
	/**
		accessor function to get the form of one isotope in shipment

		@param name the name of the isotope
		@return the form of the isotope
	*/
	char getForm(string name) { return form;}
	/**
		accessor function to get the decay per minutre of one isotope in shipment

		@param name the name of the isotope
		@return the decay per minute (microCurie/minute) of the isotope
	*/
	double getDPM(string name) { return isos[name].dpm;}
	/**
		accessor function to get the halflife of one isotope in shipment

		@param name the name of the isotope
		@return the halflife (days) isotope
	*/
	double getHalfLife(string name) { return isos[name].halflife; }
	/**
		accessor function to get the decay constant of one isotope in shipment

		@param name the name of the isotope
		@return the decay constant of the isotope
	*/
	double getDecayConstant(string name) { return isos[name].decayC; }
	/**
		accessor function to get the A1 value of one isotope in shipment

		@param name the name of the isotope
		@return the A1 value (TBq) of the isotope
	*/
	double getA1(string name) { return isos[name].A1; }
	/**
		accessor function to get the A2 value of one isotope in shipment

		@param name the name of the isotope
		@return the A2 value (TBq) of the isotope
	*/
	double getA2(string name) { return isos[name].A2; }
	/**
		accessor function to get the exempt limit of one isotope in shipment

		@param name the name of the isotope
		@return the exempt limit (Bq) of the isotope
	*/
	double getExLimit(string name) { return isos[name].exLim; }
	/**
		accessor function to get exempt concentration of one isotope in shipment

		@param name the name of the isotope
		@return the exempt concentration (Bq/g) of the isotope
	*/
	double getExCon(string name) { return isos[name].exCon; }
	/**
		accessor function to get the licensing limit of one isotope in shipment

		@param name the name of the isotope
		@return the licensing limit (microCurie) of the isotope
	*/
	double getLicLimit(std::string name) { return isos[name].licLim; }
	/**
		accessor function to get the reportable quantity of one isotope in shipment

		@param name the name of the isotope
		@return the reportable quantity (Curie) of the isotope
	*/
	double getReportableQ(string name) { return isos[name].reportQ; }
	/**
		accessor function to get the instruments/articles multipler
		(from 173.425_Table 4) of one isotope in shipment

		@param name the name of the isotope
		@return the instrument/article multiplier of the isotope
	*/
	double getIAMultiplier(string name) { return isos[name].iaMult; }
	/**
		accessor function to get the instruments/articles package limit multipler
		(from 173.425_Table 4) of one isotope in shipment

		@param name the name of the isotope
		@return the instrument/article package limit multiplier of the isotope
	*/
	double getIAPackageMultiplier(string name) { return isos[name].iaPLim; }
	/**
		accessor function to get the limited quantity multiplier
		(from 173.425_Table 4) of one isotope in shipment

		@param name the name of the isotope
		@return the limited quantity multiplier of the isotope
	*/
	double getLimQuanMultiplier(string name) { return isos[name].limMult; }
	/**
		accessor function to get the concentration percentage
		(based off classification) of one isotope in shipment

		@param name the name of the isotope
		@return the concentration percentage of the isotope
	*/
	double getConPercent(std::string name) { return isos[name].conPer; }
	/**
		accessor function to get the limit percentage
		(based on classification) of one isotope in shipment

		@param name the name of the isotope
		@return the limit percentage of the isotope
	*/
	double getLimPercent(std::string name) { return isos[name].limPer; }
	/**
		accessor function to return if of one isotope in shipment is fissle

		@param name the name of the isotope
		@return true if isotope is fissle
				false if isotope is not
	*/
	bool isFissile(string name) { return isos[name].isFissle; }
	/**
		accessor function to get the classification 
		(Exempt, Excepted, Type A, Type B, LSA-1) of one isotope in shipment

		@param name the name of the isotope
		@return the classification of the isotope
	*/
	string getClass(string name);
	/**
		finds the classification of entire shipment

		@return the classification of the entire shipment
	*/
	string findClass();
	/**
		finds the date that shipment can be classified as Exempt

		@param limit max number of days to check classification
		ex: dateOfExempt(20) will compute the classification of
		the shipment from today to today + 20 days.

		@return date that Shipment can be classified as exempt
				+infinity if shipment classification is not exempt after
				computing classfication for today + limit days
	*/
	date dateOfExempt(int limit);
	/**
		finds the date that shipment can be classified as Excepted

		@param limit max number of days to check classification
		ex: dateOfExcepted(20) will compute the classification of
		the shipment from today to today + 20 days.
		
		@return date that Shipment can be classified as excepted
				-infinty if shipment classification is lower then excepted
				+infinity if shipment classification is not excepted after
				computing classfication for today + limit days
	*/
	date dateOfExcepted(int limit);
	/**
		finds the date that shipment can be classified as Type A

		@param limit max number of days to check classification
		ex: dateOfTypeA(20) will compute the classification of
		the shipment from today to today + 20 days.
		
		@return date that Shipment can be classified as Type A
				-infinty if shipment classification is lower then Type A
				+infinity if shipment classification is not Type A after
				computing classfication for today + limit days
	*/
	date dateOfTypeA(int limit);
	/**
		finds the date that shipment can be classified as Type B

		@param limit max number of days to check classification
		ex: dateOfTypeB(20) will compute the classification of
		the shipment from today to today + 20 days.

		@return date that Shipment can be classified as Type B
				-infinty if shipment classification is lower then Type B
				+infinity if shipment classification is not Type B after
				computing classfication for today + limit days
	*/
	date dateOfTypeB(int limit);
	/**
		finds the date that shipment can be classified as Type B: HRCQ

		@param limit max number of days to check classification
		ex: dateOfHRCQ(20) will compute the classification of
		the shipment from today to today + 20 days.
		
		@return date that Shipment can be classified as Type B: HRCQ
				-infinty if shipment classification is lower then Type B: HRCQ
				+infinity if shipment classification is not Type B after
				computing classfication for today + limit days
	*/
	date dateOfHRCQ(int limit);
	/**
		gets the number of days to check if shipment can be 
		classified at a lower level given by the user.

		@return number of days user would like to check the
				classification of the shipment. If user does
				not wish to check or if classification is already
				exempt returns -1
				ex: days2chk = 20 (user wants to check 20 days from today)
	*/
	int getDaysToCheck() { return days2chk; }
	/**
		gets the classification of the shipment

		@return the classification of the shipment as an int
				0 = Exempt
				1 = Excepted
				2 = Type A
				4 = Type B
				8 = Type B: Highway Route Control
	*/
	int getShipmentClass() { return sClass; }
	friend std::ostream& operator<<(std::ostream &out, SHIPMENT &shipment);

 private:
	 struct isotope_data {  // struct to hold all pertinent information about a single isotope
		 double A0;  // Initial Activity (microCi) of isotope  
		 double Atoday;  // Today's activity (microCi) of isotope   
		 double CToday;  // Today's concentration (microCi/gram or microCi/liter) of isotope   
		 double dpm;  //  decays per minute of isotope  
		 double halflife;  //  Halflife (days) of isotope (from Half_Life(days).txt)  
		 double A1;  // A1 (TBq) of isotope (from A1(Tbq).txt)   
		 double A2;  // A2 (Tbq) of isotope (from A2(TBq).txt)   
		 double aCon;  // Activity concentration (Bq) of isotope (Activty of isotope / sum of entire package concentration)	 
		 double aFrac;  // Activity Fraction (Bq) of isotope (Activity fraction of isotope / sum of entire package activity)   
		 double exLim;  // Exempt Limit (Bq) of isotope (from Exempt_limit(Bq).txt)   
		 double exCon;  // Exempt Concentration (Bq/gram) of isotope (from Exempt_concentration(Bq-g).txt)  
		 double licLim;  // Licensing Limit (microCi) of isotope (fromm Licensing_limit(microCi).txt)  
		 double iaMult;  // instruments/articles multipler from 173.425_Table 4 (from IA_Limited_limit.txt)
		 double iaPLim;  // instruments/articles package limit multipler from 173.425_Table 4 (from IA_Package_limit.txt)
		 double limMult;  // limited quantities limit multipler from 173.425_Table 4  
		 double reportQ;  // reportable quantity (Ci) of isotope (from Reportable_Quantity(Ci).txt)
		 double RQFrac;  // fraction of reportable quantity (Activity Today (microCi) / Reportable Quantity (microCuries))
		 double decayC;  // decay constant (1 / halflife(days)) of isotope (from Decay_Constant.txt)  
		 double limPer;  // Activity percentage of isotope limit (activity of isotope / activity limit of isotope)  
		 double conPer;  // Activity concentration of isotope (activity concentration of isotope / activity concentration limit of isotope)  
		 double licPer;  // Percentage of licensing limit (activity of isotope / license_limit from Licensing_Limit(microCi).txt)  
		 double limLim;  // limited limit (TBq) for isotope. [limited limit multiplier * (A1 for special form or A2 for Normal Form)] 
		 double ALim;  // A1 value (TBq) of isotope if Special form, A2 value (TBq) of isotope if Normal Form  
		 double hrcqLim;  // Highway Route Control Limit (either 1000 * TBq or 3000 * A1/A2 whichever is the lowest number) 
		 bool isFissle = false;  // true if isotope is fissile  
		 bool isRQ = false;  // true if isotope is a reportable quantity  
		 bool isLong = false;  // for isotopes with different half lives, true if longer half life else false
		 int isoClass;  // Classification of isotope as int (0 = Exempt, 1 = Excepted, 2 = Type A, 4 = Type B, 8 = Type B: Highway Route Control) 
	 }; typedef isotope_data Data;

	 // Functions to retrieve external data
	 /**
		function to get the halflife of one isotope in shipment
		from Half_Life.txt

		@param name the name of the isotope
		@return the halflife (days) isotope or -2101 for failure
	*/
	 double HalfLife(string name);
	 /**
		function to get the decay constant of one isotope
		from decay_Constant.txt

		@param name the name of the isotope
		@return the decay constant of the isotope or -2101 for failure
	*/
	 double DecayConstant(string name);
	 /**
		function to get the A1 value of one isotope
		from A1(TBq).txt

		@param name the name of the isotope
		@return the A1 value (TBq) of the isotope or -2101 for failure
	*/
	 double A1(string name);
	 /**
		function to get the A2 value of one isotope
		from A2(TBq).txt

		@param name the name of the isotope
		@return the A2 value (TBq) of the isotope or -2101 for failure
	*/
	 double A2(string name);
	 /**
		function to get the exempt limit of one isotope
		from Exempt_limit(Bq).txt

		@param name the name of the isotope
		@return the exempt limit (Bq) of the isotope or -2101 for failure
	*/
	 double ExLimit(string name);
	 /**
		function to get exempt concentration of one isotope
		from Exempt_concentration(Bq-g).txt

		@param name the name of the isotope
		@return the exempt concentration (Bq/g) of the isotope or -2101 for failure
	*/
	 double ExCon(string name);
	 /**
		function to get the licensing limit of one isotope
		from Licensing_Limit(microCi).txt

		@param name the name of the isotope
		@return the licensing limit (microCurie) of the isotope or -2101 for failure
	*/
	 double LicensingLimit(string name);
	 /**
		function to get the reportable quantity of one isotope
		from Reportable_Quantity(Ci)

		@param name the name of the isotope
		@return the reportable quantity (Curie) of the isotope or -2101 for failure
	*/
	 double ReportableQ(string name);
	 /**
		function to get the instruments/articles multipler
		(from 173.425_Table 4) from IA_Limited_limit.txt

		@param name the name of the isotope
		@return the instrument/article multiplier of the isotope or -2101 for failure
	*/
	 double IAMultiplier(char state, char form);
	 /**
		function to get the instruments/articles package limit multipler
		(from 173.425_Table 4) from IA_Package_limit.txt

		@param name the name of the isotope
		@return the instrument/article package limit multiplier of the isotope or -2101 for failure
	*/
	 double IAPackageMultiplier(char state, char form);
	 /**
		function to get the limited quantity multiplier
		(from 173.425_Table 4) from Limited_limit.txt

		@param name the name of the isotope
		@return the limited quantity multiplier of the isotope or -2101 for failure
	*/
	 double LimQuanMultiplier(char state, char form);
	 /**
		function to get the tritiated limited quantity multiplier
		(from 173.425_Table 4) using today's Activity

		@param Today's activity for the isotope (microCi)
		@return the tritiated limited quantity multiplier of the isotope
				if failure or error returns -2101
	*/
	 double TritiatedLimQuanLimit(double AToday);
	 /**
		function to figure of if user inputed name of isotope
		is a valid isotope using ValidIsotopes.txt

		@param name the name of the isotope
		@return true if name found in ValidIsotopes.txt else false
	*/
	 bool validIso(string name);

	 // Functions to determine class of isotope
	 /**
		function to figure out if isotope is Exempt

		@param name the name of the isotope
		@return true if isotope name can be classified as Exempt
				else false
	*/
	 bool exemptClass(string name);
	 /**
		function to figure out if isotope is Excepted

		@param name the name of the isotope
		@return true if isotope name can be classified as Excepted
				else false
	*/
	 bool limitedClass(string name);
	 /**
		function to figure out if isotope is Type A

		@param name the name of the isotope
		@return true if isotope name can be classified as Type A
				else false
	*/
	 bool typeAClass(string name);
	 /**
		function to figure out if isotope is Type B

		@param name the name of the isotope
		@return true if isotope name can be classified as Type B
				else false
	*/
	 bool typeBClass(string name);
	 /**
		function to figure out if isotope is Type B: Highway Route Control

		@param name the name of the isotope
		@return true if isotope name can be classified as Type B: Highway Route Control
				else false
	*/
	 bool HRCQClass(string name);

	 map<string, Data> isos;  // container to hold isotopes in shipment key = isotope name, value = data for isotope from Data struct
	 map<string, Data>::iterator iter;  // iterator for map 
	 bool isPackageRC = false;  // true if package is a reportable quantity
	 int sClass;  // Classification of Shipment as int (0 = Exempt, 1 = Excepted, 2 = Type A, 4 = Type B, 8 = Type B: Highway Route Control)
	 int days2chk;  // How many days to recheck classification of shipment to see if it can be classified at a lower classification
	 char nature;  // Nature (Regular, Instrument, Article) of the shipment
	 char state;  // State (Solid, Liqud, Gas) of the shipment
	 char form;  // Form (Special, Normal) of the shipment
	 double mass;  // Mass (grams, Liters) of the shipment
	 double contactDose;  // Contact dose reading (MEASUREMENT) of shipment
	 double meterDose;  // Meter Dose reading (MEASUREMENT) of shipment
	 string absorption;  // Lung Absorption (Fast, Medium, Slow) for Gasses and certain isotopes
	 string sClassOut;  // Text description of Shipment's class
	 date date;  // Date Gamma Spec'd
	 boost::gregorian::date today;  // Today's Date
	 boost::gregorian::date newT;  // New Today (used when calculating shipment classification in future)
	 boost::gregorian::date licExempt;  // Date an individual isotope is exempt from licensing
	 boost::gregorian::date slicExempt;  // Date shipemnt is exempt from licensing
	 boost::gregorian::date decayRelease;  // Date decays per minute < 500
	 date_duration decayTime;  // How long since date gamma spec'd (Today's date - Date Gamma Spec'd)
};
#endif //SHIPMENT_H
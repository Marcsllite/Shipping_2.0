/**
	@author Marc Pierre Marc_Pierre@student.uml.edu
	@version 1.0 10/01/18
*/

#include "shipment.h"
#include <iostream>
#include <cstdio>

using std::cout;
using std::cin;
using std::endl;

int main() {
	SHIPMENT s1;
	cout << "\nOpen \"ShipmentDetails.txt\" (in Shipping 2.0.exe containing folder) for shipment details\n\n";
	std::ofstream out("ShipmentDetails.txt");
	auto coutbuf = cout.rdbuf(out.rdbuf());  //save and redirect *See https://stackoverflow.com/questions/10150468/how-to-redirect-cin-and-cout-to-files*
	cout << "##################################### SHIPMENT " << " #####################################\n";
	cout << s1 << endl;

	int days2chk = s1.getDaysToCheck();
	if(days2chk > 0) {
		int sClass = s1.getShipmentClass();
		switch (sClass) {
		case 8:
			cout << "Date Shipment can be packaged as Type B: " << dateWithWeekday(s1.dateOfTypeB(days2chk)) << endl;
		case 4:
			cout << "Date Shipment can be packaged as Type A: " << dateWithWeekday(s1.dateOfTypeA(days2chk)) << endl;
		case 2:
			cout << "Date Shipment can be packaged as Excepted: " << dateWithWeekday(s1.dateOfExcepted(days2chk)) << endl;
		case 1:
			cout << "Date Shipment can be packaged as Exempt: " << dateWithWeekday(s1.dateOfExempt(days2chk)) << endl;
			break;
		case 0:
			cout << "Shipment can already be classified as Exempt today.\n";
			break;
		default:
			cout << "Main: Error with getShipmentClass\n";
			break;
		}
	}
	cout << "########################################################################################\n\n\n";

	std::system("pause");
	return 0;
}
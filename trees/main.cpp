// Trees lab exercise
// Adam Sampson <a.sampson@abertay.ac.uk>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

#include "OrgChart.h"

// Import things we need from the standard library
using std::cout;
using std::endl;

void test_orgchart() {
	// The CEO.
	OrgChart chart("Ada");
	Person *ada = chart.root();

	// The section heads.
	Person *ben = ada->add_child("Ben");
	Person *cas = ada->add_child("Cas");

	// The workers on the factory floor.
	Person *dan = ben->add_child("Dan");
	Person *ell = ben->add_child("Ell");
	Person *fay = ben->add_child("Fay");
	Person *gus = cas->add_child("Gus");
	Person *hal = cas->add_child("Hal");
	Person *ivy = cas->add_child("Ivy");

	// The apprentices.
	Person *jeb = ell->add_child("Jeb");
	Person *kim = gus->add_child("Kim");

	cout << "Employees of AdaCorp:\n";
	chart.print();
	cout << "\n";

	cout << "Testing level()\n";
	assert(ada->level() == 1);
	assert(ben->level() == 2);
	assert(gus->level() == 3);
	assert(kim->level() == 4);

	cout << "Testing contains()\n";
	assert(chart.contains("Ada"));
	assert(chart.contains("Cas"));
	assert(chart.contains("Hal"));
	assert(chart.contains("Jeb"));
	assert(!chart.contains("Adam"));
	assert(!chart.contains("Ruth"));

	cout << "Testing size()\n";
	assert(chart.size() == 11);
	Person *lee = cas->add_child("Lee");
	assert(chart.size() == 12);

	cout << "Testing height()\n";
	assert(chart.height() == 4);
	Person *mia = kim->add_child("Mia");
	assert(chart.height() == 5);
}

int main(int argc, char *argv[]) {
	test_orgchart();

	cout << "All OK!\n";
	
	return 0;
}
///////////////////////////////////////////////////////////////
// 
// TheSortingQuiz
// This quiz will ask the user to identify with one of four 
// adjectives. It will then sort the user into a Hogwarts house
// based on the user's answer. If the user does not enter one of 
// the four given traits, it will tell them to try a different 
// wizarding school.
//
// ToDos: 
//   Test for case in user response. IE "brave" should equal "Brave" 
//   Less hard coding for responses. Hash answers/houses?
//   
// Author: Lori Crotser
// Date: 12/27/2017
///////////////////////////////////////////////////////////////


#include <iostream>
using namespace std;


int main() {

	//Init some variables
	string adj;     
	string gryffindorTrait ="Brave";
	string ravenclawTrait ="Smart";
	string slytherinTrait ="Driven";
	string hufflepuffTrait ="Friendly";
	string welcome = "Welcome to Hogwarts School of Witchcraft and Wizardry.";

	//Ask the question 
	cout << "Given the following four adjectives, how would you describe yourself? Brave? Smart? Driven? Friendly?"  << endl;

	//Get user input
	getline(cin , adj);

	//Strip any spaces
	for (int i = 0; i < adj.size(); i++) 
	{
		size_t found=adj.find(' ');
		if (found < adj.size()) {
			adj.replace(found,1,"");
		}
	}

	//Compare strings to see if output matches traits listed in question. 
	//If we find a match, let the user know they've been sorted! 
	if (adj.compare(gryffindorTrait) == 0) {
		cout << "Congratulations! You have been sorted into Gryffindor House. " << welcome << endl;
	}
	else if (adj.compare(ravenclawTrait) == 0) {
		cout << "Congratulations! You have been sorted into Ravenclaw House. " << welcome << endl;
	}
	else if (adj.compare(slytherinTrait) == 0) {
		cout << "Congratulations! You have been sorted into Slytherin House. " << welcome << endl;
	}
	else if (adj.compare(hufflepuffTrait) == 0) {
		cout << "Congratulations! You have been sorted into Hufflepuff House. " << welcome << endl;
	}

	else {
	   cout << "Hmm. I am having trouble placing you here at Hogwarts School of Witchcraft and Wizardry. Perhaps you should try Beauxbatons or Durmstrang?"  << endl;
	return 0;
	}

}


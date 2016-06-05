/*
*
* Natural Language Processing Assignment
* Removing Stop Words from a given Data Set
*
*/

#include <bits/stdc++.h>

using namespace std;

int main() {
	
	ifstream dataset;
	
	cout << "Enter Name of the File: " << endl;
	string name; cin >> name;
	
	dataset.open((name + ".txt"));
			
	
	if(dataset.is_open()) {
		string line;
		
		int i = 0;
		
		ofstream file;
		file.open("negative1.txt");
		
		while(getline(dataset, line)) {
			
			file << "- "
			file << line;
			
			i++;
			
			if((i/50) != ((i-1)/50)) {
				int idx = i/50;
				
				file.close();
				string n = "negative" + to_string((idx+1)) + ".txt";
				file.open(n);
			}
			else
				file << "\n";
		}
		
		file.close();
		dataset.close();
		
		cout << "PROGRAM SUCCESS." << endl;
	}
	else {
		cout << "Error Opening Given File." << endl;
	}
		
	
	
	return 0;
}

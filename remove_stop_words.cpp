/*
*
* Natural Language Processing Assignment
* Removing Stop Words from a given Data Set
*
*/

#include <bits/stdc++.h>

using namespace std;

vector<string> swords;


string remove_stop_words_line(string line) {
	
	string newline;
	int l = line.length();
	
	for(int i = 0; i < l; i++)
		if(line[i] >= 65 && line[i] <= 90)
			newline.push_back(tolower(line[i]));
		else if(line[i] >= 97 && line[i] <= 122)
			newline.push_back(line[i]);
		else if(line[i] == 32)
			newline.push_back(line[i]);
	
	// Now Removing Stop Words
	string final_line;
	
	istringstream wordstream(newline);
    string word;
	
    while(wordstream >> word) {

		bool flag = false;
		for(string s : swords)
			if(word.compare(s) == 0) {
				flag = true;
				break;
			}
		
		if(!flag) {
			final_line.append(word);
			final_line.push_back(' ');
		}
    }
	
	return final_line;
}

void remove_stop_words(string name, string oname) {
	
	ifstream stopwords;
	stopwords.open("stopwords.txt");
	
	if(stopwords.is_open()) {
		string line;
		
		while(getline(stopwords, line))
			swords.push_back(line);
		
		stopwords.close();		
	}
	else {
		cout << "Error Opening Stop Words File." << endl;
		return;
	}
	
	// All Stop Words stored in vector swords
	// Opening Data Set to Remove Stop words from iter_swap
		
	ifstream data;
	ofstream newdata;
	data.open(name);
	newdata.open(oname);
	
	if(data.is_open()) {
		string line;
		
		while(getline(data, line)) {
			
			string add_file_string = remove_stop_words_line(line);
			newdate << "- "
			newdata << add_file_string;
			newdata << "\n";
		}
		
		data.close();
		newdata.close();
		cout << "Program SUCCESS" << endl;
	}
	else {
		cout << "Error Opening Data Set File." << endl;
		return;
	}
	
}


int main() {
	
	cout << "Enter the Name of the Input dataset (also add .txt)" << endl;
	string name, oname; cin >> name;
	cout << "Enter the Name of the Output Dataset (also add .txt)" << endl;
	cin >> oname;
	remove_stop_words(name, oname);
	
	return 0;
}

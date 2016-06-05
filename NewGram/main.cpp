/*
*
* Natural Language Processing Assignment
* Removing Stop Words from a given Data Set
*
*/

#include <bits/stdc++.h>

using namespace std;

int find_vocab(int idx) {
	
	// dont include idx file
	
	map<string, int> mp, bmp;
	
	// PreProcessing
	
	for(int i = 1; i <= 10; i++) {
		
		string line;
		
		if(i == idx) continue;
		
		ifstream file;
		string n = "pos_neg" + to_string(i) + ".txt";
		file.open(n);
		
		while(getline(file, line)) {
			
			string prev = ".";
			istringstream wordstream(line);
			string word;
			while(wordstream >> word) {
				
				if(word == "-" || word == "+") continue;
				
				mp[word]++;
				string latest = prev + " " + word;
				prev = word;
				bmp[latest]++;
			}
		}
		file.close();
	}
	
	auto it = mp.begin();
	
	ofstream voc;
	voc.open("vocab.txt");
	
	int sz = 0;
	
	while(it != mp.end()) {
		if(it->second > 1) {
			voc << it->first;
			voc << "\n";
			sz++;
			
			++it;
		}
		else
			++it;
	}
	
	it = bmp.begin();
	while(it != bmp.end()) {
		if(it->second > 2) {
			voc << it->first;
			
			++it;
			if(it != bmp.end())
				voc << "\n";
		}
		else
			++it;
	}
	voc.close();
	
	return sz;
	// Vocab BUILT
}

pair<int, int> find_freq(int idx, map<string, int>& mp, map<string, int>& mp2) {
	
	pair<int, int> sz = make_pair(0, 0);
	
	for(int i = 1; i <= 10; i++) {
		
		string line;
		
		if(i == idx) continue;
		
		ifstream file;
		string n = "pos_neg" + to_string(i) + ".txt";
		file.open(n);
		
		while(getline(file, line)) {
			
			string prev = ".";
			istringstream wordstream(line);
			string word;
			wordstream >> word;
			if(word == "+")
				while(wordstream >> word) {
					mp[word]++;
					string latest = prev + " " + word;
					prev = word;
					mp[latest]++;
					sz.first++;
				}
			else
				while(wordstream >> word) {
					mp2[word]++;
					string latest = prev + " " + word;
					prev = word;
					mp2[latest]++;
					sz.second++;
				}			
		}
		file.close();
	}

	return sz;
}

// MAIN FUNCTION

double solve(int idx) {
	
	string line;
	// Test Set is file idx
	// Training Set is remaining files
	
	// Finding Vocabulary
	int vocab_size = find_vocab(idx);
	
	// FINDING Freq
	map<string, int> pos_freq;
	map<string, int> neg_freq;
	
	pair<int, int> sz = find_freq(idx, pos_freq, neg_freq);
	
	
	int size_positive = sz.first;
	int size_negative = sz.second;
			
	// Generating Model
	
	ifstream voc;
	voc.open("vocab.txt");
	
	map<string, double> pos_prob;
	map<string, double> neg_prob;
	
	while(getline(voc, line)) {
		
		// positive class
		double prob = ((double)(pos_freq[line] + 1))/((double)(vocab_size + size_positive));
		prob = log10(prob);
		pos_prob[line] = prob;
		
		// negative class
		prob = ((double)(neg_freq[line] + 1))/((double)(vocab_size + size_negative));
		prob = log10(prob);
		neg_prob[line] = prob;
	}

	// Model Generated
	
	// FINDING ACCURACY
	
	
	// TEST SET
	
	ifstream test_set;
	string n = "pos_neg" + to_string(idx) + ".txt";
	test_set.open(n);
	
	int correct = 0, wrong = 0;
	
	while(getline(test_set, line)) {
		
		// this is a document
		
		istringstream wordstream(line);
		string word;
		
		double res1 = 0, res2 = 0;
		string prev = ".";
		string curr;
		
		while(wordstream >> word) {
			
			if(word == "+" || word == "-") {
				curr = word;
				continue;
			}
			
			string latest = prev + " " + word;
			
			if(pos_prob[latest] == 0)
				res1 += pos_prob[word] != 0 ? pos_prob[word] : log10(((double)1/(double)(vocab_size + size_positive)));
			else
				res1 += pos_prob[latest];
				
			if(neg_prob[latest] == 0)
				res2 += neg_prob[word] != 0 ? neg_prob[word] : log10(((double)1/(double)(vocab_size + size_negative)));
			else
				res2 += neg_prob[latest];

			prev = word;
		}
		if(res1 >= res2) {
			if(curr == "+") correct++; else wrong++;
		}
		else {
			
			if(curr == "-") correct++; else wrong++;
		}
	}
	test_set.close();
	
	double accuracy = (((double)(correct))/((double)(wrong + correct)));
	
	cout << accuracy << endl;
	
	return accuracy;
}

int main() {
	
	vector<double> all_accuracy;
	
	for(int i = 1; i <= 10; i++) {
		double acc = solve(i);
		all_accuracy.push_back(acc);
	}
	
	// FINDING AVERAGE OF ALL ACCURACIES
	
	double avg = 0;
	
	for(int i = 0; i < all_accuracy.size(); i++)
		avg += all_accuracy[i];
	
	avg = avg/((double)all_accuracy.size());
	
	cout << "Average of All Accuracies: " << avg << endl;
	
	return 0;
}

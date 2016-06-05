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
		string n = "positive" + to_string(i) + ".txt";
		file.open(n);
		
		while(getline(file, line)) {
			
			string prev = ".";
			istringstream wordstream(line);
			string word;
			while(wordstream >> word) {
				mp[word]++;
				string latest = prev + " " + word;
				prev = word;
				bmp[latest]++;
			}
		}
		file.close();
		
		n = "negative" + to_string(i) + ".txt";
		file.open(n);

		while(getline(file, line)) {
			
			string prev = ".";
			istringstream wordstream(line);
			string word;
			while(wordstream >> word) {
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
		if(it->second > 1) {
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

int find_freq1(int idx, map<string, int>& mp) {
	
	int sz = 0;
	
	for(int i = 1; i <= 10; i++) {
		
		string line;
		
		if(i == idx) continue;
		
		ifstream file;
		string n = "positive" + to_string(i) + ".txt";
		file.open(n);
		
		while(getline(file, line)) {
			
			string prev = ".";
			istringstream wordstream(line);
			string word;
			while(wordstream >> word) {
				mp[word]++;
				string latest = prev + " " + word;
				prev = word;
				mp[latest]++;
				sz++;
			}
		}
		file.close();
	}

	return sz;
}

int find_freq2(int idx, map<string, int>& mp) {
	
	int sz = 0;
	
	for(int i = 1; i <= 10; i++) {
		
		string line;
		
		if(i == idx) continue;
		
		ifstream file;
		string n = "negative" + to_string(i) + ".txt";
		file.open(n);
		
		while(getline(file, line)) {
			
			string prev = ".";
			istringstream wordstream(line);
			string word;
			while(wordstream >> word) {
				mp[word]++;
				string latest = prev + " " + word;
				prev = word;
				mp[latest]++;				
				sz++;
			}
		}
		file.close();
	}

	return sz;
}

double solve(int idx) {
	
	string line;
	// Test Set is file idx
	// Training Set is remaining files
	
	// Finding Vocabulary
	int vocab_size = find_vocab(idx);
	
	
	// FINDING Freq
	map<string, int> pos_freq;
	map<string, int> neg_freq;
	
	int size_positive = find_freq1(idx, pos_freq);
	int size_negative = find_freq2(idx, neg_freq);
		
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
	
	
	// POSITIVE TEST SET
	
	ifstream test_set;
	string n = "positive" + to_string(idx) + ".txt";
	test_set.open(n);
	
	vector< pair<double, double> > test_set_prob; int l = 0;
	
	while(getline(test_set, line)) {
		
		// this is a document
		
		istringstream wordstream(line);
		string word;
		
		double res = 1;
		string prev = ".";
		
		while(wordstream >> word) {
			
			string latest = prev + " " + word;
			
			if(pos_prob[latest] == 0) {
				
				if(pos_prob[word] == 0)
					pos_prob[word] = log10(((double)1/(double)(vocab_size +  + size_positive)));
				
				res *= pos_prob[word];
			}
			else
				res *= pos_prob[latest];
			prev = word;
		}
		
		test_set_prob.push_back(make_pair(res, 0));
	}
	test_set.close();
	
	l = 0;
	n = "positive" + to_string(idx) + ".txt";
	test_set.open(n);
	
	while(getline(test_set, line)) {
		
		// this is a document
		
		istringstream wordstream(line);
		string word;
		
		double res = 1;
		string prev = ".";
		
		while(wordstream >> word) {
			
			string latest = prev + " " + word;
			if(neg_prob[latest] == 0) {
				if(neg_prob[word] == 0)
					neg_prob[word] = log10(((double)1/(double)(vocab_size +  + size_negative)));					
				res *= neg_prob[word];
			}
			else
				res *= neg_prob[latest];
			prev = word;
		}
		
		test_set_prob[l++].second = res;
	}
	test_set.close();

	
	n = "positive" + to_string(idx) + ".txt";
	test_set.open(n);
	
	int A[2][2];
	A[0][0] = A[0][1] = A[1][0] = A[1][1] = 0;
	
	int j = 0;
	while(getline(test_set, line)) {
		
		if(test_set_prob[j].first > test_set_prob[j].second)
			A[0][0]++;
		else
			A[1][0]++;
		j++;
		
		
	}
	test_set.close();
	
	// NEGATIVE TEST SET
	
	n = "negative" + to_string(idx) + ".txt";
	test_set.open(n);
	vector< pair<double, double> > test_set_prob_neg; l = 0;

	while(getline(test_set, line)) {
		
		// this is a document
		
		istringstream wordstream(line);
		string word;
		
		double res = 1;
		string prev = ".";
		
		while(wordstream >> word) {
			
			string latest = prev + " " + word;
			if(pos_prob[latest] == 0) {
				if(pos_prob[word] == 0)
					pos_prob[word] = log10(((double)1/(double)(vocab_size +  + size_positive)));				
				res *= pos_prob[word];
			}
			else
				res *= pos_prob[latest];
			
			prev = word;
		}
		
		test_set_prob_neg.push_back(make_pair(res, 0));
	}
	test_set.close();
	
	l = 0;
	n = "negative" + to_string(idx) + ".txt";
	test_set.open(n);
	
	while(getline(test_set, line)) {
		
		// this is a document
		
		istringstream wordstream(line);
		string word;
		
		double res = 1;
		string prev = ".";
		
		while(wordstream >> word) {
			
			string latest = prev + " " + word;
			if(neg_prob[latest] == 0) {
				if(neg_prob[word] == 0)
					neg_prob[word] = log10(((double)1/(double)(vocab_size +  + size_negative)));					
				res *= neg_prob[word];
			}
			else
				res *= neg_prob[latest];
			prev = word;
		}
		
		test_set_prob_neg[l++].second = res;
	}
	test_set.close();
	
	// Accuracy Building
	
	n = "negative" + to_string(idx) + ".txt";
	test_set.open(n);
	
	j = 0;
	while(getline(test_set, line)) {
		
		if(test_set_prob_neg[j].first < test_set_prob_neg[j].second)
			A[1][1]++;
		else
			A[0][1]++;
		j++;
	}
	test_set.close();
	
	double accuracy = ((double)(A[0][0] + A[1][1]))/((double)(A[0][0] + A[1][1] + A[1][0] + A[0][1]));
	
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

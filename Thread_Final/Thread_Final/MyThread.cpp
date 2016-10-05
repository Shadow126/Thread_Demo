#include "MyThread.h"
#include<fstream>
#include<iostream>
#include<string>
using namespace std;
MyThread::MyThread()
{
}
void MyThread::wirte_file(string path) {
	ofstream myfile(path);
	string str;
	do {
		cin >> str;
		if (str.compare("exit") == 0) {
			break;
		}
		myfile << str;
		//cout << str << endl;
	} while (str.compare("exit") != 0);
	myfile.close();
}


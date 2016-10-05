#include<iostream>
#include<string>
#include<string.h>
#include "MyThread.h"
#include<thread>
#include<fstream>
#include<Windows.h>
#include<vector>
#include<list>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
list<string> list_file;

void Write(string path, string s) {
	ofstream f;
	f.open(path, ios::out | ios::app);
	f << s << endl;
	f.close();
}

DWORD WINAPI GetLastWriteTime(HANDLE hFile, LPTSTR lpszString, DWORD dwSize)
{
	
	TCHAR szBuf[MAX_PATH];
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;
	DWORD dwRet;

	// Retrieve the file times for the file.
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return FALSE;

	// Convert the last-write time to local time.
	FileTimeToSystemTime(&ftWrite, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	// Build a string showing the date and time.
	dwRet = StringCchPrintf(lpszString, dwSize,
		TEXT("%02d/%02d/%d  %02d:%02d"),
		stLocal.wMonth, stLocal.wDay, stLocal.wYear,
		stLocal.wHour, stLocal.wMinute);

	if (GetLastWriteTime(hFile, szBuf, MAX_PATH))
		_tprintf(TEXT("Last write time is: %s\n"), szBuf);
}

DWORD WINAPI check_file(LPVOID a) {
	WIN32_FIND_DATA info;
	string path = "D:\\1LINK\\Week3\\Thread_Final\\content\\";
	string searchPatter = "*.txt";
	string fullFile = path + searchPatter;

	HANDLE file = FindFirstFile(fullFile.c_str(), &info);
	if (file != INVALID_HANDLE_VALUE) {
		struct FileInfo {
			HANDLE h;
			WIN32_FIND_DATA info;
		} newest;
		newest.h = file;
		newest.info = info;
		while (FindNextFile(file, &info)) {
			if (CompareFileTime(&info.ftLastWriteTime, &newest.info.ftLastWriteTime) > 0) {
				newest.h = file;
				newest.info = info;
			}
		}
		
		std::cout << newest.info.cFileName << '\n';
		
		ifstream in;
		string filepath = path + info.cFileName;
		in.open(filepath.c_str(), ios::in);
		string line;
		
		while (getline(in, line)){
			cout << line << endl;
			Write("D:\\1LINK\\Week3\\Thread_Final\\Thread_Final\\note.txt", line);
		}
		in.close();
		FindClose(file);
		//file2.close();
	}
	return 1;
}

DWORD WINAPI Read(LPVOID arg){
	//std::thread t1(funcion_1);//t1 bat dau chay

	//MyThread thread;
	///thread.wirte_file("note.txt");
	//ofstream myfile("note.txt");
	//vector<string> v;


	string path = "D:\\1LINK\\Week3\\Thread_Final\\content\\";
	string searchPatter = "*.txt";
	string fullFile = path + searchPatter;

	WIN32_FIND_DATA FindData;
	HANDLE hFind;

	
	hFind = FindFirstFile(fullFile.c_str(), &FindData);
	if (hFind == INVALID_HANDLE_VALUE) {
		cout << "Khong tim thay" << endl;
		return -1;
	}

	cout << "Doc duong dan toi file .txt: " << endl;
	string line;
	vector<string> st;
	do {
		string filePath = path + FindData.cFileName;

		st.push_back(filePath.c_str());
		cout << filePath << endl;//in ra duong dan toi file .txt oke
	} while (FindNextFile(hFind, &FindData) > 0);
	for (int i = 0; i < st.size(); i++) {
		ifstream in;
		in.open(st[i].c_str(), ios::in);

		while (getline(in, line))
		{
			cout << line << endl;
			Write("D:\\1LINK\\Week3\\Thread_Final\\Thread_Final\\note.txt", line);
		}
		st.clear();
		in.close();
	}
	/*for (int i = 0; i < st.size(); i++) {
		string line;
		ifstream file(st[i]);
		while (getline(file, line)){
			cout << line << endl;
			Write("D:\\1LINK\\Week3\\Thread_Final\\Thread_Final\\note.txt", line);
		}
		file.close();
		st[i] = -1;
	}*/
	return 0;
}
int main() {
	HANDLE handel;
	HANDLE hFile,hcheck;
	TCHAR szBuf[MAX_PATH];
	//Read();
	while (true) {
		handel = CreateThread(0, NULL, Read, 0, NULL, 0);

		hcheck = CreateThread(0, NULL, check_file, 0, NULL, 0);
		Sleep(15000);
	}
	
	CloseHandle(handel);
}

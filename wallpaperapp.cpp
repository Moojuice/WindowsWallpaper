// wallpaperapp.cpp : Program which allows the user to have more customization on their wallpaper shuffling. Can sort pictures into folders which 
// can be reached based on the localtime. 
//

#include "stdafx.h"
#include "wallpaperapp.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector> 
#include <atlconv.h>    
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>
#include <time.h>
#include <thread> 

#include <comutil.h>
#include <stdio.h>

using namespace std;


//Sample Fields, put your actual ones in the settings.txt
//It is NECESSARY that the folder path uses double \'s, and ends with \\ (you have to replace/add them yourself)
//You can retrieve the folder path by clicking at the top of the url with the arrows
//Example: "C:\\Users\\Public\\Pictures\\Sample Pictures\\"
static string folderFilePath = "C:\\Users\\Public\\Pictures\\Sample Pictures\\";

static double timeToShuffle = 1; //this is in hours

static bool randomized = true;

static bool dayNight = false;

static bool seasons = false;

static string dayFolderPath = "C:\\Users\\Public\\Pictures\\Sample Pictures\\Day\\";

static string nightFolderPath = "C:\\Users\\Public\\Pictures\\Sample Pictures\\Night\\";

/*
Self-explanatory
*/
double hoursToSec(double input) {
	return input * 3600;
}

/*
Mostly taken from references online with slight modifications. Takes in a string for the URL, and then adds all of the file names to the folder URL 
to the new vector 
*/
vector<string> retrieveFilePaths(string folder)
{
	vector<string> result;
	char search_path[200];
	sprintf_s(search_path, "%s*.*", folder.c_str());
	WIN32_FIND_DATAA fd; 
	HANDLE hFind = ::FindFirstFileA(search_path, &fd); 
	if(hFind != INVALID_HANDLE_VALUE) 
	{ 
		do 
		{ 
			// read all (real) files in current folder, delete '!' read other 2 default folders
			if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) 
			{
				result.push_back(folder + fd.cFileName);
			}
		}while(::FindNextFileA(hFind, &fd)); 
		::FindClose(hFind); 
	} 
	return result;
}

/*
Reads the user's settings based on a very strict format using string streams and a file stream
*/
void readSettings() {
	ifstream ifs;
	ifs.open("settings.txt");
	if (ifs.is_open()) {
		string line, placeholder;
		getline(ifs, line); 
		istringstream iss(line);

		folderFilePath.clear();
		iss >> placeholder; 
		while (iss >> placeholder) {
			folderFilePath += placeholder + " "; //Allows for spacing in folders 
		}
		folderFilePath.pop_back();

		getline(ifs, line); 
		istringstream iss2(line);
		iss2 >> placeholder >> timeToShuffle;
		//cout << "time to shuffle is: " << timeToShuffle << endl;

		getline(ifs, line); 
		istringstream iss3(line);
		iss3 >> placeholder >> randomized;

		getline(ifs, line); 
		istringstream iss4(line);
		iss4 >> placeholder >> dayNight;

		getline(ifs, line); 
		istringstream iss5(line);
		iss5 >> placeholder >> seasons;
 
		dayFolderPath.clear();
		getline(ifs, line); 
		istringstream iss6(line);
		iss6 >> placeholder;
		while (iss6 >> placeholder) {
			dayFolderPath += placeholder + " ";
		}
		dayFolderPath.pop_back();

		nightFolderPath.clear();
		getline(ifs, line); 
		istringstream iss7(line);
		iss7 >> placeholder;
		while (iss7 >> placeholder) {
			nightFolderPath += placeholder + " ";
		}
		nightFolderPath.pop_back();

		ifs.close();
		
	}
	else {
		throw BAD_READ;
	}
}

/*
Calculates the next picture based on the current index and the # of elements in the folder.
Calculates either linearlly or randomly, based on user preference
*/
void nextIndex(unsigned &currentIndex, unsigned collectionSize) {
	if (randomized) {
		unsigned temp;
		do {
			temp = rand() % collectionSize;
		}
		while (currentIndex == temp);
		currentIndex = temp;
	}
	else {
		if (currentIndex >= collectionSize) {
			currentIndex = 0;
		}
		else {
			++currentIndex; 
		}
	}
	//cout << "Next index found, currently: " << currentIndex << endl;
}

/*
Takes in the URL and picture index uses it to update the wallpaper
*/
void updateWallpaper(unsigned &currentIndex, vector<string> usableFolder) {
	SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (PVOID)usableFolder[currentIndex].c_str(), SPIF_UPDATEINIFILE);
	//cout << "Next wallpaper done" << endl;
}

/*
Executes as a separate thread which shuffles based on the user's time 
*/
void timerFunction(unsigned& currentIndex, vector<string> usableFolder) {
	//cout << "Wallpaper timer has started" << endl;
	time_t start = time(0);
	while (true) {
		if (quit) {
			break;
		}
		time_t end = time(0);
		if (hoursToSec(timeToShuffle) <= end-start) {
			start = time(0);
			nextIndex(currentIndex, usableFolder.size());
			updateWallpaper(currentIndex, usableFolder);
		}
	}
}

/*
Uses the local time to calculate if it is daytime (6am to 6pm) or night 
*/
bool isDay() {
	time_t tt = time(NULL);
	struct tm t;
	localtime_s(&t, &tt);
	return t.tm_hour < 18 && t.tm_hour >=6;
}

/*
DayNight equivalent of the previous timer which also takes into consideration isDay and takes in more parameters 
*/
void timerFunctionDN(unsigned& currentIndex, vector<string> dayFolder, vector<string> nightFolder) {
	//cout << "DNWallpaper timer has started" << endl;
	time_t start = time(0);
	while (true) {
		if (quit) {
			break;
		}
		time_t end = time(0);
		if (hoursToSec(timeToShuffle) <= end-start) {
			start = time(0);
			if (isDay()) {
				nextIndex(currentIndex, dayFolder.size());
				updateWallpaper(currentIndex, dayFolder);
			}
			else {
				nextIndex(currentIndex, nightFolder.size());
				updateWallpaper(currentIndex, nightFolder);
			}
		}
	}
}

/*
Prompts the user to exit at any time 
*/
void promptUser() {
	cout << "Type in \"quit\" when you wish to terminate" << endl;
	while (true) {
		string userInput;
		cin >> userInput;
		if (userInput == "quit") {
			quit = true;
			break;
		}
	}
}

/*
Future add-on function I plan to write which will calculate the color of the picture 
*/
bool dayNightCalc (string filePath) {
	return true;
}


int main(int argc, char* argv[])
{	
	try {
		srand(static_cast<unsigned int>(time(NULL))); //random seed for our rand

		if (argc != 1) {
			throw USAGE;
		}

		readSettings();
		//cout << "Settings successfully read" << endl;

		vector<string> dayFolder;
		vector<string> nightFolder;
		vector<string> completeFolder;
		unsigned currentIndex;

		if (dayNight) {
			dayFolder = retrieveFilePaths(dayFolderPath);
			nightFolder = retrieveFilePaths(nightFolderPath);
			if (dayFolder.size() == 0 || nightFolder.size() == 0) {
				throw EMPTY_FOLDER;
			}

			if (isDay()) {
				currentIndex = dayFolder.size();
				nextIndex(currentIndex, dayFolder.size());
				updateWallpaper(currentIndex, dayFolder);
			}
			else {
				currentIndex = nightFolder.size();
				nextIndex(currentIndex, nightFolder.size());
				updateWallpaper(currentIndex, nightFolder);
			}
			thread shiftWallpaper(timerFunctionDN, currentIndex, dayFolder, nightFolder);
			thread userInput(promptUser);
			userInput.join();
			shiftWallpaper.join();
		}
		else {
			completeFolder = retrieveFilePaths(folderFilePath);
			if (completeFolder.size() == 0) {
				throw EMPTY_FOLDER;
			}

			currentIndex = completeFolder.size();
			nextIndex(currentIndex, completeFolder.size());
			updateWallpaper(currentIndex, completeFolder);

			thread shiftWallpaper(timerFunction, currentIndex, completeFolder);
			thread userInput(promptUser);
			userInput.join();
			shiftWallpaper.join();
		}
		
		cout << "System has exited, current index is: " << currentIndex << " if you would like to continue" << endl;
		return SUCCESS;

	} catch(RETURN_VALUES e) {
		switch(e) {
			case USAGE:
				cout << "Usage: Proper usage contains only 0 arguments, simply wallpaperapp.exe" << endl;
				return USAGE;
			case BAD_READ:
				cout << "Error: Unable to read settings file, please make sure it is located in the same folder as the exe" << endl;
				return e;
			case EMPTY_FOLDER:
				cout << "Error: Either folder is empty, or could not locate files/folder" << endl;
				return EMPTY_FOLDER;
		}
	}
}


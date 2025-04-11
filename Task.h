#define _CRT_SECURE_NO_WARNINGS

#pragma once

#include <fstream>
#include <cstring>
#include <stdexcept>
#include <iostream>

class Task
{
public:
	Task() = delete;
	Task(std::ifstream&);
	Task(const char*, unsigned int, unsigned int, const char*);
	Task(const Task&);
	~Task();

	bool operator==(const Task& other) const;

	const char* const getDescription() const;
	unsigned int getPriority() const;
	bool getStatus() const;
	unsigned int getDuration() const;

	void markAsReady();

	void print(std::ofstream&) const;

private:
	static char* readDescriptionFromFile(std::ifstream&);
	static char* readDescriptionFromObject(const Task&);
	static bool validateStartingTime(const char*);
	static bool validateDuration(unsigned int, const char*);
	static unsigned int getTimeInMinutes(const char*);

private:
	static const int MAX_PRIORITY;

	const char* const description;
	unsigned int priority;
	bool status; // defaults to false
	unsigned int duration; // in minutes
	char startingTime[6];
};


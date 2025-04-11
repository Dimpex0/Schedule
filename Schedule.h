#pragma once

#include <new>

#include "Task.h"

class Schedule
{
public:
	Schedule(const char*);
	Schedule(const Schedule&);
	Schedule& operator=(const Schedule&);
	~Schedule();

	bool addTask(const Task&);

private:
	void deleteTasks(Task**, size_t);
	void clearTasks();
	void copy(const Schedule&);

private:
	char* name;
	Task** tasks;
	size_t tasksCount;
};


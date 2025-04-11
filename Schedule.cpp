#include "Schedule.h"


Schedule::Schedule(const char* name) : 
	name(nullptr),
	tasks(nullptr),
	tasksCount(0)
{
	this->name = strcpy(new char[strlen(name) + 1], name);
}

Schedule::Schedule(const Schedule& other) {
	copy(other);
}

Schedule& Schedule::operator=(const Schedule& other) {
	if (this != &other) {
		copy(other);
	}
	return *this;
}

Schedule::~Schedule() {
	delete[] this->name;
	deleteTasks(this->tasks, this->tasksCount);
}

bool Schedule::addTask(const Task& task) {
	Task** newTasks = new(std::nothrow) Task * [this->tasksCount + 1];
	if (!newTasks) {
		return false;
	}
	Task** tasksForDelete = new(std::nothrow) Task * [this->tasksCount];
	if (!tasksForDelete) {
		delete[] newTasks;
		return false;
	}

	int deleteCount = 0;

	for (size_t i = 0; i < this->tasksCount; i++) {
		Task currentTask = *(this->tasks[i]);
		if (currentTask == task && currentTask.getPriority() < task.getPriority()) {
			tasksForDelete[deleteCount] = this->tasks[i];
			deleteCount++;
		}
		else {
			delete[] newTasks;
			deleteTasks(tasksForDelete, deleteCount);
			return false;
		}
	}

	newTasks[this->tasksCount] = new(std::nothrow) Task(task);
	if (!newTasks[this->tasksCount]) {
		delete[] newTasks;
		deleteTasks(tasksForDelete, deleteCount);
		return false;
	}

	deleteTasks(this->tasks, this->tasksCount);
	this->tasksCount++;
	this->tasks = newTasks;
	clearTasks();
}

void Schedule::deleteTasks(Task** tasks, size_t count) {
	for (size_t i = 0; i < count; i++) {
		delete tasks[i];
	}
	delete[] tasks;
}

void Schedule::clearTasks() {
	size_t tasksCount = 0;

	for (size_t i = 0; i < this->tasksCount; i++) {
		if (this->tasks[i]) {
			tasksCount++;
		}
	}

	// ако хвърли грешка, addTask ще остави nullptr елементи и невалидна бройка
	Task** newTasks = new(std::nothrow) Task * [tasksCount];
	if (!newTasks) {
		return;
	}

	size_t taskIndex = 0;
	for (size_t i = 0; i < this->tasksCount; i++) {
		if (this->tasks[i]) {
			newTasks[taskIndex] = this->tasks[i];
			taskIndex++;
		}
	}
	delete[] this->tasks;
	this->tasks = newTasks;
	this->tasksCount = tasksCount;
}

void Schedule::copy(const Schedule& other) {
	char* newName = strcpy(new char[strlen(other.name) + 1], other.name);

	Task** newTasks = new(std::nothrow) Task * [other.tasksCount];

	if (!newTasks) {
		delete[] newName;
		delete[] newTasks;
		throw std::bad_alloc();
	}

	for (size_t i = 0; i < other.tasksCount; i++) {
		try {
			newTasks[i] = new Task(*other.tasks[i]);
		}
		catch (...) {
			for (size_t j = 0; j < i; j++) {
				delete newTasks[j];
			}
			delete[] newTasks;
			delete[] newName;
			throw;
		}
	}

	delete[] this->name;
	deleteTasks(this->tasks, this->tasksCount);

	this->name = newName;
	this->tasks = newTasks;
	this->tasksCount = other.tasksCount;
}
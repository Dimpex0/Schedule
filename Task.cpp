#include "Task.h"
#include "Schedule.h";

const int Task::MAX_PRIORITY = 5;

Task::Task(const char* description, unsigned int priority, unsigned int duration, const char* startingTime) :
	description([description]() {
	return strcpy(new char[strlen(description) + 1], description);
		}()),
	status(false),
	duration(duration)
{
	if (priority > Task::MAX_PRIORITY || !validateDuration(duration, startingTime)) {
		delete[] this->description;
		throw std::invalid_argument("Invalid priority or starting time");
	}
	this->priority = priority;
	strcpy(this->startingTime, startingTime);
}

Task::Task(std::ifstream& ifs) :
	description(Task::readDescriptionFromFile(ifs))
{
	unsigned int priority;
	bool status;
	unsigned int duration;
	char startingTime[6];

	ifs >> priority >> status >> duration >> startingTime;

	if (ifs) {
		if (priority > Task::MAX_PRIORITY) {
			delete[] this->description;
			throw std::invalid_argument("Invalid priority");
		}
		this->priority = priority;
		this->status = status;
		this->duration = duration;
		strcpy(this->startingTime, startingTime);
		return;
	}

	delete[] description;
	ifs.clear();
	throw std::runtime_error("Couldn't read file");
}

Task::Task(const Task& other) :
	description(Task::readDescriptionFromObject(other)),
	status(false),
	duration(other.duration),
	priority(other.priority)
{}

Task::~Task() {
	delete[] description;
}


// Сравнява дали два таска се застъпват
bool Task::operator==(const Task& other) const {
	unsigned int thisStartingTimeInMinutes = getTimeInMinutes(this->startingTime);
	unsigned int otherStartingTimeInMinutes = getTimeInMinutes(other.startingTime);

	unsigned int thisEndTimeInMinutes = thisStartingTimeInMinutes + this->duration;
	unsigned int otherEndTimeInMinutes = otherStartingTimeInMinutes + other.duration;

	return (thisStartingTimeInMinutes < otherEndTimeInMinutes) &&
		(otherStartingTimeInMinutes < thisEndTimeInMinutes);
}

const char* const Task::getDescription() const {
	return this->description;
}

unsigned int Task::getPriority() const {
	return this->priority;
}

unsigned int Task::getDuration() const {
	return this->duration;
}

bool Task::getStatus() const {
	return this->status;
}

void Task::markAsReady() {
	this->status = true;
}

void Task::print(std::ofstream& os) const {
	// Expects os to be valided from the outside world
	int descriptionLength = strlen(this->description);
	os << descriptionLength << " " << this->description << " "
		<< this->priority << " " << this->status << " " << this->duration << " " << this->startingTime;
}

char* Task::readDescriptionFromFile(std::ifstream& ifs) {
	int descriptionLength;
	ifs >> descriptionLength;
	char buff[256];
	ifs >> buff;

	if (ifs) {
		return strcpy(new char[strlen(buff) + 1], buff);
	}

	ifs.clear(); // clear the stream for the otside world
	throw std::runtime_error("Couldn't read file");
}

char* Task::readDescriptionFromObject(const Task& other) {
	return strcpy(new char[strlen(other.description) + 1], other.description);
}

bool Task::validateStartingTime(const char* time) {
	if (!time || std::strlen(time) != 5) return false;
	if (time[2] != ':') return false;

	int hours = (time[0] - '0') * 10 + (time[1] - '0');
	int minutes = (time[3] - '0') * 10 + (time[4] - '0');

	return (0 <= hours && hours <= 23) && (0 <= minutes && minutes <= 59);
}

bool Task::validateDuration(unsigned int duration, const char* startingTime) {
	if (!validateStartingTime(startingTime)) {
		return false;
	}

	unsigned int startingTimeInMinutes = getTimeInMinutes(startingTime);

	return (startingTimeInMinutes + duration) < 24 * 60;
}

unsigned int Task::getTimeInMinutes(const char* time) {
	int hours = (time[0] - '0') * 10 + (time[1] - '0');
	int minutes = (time[3] - '0') * 10 + (time[4] - '0');

	return hours * 60 + minutes;
}

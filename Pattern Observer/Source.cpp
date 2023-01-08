#include <iostream>
#include <string>
#include <memory>
#include <format>
#include <list>

class IObserver
{
public:
	virtual ~IObserver() = default;
	virtual void Update(const std::string& message) = 0;
};

class ISubject
{
public:
	virtual ~ISubject() = default;
	virtual void Attach(IObserver* observer) = 0;
	virtual void Detach(IObserver* observer) = 0;
	virtual void Notify() = 0;
};

class Subject : public ISubject
{
public:
	void Attach(IObserver* observer)
	{
		mObservers.push_back(observer);
	}

	void Detach(IObserver* observer)
	{
		mObservers.remove(observer);
	}

	void Notify()
	{
		for (const auto& observer : mObservers)
		{
			observer->Update(mMessage);
		}
	}

	void CreateEvent(const std::string& massage)
	{
		mMessage = massage;
		Notify();
	}

	void HowManyObservers()
	{
		std::cout << std::format("There are {} observers in the list.", mObservers.size());
		std::cout << std::endl;
	}
private:
	std::list<IObserver*> mObservers;
	std::string mMessage;
};

class Observer : public IObserver
{
public:
	Observer(Subject& subject) : mSubject(subject) 
	{
		mSubject.Attach(this);
		std::cout << std::format("Observer {} is created.", ++obsNumber);
		std::cout << std::endl;
		mNumber = obsNumber;
	}

	~Observer() override
	{
		RemoveMeFromList();
	}

	void AddMeToList()
	{
		mSubject.Attach(this);
		std::cout << std::format("Observer {} is added to the list.", mNumber);
		std::cout << std::endl;
	}

	void RemoveMeFromList()
	{
		mSubject.Detach(this);
		std::cout << std::format("Observer {} is removed from the list.", mNumber);
		std::cout << std::endl;
	}

	void Update(const std::string& message) override
	{
		std::cout << std::format("Observer {} got a new message: {}", mNumber, message);
		std::cout << std::endl;
	}

private:
	Subject& mSubject;
	static int obsNumber;
	int mNumber{ 0 };
};

int Observer::obsNumber = 0;

int main()
{
	Subject subject;
	Observer* obs1 = new Observer(subject);
	Observer* obs2 = new Observer(subject);
	Observer* obs3 = new Observer(subject);
	subject.HowManyObservers();
	subject.CreateEvent("The first event!!!");

	Observer* obs4 = new Observer(subject);
	Observer* obs5 = new Observer(subject);

	obs1->RemoveMeFromList();
	obs2->RemoveMeFromList();
	obs3->RemoveMeFromList();

	subject.HowManyObservers();
	subject.CreateEvent("The second event!!!");

	obs1->AddMeToList();
	obs2->AddMeToList();
	obs3->AddMeToList();

	subject.HowManyObservers();
	subject.CreateEvent("The third event!!!");

	delete obs1;
	delete obs2;
	delete obs3;
	delete obs4;
	delete obs5;

	return 0;
}
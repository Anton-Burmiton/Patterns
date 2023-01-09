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
	virtual void Attach(std::weak_ptr<IObserver> observer) = 0;
	virtual void Detach(std::weak_ptr<IObserver> observer) = 0;
	virtual void Notify() = 0;
};

class Subject : public ISubject
{
public:
	void Attach(std::weak_ptr<IObserver> observer)
	{
		mObservers.push_back(observer);
	}

	void Detach(std::weak_ptr<IObserver> observer)
	{
		mObservers.erase(std::remove_if(mObservers.begin(), mObservers.end(),
			[&](const std::weak_ptr<IObserver>& wptr)
			{
				return wptr.expired() || wptr.lock() == observer.lock();
			}),
			mObservers.end());
	}

	void Notify()
	{
		for (const auto& observer : mObservers)
		{
			auto obs = observer.lock();
			if (obs)
			{
				obs->Update(mMessage);
			}
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
	std::list<std::weak_ptr<IObserver>> mObservers;
	std::string mMessage;
};

class Observer : public IObserver, public std::enable_shared_from_this<Observer>
{
public:
	Observer(Subject& subject) : mSubject(subject)
	{
		std::cout << std::format("Observer {} is created.", ++obsNumber);
		std::cout << std::endl;
		mNumber = obsNumber;
	}

	~Observer() override
	{
		Unsubscribe();
	}

	void Subscribe()
	{
		mSubject.Attach(weak_from_this());
		std::cout << std::format("Observer {} is added to the list.", mNumber);
		std::cout << std::endl;
	}

	void Unsubscribe()
	{
		mSubject.Detach(weak_from_this());
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
	std::shared_ptr<Observer> obs1(new Observer(subject));
	std::shared_ptr<Observer> obs2(new Observer(subject));
	std::shared_ptr<Observer> obs3(new Observer(subject));
	obs1->Subscribe();
	obs2->Subscribe();
	obs3->Subscribe();
	subject.HowManyObservers();
	subject.CreateEvent("The first event!!!");

	std::shared_ptr<Observer> obs4(new Observer(subject));
	std::shared_ptr<Observer> obs5(new Observer(subject));
	obs4->Subscribe();
	obs5->Subscribe();

	obs1->Unsubscribe();
	obs2->Unsubscribe();
	obs3->Unsubscribe();

	subject.HowManyObservers();
	subject.CreateEvent("The second event!!!");

	obs1->Subscribe();
	obs2->Subscribe();
	obs3->Subscribe();

	subject.HowManyObservers();
	subject.CreateEvent("The third event!!!");

	return 0;
}
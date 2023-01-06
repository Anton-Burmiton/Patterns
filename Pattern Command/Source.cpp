#include <iostream>
#include <string>
#include <memory>
#include <format>
#include <vector>

class Command
{
public:
	Command() = default;

	// the rule of five
	Command(const Command&) = default;
	Command(Command&&) = default;
	Command& operator=(const Command&) = default;
	Command& operator=(Command&&) = default;
	virtual ~Command() = default;

	virtual void Execute() const = 0;
};

class PrintCommand : public Command
{
public:
	PrintCommand(std::string dataToPrint) : mDataToPrint(std::move(dataToPrint)) {}

	void Execute() const override
	{
		std::cout << std::format("Printing data: {}", mDataToPrint);
		std::cout << std::endl;
	}
private:
	std::string mDataToPrint;
};

class Reciever
{
public:
	std::string Double(const std::string& dataToDouble)
	{
		std::cout << std::format("Doubling data...");
		std::cout << std::endl;
		return std::string(dataToDouble + dataToDouble);
	}
	std::string Reverse(const std::string& dataToReverse)
	{
		std::cout << std::format("Reversing data...");
		std::cout << std::endl;
		return std::string(dataToReverse.rbegin(), dataToReverse.rend());
	}

	void Print(const std::string& dataToReverse) const
	{
		std::cout << std::format("Printing data: {}", dataToReverse);
		std::cout << std::endl;
	}
};

class ReversePrintCommand : public Command
{
public:
	ReversePrintCommand(Reciever* receiver, std::string data) : mReciever(receiver), mData(std::move(data)) {}

	void Execute() const override
	{
		if (mReciever)
		{
			auto data = mReciever->Reverse(mData);
			mReciever->Print(data);
		}
	}
private:
	Reciever* mReciever{ nullptr };
	std::string mData;
};

class Invoker
{
public:
	void AddCommand(std::unique_ptr<Command>&& command)
	{
		mCommands.push_back(std::move(command));
	}

	void ExecuteCommands()
	{
		for (const auto& command : mCommands)
		{
			command->Execute();
		}
	}
private:
	std::vector<std::unique_ptr<Command>> mCommands;
};

int main()
{
	Invoker inv;
	inv.AddCommand(std::make_unique<PrintCommand>(("I love commands!")));
	Reciever reciever;
	inv.AddCommand(std::make_unique<ReversePrintCommand>(&reciever, "Complex command!"));
	inv.ExecuteCommands();
	return 0;
}
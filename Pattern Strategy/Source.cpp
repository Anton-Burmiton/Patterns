#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <utility>


class Strategy
{
public:
	Strategy() = default;
	// the rule of five
	Strategy(const Strategy&) = default;
	Strategy(Strategy&&) = default;
	Strategy& operator=(const Strategy&) = default;
	Strategy& operator=(Strategy&&) = default;
	virtual ~Strategy() = default;

	virtual std::string DoAlgorithm(std::string_view data) const = 0;
};

class RemoveSpacesStrategy : public Strategy
{
public:
	std::string DoAlgorithm(std::string_view data) const override
	{
		std::string result(data);
		result.erase(std::remove_if(result.begin(), result.end(), std::isspace), result.end());
		return result;
	}
};

class ReverseStringStrategy : public Strategy
{
public:
	std::string DoAlgorithm(std::string_view data) const override
	{
		std::string result(data);
		std::reverse(result.begin(), result.end());
		return result;
	}
};

class Context
{
public:
	Context(std::unique_ptr<Strategy>&& strategy, const std::string& data) : mStrategy(std::move(strategy)), mData(data) {}

	void SetStrategy(std::unique_ptr<Strategy>&& strategy)
	{
		mStrategy = std::move(strategy);
	}

	void SetData(const std::string& data)
	{
		mData = data;
	}

	void DoStrategy()
	{
		if (mStrategy)
		{
			std::cout << "Data: " << mData << std::endl;
			std::string result = mStrategy->DoAlgorithm(mData);
			std::cout << "Result: " << result << std::endl;
		}
		else
		{
			std::cout << "Context: Strategy isn't set!" << std::endl;
		}
	}

private:
	std::unique_ptr<Strategy> mStrategy;
	std::string mData;
};

int main()
{
	// client code
	Context context(std::make_unique<ReverseStringStrategy>(), "I have a data string!");
	context.DoStrategy();
	context.SetStrategy(std::make_unique<RemoveSpacesStrategy>());
	context.DoStrategy();
	std::string newData{ "32167 This is cheat!" };
	context.SetData(newData);
	context.DoStrategy();
	auto reverseStringStrategy = std::make_unique<ReverseStringStrategy>();
	context.SetStrategy(std::move(reverseStringStrategy));
	context.DoStrategy();
	return 0;
}
#include <iostream>
#include <format>
#include <vector>

class CharacterNamesService
{
public:
	virtual std::string GenerateCharName()
	{
		static int number = 0;
		return std::format("Char{}", std::to_string(number));
	}
	virtual ~CharacterNamesService() = default;
};

class GameClient
{
public:
	void GetName(CharacterNamesService& service)
	{
		std::cout << service.GenerateCharName() << std::endl;
	}
};

class NPCService
{
public:
	std::vector<unsigned int> GetCipheredName()
	{
		static unsigned int number = 0;
		return { 1, 14, 20, 15, 14, ++number };
	}
};

class NPCNamesAdapter : public CharacterNamesService
{
public:
	NPCNamesAdapter(NPCService* NPCservice) : mNPCService(NPCservice) {}
	std::string GenerateCharName() override
	{
		const auto&& npcCipheredName = mNPCService->GetCipheredName();
		std::string decipheredName;
		for (auto i = 0; i < npcCipheredName.size() - 1; ++i)
		{
			decipheredName.push_back('a' + npcCipheredName[i] - 1);
		}
		return decipheredName + std::to_string(npcCipheredName.back());
	}
private:
	NPCService* mNPCService{nullptr};
};


class NPCNamesAdapterMultipleInheritance : public CharacterNamesService, public NPCService
{
public:
	std::string GenerateCharName() override
	{
		const auto&& npcCipheredName = GetCipheredName();
		std::string decipheredName;
		for (auto i = 0; i < npcCipheredName.size() - 1; ++i)
		{
			decipheredName.push_back('a' + npcCipheredName[i] - 1);
		}
		return decipheredName + std::to_string(npcCipheredName.back());
	}
};


int main()
{
	CharacterNamesService cns;
	GameClient gc;
	gc.GetName(cns);
	NPCService npcService;
	NPCNamesAdapter adapter(&npcService);
	gc.GetName(adapter);

	NPCNamesAdapterMultipleInheritance mi;
	gc.GetName(mi);
	return 0;
}
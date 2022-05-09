// used: shgetknownfolderpath
#include <shlobj.h>
// used: json parser implementation
#include <json.hpp>

#include "Config.h"

#include "../../Utilities/Console/Logging.h"

bool Config::Setup(std::string_view szDefaultFileName) {
	// create directory "settings" in "%appdata%/roaming" if it incorrect or doesnt exists
	if (!std::filesystem::is_directory(fsPath))
	{
		std::filesystem::remove(fsPath);
		if (!std::filesystem::create_directories(fsPath))
			return false;
	}

	// create default config
	if (!Save(szDefaultFileName))
		return false;

	// load default config
	if (!Load(szDefaultFileName))
		return false;

	// refresh configs list
	Refresh();

	return true;
}

bool Config::Save(std::string_view szFileName)
{
	// check for extension if it is not our replace it
	std::filesystem::path fsFilePath(szFileName);
	if (fsFilePath.extension() != ".cfg")
		fsFilePath.replace_extension(".cfg");

	// get utf-8 full path to config
	const std::string szFile = std::filesystem::path(fsPath / fsFilePath).string();
	nlohmann::json config = { };

	try
	{
		for (auto& variable : vecVariables)
		{
			nlohmann::json entry = { };

			// save hashes to compare it later
			entry["name-id"] = variable.uNameHash;
			entry["type-id"] = variable.uTypeHash;

			// get current variable
			switch (variable.uTypeHash)
			{
			case FNV1A::HashConst("int"):
			{
				entry["value"] = variable.Get<int>();
				break;
			}
			case FNV1A::HashConst("float"):
			{
				entry["value"] = variable.Get<float>();
				break;
			}
			case FNV1A::HashConst("bool"):
			{
				entry["value"] = variable.Get<bool>();
				break;
			}
			case FNV1A::HashConst("std::string"):
			{
				entry["value"] = variable.Get<std::string>();
				break;
			}
			case FNV1A::HashConst("std::vector<bool>"):
			{
				const auto& vecBools = variable.Get<std::vector<bool>>();

				// store vector values as sub-node
				nlohmann::json sub = { };

				// fill node with all vector values
				for (const auto&& bValue : vecBools)
					sub.push_back(static_cast<bool>(bValue));

				entry["value"] = sub.dump();
				break;
			}
			case FNV1A::HashConst("std::vector<int>"):
			{
				const auto& vecInts = variable.Get<std::vector<int>>();

				// store vector values as sub-node
				nlohmann::json sub = { };

				// fill node with all vector values
				for (const auto& iValue : vecInts)
					sub.push_back(iValue);

				entry["value"] = sub.dump();
				break;
			}
			case FNV1A::HashConst("std::vector<float>"):
			{
				const auto& vecFloats = variable.Get<std::vector<float>>();

				// store vector values as sub-node
				nlohmann::json sub = { };

				// fill node with all vector values
				for (const auto& flValue : vecFloats)
					sub.push_back(flValue);

				entry["value"] = sub.dump();
				break;
			}
			default:
				break;
			}

			// add current variable to config
			config.push_back(entry);
		}
	}
	catch (const nlohmann::detail::exception& ex)
	{
		Console::PushConsoleColor(FOREGROUND_RED);
		Console::Print("Config Error", "Json save failed: {}", ex.what());
		Console::PopConsoleColor();
		return false;
	}

	// open output config file
	std::ofstream ofsOutFile(szFile, std::ios::out | std::ios::trunc);

	if (!ofsOutFile.good())
		return false;

	try
	{
		// write stored variables
		ofsOutFile << config.dump(4);
		ofsOutFile.close();
	}
	catch (std::ofstream::failure& ex)
	{
		Console::PushConsoleColor(FOREGROUND_RED);
		Console::Print("Config Error", "Failed to save config: {}", ex.what());
		Console::PopConsoleColor();
		return false;
	}

	Console::Print("Config Info", "Saved config file at: {}", szFile);
	return true;
}

bool Config::Load(std::string_view szFileName)
{
	// get utf-8 full path to config
	const std::string szFile = std::filesystem::path(fsPath / szFileName).string();
	nlohmann::json config = { };

	// open input config file
	std::ifstream ifsInputFile(szFile, std::ios::in);

	if (!ifsInputFile.good())
		return false;

	try
	{
		// parse saved variables
		config = nlohmann::json::parse(ifsInputFile, nullptr, false);

		// check is json parse failed
		if (config.is_discarded())
			return false;

		ifsInputFile.close();
	}
	catch (std::ifstream::failure& ex)
	{
		Console::PushConsoleColor(FOREGROUND_RED);
		Console::Print("Config Error", "Failed to load Config File : {}", ex.what());
		Console::PopConsoleColor();
		return false;
	}

	try
	{
		for (const auto& variable : config)
		{
			const std::size_t nIndex = GetVariableIndex(variable["name-id"].get<FNV1A_t>());

			// check is variable exist
			if (nIndex == CInvalidVariable)
				continue;

			// get variable
			auto& entry = vecVariables.at(nIndex);

			switch (variable["type-id"].get<FNV1A_t>())
			{
			case FNV1A::HashConst("bool"):
			{
				entry.Set<bool>(variable["value"].get<bool>());
				break;
			}
			case FNV1A::HashConst("float"):
			{
				entry.Set<float>(variable["value"].get<float>());
				break;
			}
			case FNV1A::HashConst("int"):
			{
				entry.Set<int>(variable["value"].get<int>());
				break;
			}
			case FNV1A::HashConst("std::string"):
			{
				entry.Set<std::string>(variable["value"].get<std::string>());
				break;
			}
			case FNV1A::HashConst("std::vector<bool>"):
			{
				const nlohmann::json vector = nlohmann::json::parse(variable["value"].get<std::string>());
				auto& vecBools = entry.Get<std::vector<bool>>();

				for (std::size_t i = 0U; i < vector.size(); i++)
				{
					// check is item out of bounds
					if (i < vecBools.size())
						vecBools.at(i) = vector.at(i).get<bool>();
				}

				break;
			}
			case FNV1A::HashConst("std::vector<int>"):
			{
				const nlohmann::json vector = nlohmann::json::parse(variable["value"].get<std::string>());
				auto& vecInts = entry.Get<std::vector<int>>();

				for (std::size_t i = 0U; i < vector.size(); i++)
				{
					// check is item out of bounds
					if (i < vecInts.size())
						vecInts.at(i) = vector.at(i).get<int>();
				}

				break;
			}
			case FNV1A::HashConst("std::vector<float>"):
			{
				const nlohmann::json vector = nlohmann::json::parse(variable["value"].get<std::string>());
				auto& vecFloats = entry.Get<std::vector<float>>();

				for (std::size_t i = 0U; i < vector.size(); i++)
				{
					// check is item out of bounds
					if (i < vecFloats.size())
						vecFloats.at(i) = vector.at(i).get<float>();
				}

				break;
			}
			default:
				break;
			}
		}
	}
	catch (const nlohmann::detail::exception& ex)
	{
		Console::PushConsoleColor(FOREGROUND_RED);
		Console::Print("Config Error", "Json load failed: {}", ex.what());
		Console::PopConsoleColor();
		return false;
	}

	Console::Print("Config Info", "Loaded Config File at: {}", szFile);
	return true;
}

void Config::Remove(const std::size_t nIndex)
{
	const std::string& szFileName = vecFileNames.at(nIndex);

	// unable delete default config
	if (szFileName.compare("main.cfg") == 0)
		return;

	// get utf-8 full path to config
	const std::string szFile = std::filesystem::path(fsPath / szFileName).string();

	if (std::filesystem::remove(szFile))
	{
		vecFileNames.erase(vecFileNames.cbegin() + static_cast<std::ptrdiff_t>(nIndex));
		Console::Print("Config Info", "Removed Config File at: {}", szFile);
	}
}

void Config::Refresh()
{
	vecFileNames.clear();

	for (const auto& it : std::filesystem::directory_iterator(fsPath))
    {
		if (it.path().filename().extension() == ".cfg")
		{
			Console::Print("Config Info", "Found Config File: {}", it.path().filename().string());
			vecFileNames.push_back(it.path().filename().string());
		}
    }
}

std::size_t Config::GetVariableIndex(const FNV1A_t uNameHash)
{
	for (std::size_t i = 0U; i < vecVariables.size(); i++)
	{
		if (vecVariables.at(i).uNameHash == uNameHash)
			return i;
	}

	return CInvalidVariable;
}


std::filesystem::path Config::GetWorkingPath()
{
	std::filesystem::path fsWorkingPath;

	// get path to Appdata Roaming
	if (PWSTR pszPathToRoaming; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0UL, nullptr, &pszPathToRoaming)))
	{
		fsWorkingPath.assign(pszPathToRoaming);
		fsWorkingPath.append(".cfg");
		CoTaskMemFree(pszPathToRoaming);
	}

	return fsWorkingPath;
}

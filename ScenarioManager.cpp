#include "ScenarioManager.h"
#include <fstream>
#include <sstream>
#include <vector>


ScenarioManager::ScenarioManager()
{
}

ScenarioManager::~ScenarioManager()
{
}

void ScenarioManager::LoadScenarioFile(const std::string& filename)
{
    this->scenarioBuffer.clear();
    this->currentLine = 0;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
    {
        this->scenarioBuffer.push_back(line);
    }
}

void ScenarioManager::SaveScenarioFile(const std::string& filename)
{
    std::ofstream file(filename);
    for (const auto& line : this->scenarioBuffer)
    {
        file << line << '\n';
    }
}

std::string ScenarioManager::GetNextLine()
{
    if (this->currentLine < this->scenarioBuffer.size())
    {
        return this->scenarioBuffer[this->currentLine++];
    }
    return {};
}
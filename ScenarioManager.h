#pragma once

#include <memory>
#include <string>
#include <vector>

class ScenarioManager
{
public:
    ScenarioManager();
    ~ScenarioManager();

    void LoadScenarioFile(const std::string& filename);
    void SaveScenarioFile(const std::string& filename);
    std::string GetNextLine();

private:
    std::vector<std::string> scenarioBuffer;
    size_t currentLine = 0;
};
#include "GameConfig.h"

DifficultyLevel GameConfig::currentDifficulty = DifficultyLevel::Beginner;
bool GameConfig::isGameOver = false;

const std::map<DifficultyLevel, DifficultySettings> GameConfig::difficultyMap = {
    {DifficultyLevel::Beginner, {9, 9, 10}},
    {DifficultyLevel::Intermediate, {16, 16, 40}},
    {DifficultyLevel::Expert, {16, 30, 99}},
};

void GameConfig::setCurrentDifficulty(DifficultyLevel newDifficulty) {
    currentDifficulty = newDifficulty;
}

DifficultyLevel GameConfig::getCurrentDifficulty() {
    return currentDifficulty;
}

DifficultySettings GameConfig::getDifficultySettings(DifficultyLevel difficulty) {
    return difficultyMap.at(difficulty);
}

bool GameConfig::getGameOverFlag() {
    return isGameOver;
}
void GameConfig::setGameOverFlag(bool isOver) {
    isGameOver = isOver;
}

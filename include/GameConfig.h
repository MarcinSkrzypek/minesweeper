#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include "DifficultyLevel.h"
#include <map>

struct DifficultySettings {
            int rows, cols, mines;
        };

class GameConfig
{
    public:
        static const std::map<DifficultyLevel, DifficultySettings> difficultyMap;
        static const int MAX_ROWS = 24;
        static const int MAX_COLUMNS = 30;
        static DifficultyLevel currentDifficulty;
        static DifficultyLevel getCurrentDifficulty();
        static void setCurrentDifficulty(DifficultyLevel newDifficulty);
        static DifficultySettings getDifficultySettings(DifficultyLevel difficulty);
};

#endif // GAMECONFIG_H

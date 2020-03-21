#include <fstream>
#include <string>
#include "res_path.hpp"
#include "field.hpp"

static BlockField readBlockField(const std::string& fileName)
{
    const std::string filePath = getResourcePath() + fileName;

    std::fstream file(filePath);

    BlockField blockField;

    for (auto&& row : blockField)
        for (auto&& el : row) {
            int fileValue;
            file >> fileValue;

            el = FileToBlock[fileValue];
        }

    return blockField;
}

static BlockField getBlockField()
{
    static auto blockField = readBlockField("field.txt");
    return blockField;
}

bool canGo(int x, int y)
{
    auto blockField = getBlockField();

    if (blockField[x / FIELD_SCALE][y / FIELD_SCALE] == FREE)
        return true;
    return false;
}


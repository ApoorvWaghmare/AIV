#ifndef MAPBUILDER_H
#define MAPBUILDER_H

#include <string>
#include <fstream>
#include <vector>

using namespace std;
namespace CommonUtil
{
    class Polygon;
    class Point2D;
}

class MapBuilder
{
    public:
        // default constructor
        MapBuilder();

        // destructor
        virtual ~MapBuilder();

        // build map by reading vertices from a text file
        void build(const std::string &mapFile, CommonUtil::Polygon &mapPolygon);

    protected:

    private:

        void getPolygonVerticesFromTextFile(const std::string &mapFile, std::vector< CommonUtil::Point2D > &vertices);
};

#endif // MAPBUILDER_H

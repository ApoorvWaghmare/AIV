#include "../CommonUtilReq/Include/CUPolygon.h"
#include "../CommonUtilReq/Include/CUPoint2D.h"
#include "MapBuilder.h"

//---------------------------------------------------------------------------------
MapBuilder::MapBuilder()
{
    //ctor
}

//---------------------------------------------------------------------------------
MapBuilder::~MapBuilder()
{
    //dtor
}

//---------------------------------------------------------------------------------
void MapBuilder::build(const std::string &mapName, CommonUtil::Polygon &mapPolygon)
{
    std::vector< CommonUtil::Point2D > vertices;

    getPolygonVerticesFromTextFile(mapName, vertices);

    mapPolygon.Set(vertices);
}

//---------------------------------------------------------------------------------
void MapBuilder::getPolygonVerticesFromTextFile(const std::string &mapName, std::vector< CommonUtil::Point2D > &vertices)
{
    std::string mapFile = "Maps/" + mapName + ".txt";

    fstream mapTextFile;
    mapTextFile.open(mapFile, ios::in);

    if (!mapTextFile)
    {
        cout << "Map does not exist" << endl;
        return;
    }

    double x, y;
    char comma;
    while (mapTextFile >> x >> comma >> y)
    {
        CommonUtil::Point2D tempPoint;
        tempPoint.SetX(x);
        tempPoint.SetY(y);

        vertices.push_back(tempPoint);
    }

    mapTextFile.close();
}

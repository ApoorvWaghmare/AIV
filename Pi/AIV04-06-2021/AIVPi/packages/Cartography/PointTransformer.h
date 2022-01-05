#ifndef POINTTRANSFORMER_H
#define POINTTRANSFORMER_H

#include <vector>;

using namespace std;

namespace CommonUtil
{
    class Transform3D;
}

class PointTransformer
{
    public:
        PointTransformer();
        virtual ~PointTransformer();

        // transform points
        void transformPoints(std::vector<CommonUtil::Point3D> &localPoints, double x, double y, double z, double rotation);

    protected:

    private:

    //transformation matrix
    CommonUtil::Transform3D getTransformationMatrix(double x, double y, double z, double rotation);

};

#endif // POINTTRANSFORMER_H

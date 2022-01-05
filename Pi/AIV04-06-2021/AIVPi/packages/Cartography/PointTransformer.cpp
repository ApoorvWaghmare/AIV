#include "../CommonUtilReq/Include/CUTransform3D.h"
#include "PointTransformer.h"

PointTransformer::PointTransformer()
{
    //ctor
}

PointTransformer::~PointTransformer()
{
    //dtor
}

//------------------------------------------------------

CommonUtil::Transform3D PointTransformer::getTransformationMatrix(double x, double y, double z, double rotation)
{
    CommonUtil::Transform3D transformationMatrix;

    transformationMatrix.SetTranslationVal(-x, -y, -z);
	transformationMatrix.SetRotationAboutAxisZ(rotation);

	const CommonUtil::SqMatrix transformationSqMatrix = transformationMatrix.GetTransformationMatrix();
    CommonUtil::SqMatrix inverseTransformationSqMatrix;
    transformationSqMatrix.GetInverse(inverseTransformationSqMatrix);
    transformationMatrix.SetTransformationMatrix(inverseTransformationSqMatrix);

	return transformationMatrix;
}

//---------------------------------------------------------

void PointTransformer::transformPoints(std::vector<CommonUtil::Point3D> &localPoints, double x, double y, double z,
                                       double rotation)
{
    // calculate transformation matrix
    CommonUtil::Transform3D transformationMatrix = getTransformationMatrix(x, y, z, rotation);

	// transform all points into global coordinate system
	size_t numPoint = localPoints.size();
	for (size_t iPoint = 0; iPoint < numPoint; iPoint++)
	{
		localPoints[iPoint].ApplyTransformation(transformationMatrix.GetTransformationMatrix());
	}
}

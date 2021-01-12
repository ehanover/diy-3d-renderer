#include "renderer.h"
#include <algorithm>
#include <cmath>
#include <iostream>

Renderer::Renderer(SDL_Renderer* renderer, float textureScale) :
	mRenderer(renderer),
	mTextureSizeX(0),
	mTextureSizeY(0),
	mTextureScaledBounds(),
	mTexture(),

	mCamera(),

	// mProjectionMat(),
	mZNear(1.5),
	mZFar(-3),
	mPerspectiveMat(),

	mVertsWorldRender(),
	mVertsScreenRender(),
	mTrisRender(),
	// mColorsRender(),

	mPixels(),
	mDepths()
{
	SDL_GetRendererOutputSize(mRenderer, &mTextureSizeX, &mTextureSizeY);
	mTextureScaledBounds = {.x=0, .y=0, .w=mTextureSizeX, .h=mTextureSizeY}; // https://gamedev.stackexchange.com/a/102881
	mTextureSizeX = (int) (mTextureSizeX*textureScale);
	mTextureSizeY = (int) (mTextureSizeY*textureScale);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); // https://wiki.libsdl.org/SDL_HINT_RENDER_SCALE_QUALITY
	mTexture = SDL_CreateTexture
		(
		mRenderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		mTextureSizeX, mTextureSizeY
		);
	mPixels = std::vector<uint8_t>(mTextureSizeX * mTextureSizeY * 4);
	mDepths = std::vector<uint16_t>(mTextureSizeX * mTextureSizeY);

	// Perspective

	// https://www.techspot.com/article/1888-how-to-3d-rendering-rasterization-ray-tracing/
	double worldScale = std::min(mTextureSizeX, mTextureSizeY);
	std::vector<double> perspectiveVec{
		worldScale, 0, 0, 0, // Makes assumption about FOV angle
		0, worldScale, 0, 0,
		0, 0, (mZFar)/(mZFar-mZNear), 1,
		0, 0, (-mZFar*mZNear)/(mZFar-mZNear), 0
	};

	// https://www.euclideanspace.com/threed/rendering/opengl/index.htm
	// std::vector<double> perspectiveVec{
	// 	(double)mTextureSizeX, 0, 0, 0,
	// 	0, (double)mTextureSizeY, 0, 0,
	// 	0, 0, (mZFar+mZNear)/(mZFar-mZNear), 1, // This might be -1 instead of 1
	// 	0, 0, (2.0*mZFar*mZNear)/(mZFar-mZNear), 0
	// };

	// double perspectiveDist = 1.0;
	// std::vector<double> perspectiveVec{
	// 	(double)mTextureSizeX, 0, 0, 0,
	// 	0, (double)mTextureSizeY, 0, 0,
	// 	0, 0, 1, (1/perspectiveDist),
	// 	0, 0, 0, 0
	// };

	mPerspectiveMat = MyMatrix(4, 4, perspectiveVec);
}

void Renderer::render(const std::vector<std::reference_wrapper<Object>>& objs, const Light& light) {
	std::fill(mPixels.begin(), mPixels.end(), 0);
	std::fill(mDepths.begin(), mDepths.end(), (2>>15)-1);
	mVertsWorldRender.clear();
	mVertsScreenRender.clear();
	mTrisRender.clear();
	mNormsRender.clear();
	// mColorsRender.clear();

	fakeVertexShader(objs); // The vertex shader takes in objs and breaks them down to verts+tris+norms+etc stored in member vars
							// Maybe this should instead return lists of verts+norms+etc that utilize counterclockwise winding order so tri storage can be eliminated
	fakeGeometryShader();
	fakeFragmentShader(light);
	// drawDebugVerts();

	SDL_UpdateTexture
		(
		mTexture,
		NULL,
		mPixels.data(),
		mTextureSizeX * 4
		);
	SDL_RenderCopy(mRenderer, mTexture, NULL, &mTextureScaledBounds);
}

void Renderer::drawDebugVerts() {
	for(size_t i=0; i<mVertsScreenRender.size(); i++) {
		// Assuming these vectors' w component is 1
		MyVector vert = mVertsScreenRender.at(i);
		// std::cout << "drawing debug vert " << vert << std::endl;
		int x = (int) vert.elem(0) + (mTextureSizeX/2); // Centered world coordinates
		int y = (int) vert.elem(1) + (mTextureSizeY/2);

		for(int s=0; s<3; s++) {
			mPixels[(y*mTextureSizeX*4) + ((x+s)*4) + 1] = 250;
			mPixels[(y*mTextureSizeX*4) + ((x+s)*4) + 3] = SDL_ALPHA_OPAQUE;
		}
	}
}

void Renderer::fakeVertexShader(const std::vector<std::reference_wrapper<Object>>& objs) {
	for(size_t i=0; i<objs.size(); i++) {
		Object& obj = objs.at(i).get();
		std::vector<MyVector> objVerts = obj.verts();
		std::vector<std::array<size_t, 3>> objTris = obj.tris();
		std::vector<MyVector> objNorms = obj.norms();

		// Apply transformation matrix
		double rotY = obj.rotation().elem(1);
		std::vector<double> rotYVec{ // Y rotation
			cos(rotY), 0, -sin(rotY), 0,
			0, 1, 0, 0,
			sin(rotY), 0, cos(rotY), 0,
			0, 0, 0, 1
		};
		// std::vector<double> rotYVec{ // X rotation
		// 	1, 0, 0, 0,
		// 	0, cos(rotY), -sin(rotY), 0,
		// 	0, sin(rotY), cos(rotY), 0,
		// 	0, 0, 0, 1
		// };
		MyMatrix transMat = MyMatrix(4, 4, rotYVec);
		// Multiply all transformations together before looping over object verts

		for(size_t j=0; j<objVerts.size(); j++) {
			MyVector objVert = objVerts.at(j);

			objVert.multiplyByMatrix(transMat);
			mVertsWorldRender.push_back(objVert);
			
			objVert.multiplyByMatrix(mCamera.projectionMatrix());
			objVert.multiplyByMatrix(mPerspectiveMat);
			objVert.scalar(1.0/objVert.elem(3)); // Homogenize
			mVertsScreenRender.push_back(objVert);
		}

		MyMatrix transMatNormals(transMat); // Used to transform the normal vectors along with the vert transformations
		transMatNormals.invert();
		transMatNormals.transpose();

		for(size_t j=0; j<objTris.size(); j++) {
			std::array<size_t, 3>& objTri = objTris.at(j);
			mTrisRender.push_back(objTri);

			// https://www.scratchapixel.com/lessons/3d-basic-rendering/transforming-objects-using-matrices
			// http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
			MyVector objNorm = objNorms.at(j); // Multiply normals by the tanspose of the inverse of the object-to-camera matrix
			objNorm.multiplyByMatrix(transMatNormals);
			mNormsRender.push_back(objNorm);
		}
	}

}

void Renderer::fakeGeometryShader() {
	// Nothing will ever happen here, probably
}

std::vector<double> Renderer::shiftVertOrigin(const MyVector& v) {
	return {v.elem(0) + (mTextureSizeX/2), v.elem(1) + (mTextureSizeY/2), v.elem(2)};
}

double Renderer::edgeFunction(const MyVector& a, const MyVector& b, const MyVector& c) {
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
	// return ( (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]) );
	return ( (c.elem(0) - a.elem(0)) * (b.elem(1) - a.elem(1)) - (c.elem(1) - a.elem(1)) * (b.elem(0) - a.elem(0)) );

	// return ( (a[0] - b[0]) * (c[1] - a[1]) - (a[1] - b[1]) * (c[0] - a[0]) ); // Supposed to fix rasterizing if vertices declared clockwise
}

double Renderer::facingRatio(const MyVector& norm) {
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/shading-normals
	return norm.dot(mCamera.cameraEye()); // This might also depend on cameraAt variable, but if cameraAt is all zero then it doesn't matter
}

uint16_t Renderer::scaledVertDepth(double dw) {
	// double f = (mZFar/(mZFar-mZNear)) + (1/dw)*(-mZFar*mZNear)/(mZFar-mZNear); // https://en.wikipedia.org/wiki/Z-buffering#Mathematics
	double f = (-dw + mZNear) / (mZNear + -mZFar); // (my intuition calculation)
	uint16_t r = (uint16_t) (((2<<15)-1) * f);
	// std::cout << "got z=" << z << ", f=" << f << ", r=" << r << std::endl;
	return r;
}

void Renderer::fakeFragmentShader(const Light& light) {
	// Calculates pixel colors from verts+tris+norms by rasterizing
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage

	for(size_t i=0; i<mTrisRender.size(); i++) {

		// Every tri should have a corresponding norm
		std::array<size_t, 3>& tri = mTrisRender.at(i);
		MyVector& norm = mNormsRender.at(i);
		double ratio = facingRatio(norm);

		if(ratio <= 0) { // https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/shading-normals
			continue;
		}

		MyVector triVertScreen1 = MyVector(shiftVertOrigin(mVertsScreenRender.at(tri[0]))); // 3D
		MyVector triVertScreen2 = MyVector(shiftVertOrigin(mVertsScreenRender.at(tri[1])));
		MyVector triVertScreen3 = MyVector(shiftVertOrigin(mVertsScreenRender.at(tri[2])));

		std::array<double, 3> triXs = {triVertScreen1.elem(0), triVertScreen2.elem(0), triVertScreen3.elem(0)};
		std::array<double, 3> triYs = {triVertScreen1.elem(1), triVertScreen2.elem(1), triVertScreen3.elem(1)};
		std::pair<double*, double*> triXsBounds = std::minmax_element(triXs.begin(), triXs.end());
		std::pair<double*, double*> triYsBounds = std::minmax_element(triYs.begin(), triYs.end());

		double area = std::abs(edgeFunction(triVertScreen1, triVertScreen2, triVertScreen3)); // Why is this sometimes negative?

		for(int y=(int)*(triYsBounds.first); y<(int)*(triYsBounds.second); y++) {
			for(int x=(int)*(triXsBounds.first); x<(int)*(triXsBounds.second); x++) {

				MyVector pixVert = MyVector({(double)x, (double)y, 0});
				MyVector triVertWorld1 = MyVector(mVertsWorldRender.at(tri[0])); // 4D
				MyVector triVertWorld2 = MyVector(mVertsWorldRender.at(tri[1]));
				MyVector triVertWorld3 = MyVector(mVertsWorldRender.at(tri[2]));

				double w1 = edgeFunction(triVertScreen2, triVertScreen3, pixVert);
				double w2 = edgeFunction(triVertScreen3, triVertScreen1, pixVert);
				double w3 = edgeFunction(triVertScreen1, triVertScreen2, pixVert);

				if(w1 >= 0 && w2 >= 0 && w3 >= 0) {
					w1 /= area;
					w2 /= area;
					w3 /= area;

					triVertWorld1.scalar(w1);
					triVertWorld2.scalar(w2);
					triVertWorld3.scalar(w3);
					MyVector pixVertWorldInterp = MyVector(triVertWorld1);
					pixVertWorldInterp.add(triVertWorld2);
					pixVertWorldInterp.add(triVertWorld3);
					// pixVertWorldInterp.scalar(1/pixVertWorldInterp.elem(3));

					double depthWorld = pixVertWorldInterp.elem(2);
					if(depthWorld < mZFar || depthWorld > mZNear) {
						continue;
					}
					uint16_t depthScaled = scaledVertDepth(depthWorld);
					size_t depthIndex = (y*mTextureSizeY) + x;
					if(depthScaled >= mDepths[depthIndex]) {
						continue;
					}
					mDepths[depthIndex] = depthScaled;

					pixVertWorldInterp.dropW();
					pixVertWorldInterp.normalize();

					MyVector pixToLight = MyVector(pixVertWorldInterp);
					pixToLight.scalar(-1);
					pixToLight.add(light.position());
					pixToLight.normalize();
					double ambient = 0.2;
					double diffuse = std::max(0.0, pixToLight.dot(norm)); // Eliminate negative values (the dot product shouldn't be negative)
					double light = std::min(1.0, ambient + diffuse);

					int arrayOffset = (y*mTextureSizeX*4) + (x*4);
					mPixels[arrayOffset + 0] = 255 * light;
					// mPixels[arrayOffset + 0] = 
					// mPixels[arrayOffset + 1] = 
					// mPixels[arrayOffset + 2] = 
					mPixels[arrayOffset + 3] = SDL_ALPHA_OPAQUE;
				}

			}
		}
	}

}

#include "renderer.h"
#include <algorithm>
#include <cmath>
#include <iostream>

Renderer::Renderer(SDL_Renderer* renderer, float textureScale, Camera camera, double zNear, double zFar) :
	mRenderer(renderer),
	mTextureSizeX(0),
	mTextureSizeY(0),
	mTextureScaledBounds(),
	mTexture(),

	mCamera(camera),
	mZNear(zNear),
	mZFar(zFar),
	mPerspectiveMat(),

	mVertsWorldRender(),
	mVertsScreenRender(),
	mTrisRender(),

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
	double fovMultiplier = 1.3; // Higher multiplier means narrower view
	std::vector<double> perspectiveVec{
		worldScale*fovMultiplier, 0, 0, 0, // Makes assumption about FOV angle
		0, worldScale*fovMultiplier, 0, 0,
		0, 0, (mZFar)/(mZFar-mZNear), 1,
		0, 0, (-mZFar*mZNear)/(mZFar-mZNear), 0
	};
	mPerspectiveMat = MyMatrix(4, 4, perspectiveVec);
}

void Renderer::render(const std::vector<std::reference_wrapper<Object>>& objs, const Light& light) {
	std::fill(mPixels.begin(), mPixels.end(), 180);
	std::fill(mDepths.begin(), mDepths.end(), (2>>16)-1);

	mVertsWorldRender.clear();
	mVertsScreenRender.clear();
	mColorsRender.clear();
	mTrisRender.clear();
	mNormsRender.clear();

	fakeVertexShader(objs); // The vertex shader takes in objs and breaks them down to verts+tris+norms+etc stored in member vars
							// Maybe this should instead return lists of verts+norms+etc that utilize counterclockwise winding order so tri storage can be eliminated
	// fakeGeometryShader();
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

		std::vector<double> v = shiftVertOrigin(mVertsScreenRender.at(i));
		int x = (int) v[0]; // Centered world coordinates
		int y = (int) v[1];

		if(x > mTextureSizeX || y > mTextureSizeY || x < 0 || y < 0) {
			continue;
		}
		// std::cout << "drawing debug vert " << vert << std::endl;
		for(int s=0; s<2; s++) {
			mPixels[(y*mTextureSizeX*4) + ((x+s)*4) + 2] = 250;
			mPixels[(y*mTextureSizeX*4) + ((x+s)*4) + 3] = SDL_ALPHA_OPAQUE;
		}
	}
}

void Renderer::fakeVertexShader(const std::vector<std::reference_wrapper<Object>>& objs) {
	size_t triObjOffset = 0;
	for(size_t i=0; i<objs.size(); i++) {
		Object& obj = objs.at(i);//.get();
		std::vector<MyVector> objVerts = obj.verts();
		std::vector<std::array<size_t, 3>> objTris = obj.tris();
		std::vector<MyVector> objNorms = obj.norms();
		// std::array<uint8_t, 3> objColor = obj.color();

		// Apply transformation matrix
		std::array<double, 3> pos = obj.position();
		std::array<double, 3> rot = obj.rotation();
		std::array<double, 3> scale = obj.scale();
		std::vector<double> rotXVec{ // X rotation
			1, 0, 0, 0,
			0, cos(rot[0]), -sin(rot[0]), 0,
			0, sin(rot[0]), cos(rot[0]), 0,
			0, 0, 0, 1
		};
		std::vector<double> rotYVec{ // Y rotation
			cos(rot[1]), 0, -sin(rot[1]), 0,
			0, 1, 0, 0,
			sin(rot[1]), 0, cos(rot[1]), 0,
			0, 0, 0, 1
		};
		std::vector<double> rotZVec{ // Z rotation
			cos(rot[2]), sin(rot[2]), 0, 0,
			-sin(rot[2]), cos(rot[2]), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		std::vector<double> posScaleVec {
			scale[0], 0, 0, 0,
			0, scale[1], 0, 0,
			0, 0, scale[2], 0, 
			pos[0], pos[1], pos[2], 1
		};
		MyMatrix transMat = MyMatrix(4, 4, rotXVec);
		transMat.multiply(MyMatrix(4, 4, rotYVec));
		transMat.multiply(MyMatrix(4, 4, rotZVec));
		transMat.multiply(MyMatrix(4, 4, posScaleVec));

		for(size_t j=0; j<objVerts.size(); j++) {
			MyVector objVert = objVerts.at(j);

			// mColorsRender.push_back(objColor);
			mColorsRender.push_back(obj.getVertColor(objVert)); // Do coloring on position in pre-transformation world space
			// Re-"calculating" colors every frame is silly if the color functions never change

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
			objTri[0] += triObjOffset;
			objTri[1] += triObjOffset;
			objTri[2] += triObjOffset;
			mTrisRender.push_back(objTri);

			// https://www.scratchapixel.com/lessons/3d-basic-rendering/transforming-objects-using-matrices
			// http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
			MyVector objNorm = objNorms.at(j); // Multiply normals by the tanspose of the inverse of the object-to-camera matrix
			objNorm.multiplyByMatrix(transMatNormals);
			mNormsRender.push_back(objNorm);
		}
		triObjOffset += objVerts.size();
	}

}

void Renderer::fakeGeometryShader() {
	// Nothing will ever happen here, probably
}

std::vector<double> Renderer::shiftVertOrigin(const MyVector& v) {
	// TODO it seems silly that this function goes from Vector to array to Vector, even though it probably isn't any less efficient
	return {v.elem(0) + (mTextureSizeX/2), v.elem(1) + (mTextureSizeY/2), v.elem(2)};
}

double Renderer::edgeFunction(const MyVector& a, const MyVector& b, const MyVector& c) {
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
	// return ( (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]) );
	return ( (c.elem(0) - a.elem(0)) * (b.elem(1) - a.elem(1)) - (c.elem(1) - a.elem(1)) * (b.elem(0) - a.elem(0)) );

	// return ( (a[0] - b[0]) * (c[1] - a[1]) - (a[1] - b[1]) * (c[0] - a[0]) ); // Supposed to fix rasterizing if vertices declared clockwise
}

uint16_t Renderer::scaledVertDepth(double dw) {
	// double f = (mZFar/(mZFar-mZNear)) + (1/dw)*(-mZFar*mZNear)/(mZFar-mZNear); // https://en.wikipedia.org/wiki/Z-buffering#Mathematics
	//     (this might be a formula that has non-linear depth, increasing resolution close to the camera)
	//     (or it might be for coordinates that have projection/perspective or something like that)

	double f = (-dw + mZNear) / (mZNear + -mZFar); // (my intuition calculation)
	uint16_t r = 65535 * f;
	return r;
}

void Renderer::fakeFragmentShader(const Light& light) {
	// Calculates pixel colors from verts+tris+norms by rasterizing
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
	for(size_t i=0; i<mTrisRender.size(); i++) {

		// Every tri should have a corresponding norm
		std::array<size_t, 3>& tri = mTrisRender.at(i);
		MyVector& norm = mNormsRender.at(i);

		MyVector cameraToTri = MyVector(mCamera.cameraEye());
		MyVector cullVert = mVertsWorldRender.at(tri[0]);
		cameraToTri.scalar(-1).add(cullVert.dropW());
		if(cameraToTri.dot(norm) >= 0) {
			continue;
		}

		MyVector triVertScreen1 = MyVector(shiftVertOrigin(mVertsScreenRender.at(tri[0]))); // 3D
		MyVector triVertScreen2 = MyVector(shiftVertOrigin(mVertsScreenRender.at(tri[1])));
		MyVector triVertScreen3 = MyVector(shiftVertOrigin(mVertsScreenRender.at(tri[2])));

		// MyVector triEdgeScreen1 = MyVector(triVertScreen1).scalar(-1).add(triVertScreen3); // For top-left rule
		// MyVector triEdgeScreen2 = MyVector(triVertScreen2).scalar(-1).add(triVertScreen1);
		// MyVector triEdgeScreen3 = MyVector(triVertScreen3).scalar(-1).add(triVertScreen2);

		std::array<uint8_t, 3>& color1 = mColorsRender.at(tri[0]);
		std::array<uint8_t, 3>& color2 = mColorsRender.at(tri[1]);
		std::array<uint8_t, 3>& color3 = mColorsRender.at(tri[2]);

		// double area = std::abs(edgeFunction(triVertScreen1, triVertScreen2, triVertScreen3)); // Why is this sometimes negative?
		double area = edgeFunction(triVertScreen1, triVertScreen2, triVertScreen3); // Why doesn't it matter when this is negative?

		std::array<double, 3> triXs = {triVertScreen1.elem(0), triVertScreen2.elem(0), triVertScreen3.elem(0)};
		std::array<double, 3> triYs = {triVertScreen1.elem(1), triVertScreen2.elem(1), triVertScreen3.elem(1)};
		double triXBoundStart = std::max(0, (int)*(std::min_element(triXs.begin(), triXs.end())));
		double triXBoundEnd = std::min(mTextureSizeX, 1 + (int)*(std::max_element(triXs.begin(), triXs.end())));
		double triYBoundStart = std::max(0, (int)*(std::min_element(triYs.begin(), triYs.end())));
		double triYBoundEnd = std::min(mTextureSizeY, 1 + (int)*(std::max_element(triYs.begin(), triYs.end())));

		for(int x=triXBoundStart; x<triXBoundEnd; x++) {
			for(int y=triYBoundStart; y<triYBoundEnd; y++) {
				MyVector pixVert = MyVector({(double)x, (double)y, 0});
				MyVector triVertWorld1 = MyVector(mVertsWorldRender.at(tri[0])); // 4D
				MyVector triVertWorld2 = MyVector(mVertsWorldRender.at(tri[1]));
				MyVector triVertWorld3 = MyVector(mVertsWorldRender.at(tri[2]));

				double w1 = edgeFunction(triVertScreen2, triVertScreen3, pixVert);
				double w2 = edgeFunction(triVertScreen3, triVertScreen1, pixVert);
				double w3 = edgeFunction(triVertScreen1, triVertScreen2, pixVert);

				// Top-left rule, skipped for now because I don't think it's a problem (details in scratchapixel post above)
				// bool shouldDraw = true;
				// shouldDraw &= (w1 == 0 ? ((triEdgeScreen1.elem(1) == 0 && triEdgeScreen1.elem(0) < 0) ||  triEdgeScreen1.elem(1) < 0) : (w1 > 0));
				// shouldDraw &= (w2 == 0 ? ((triEdgeScreen2.elem(1) == 0 && triEdgeScreen2.elem(0) < 0) ||  triEdgeScreen2.elem(1) < 0) : (w2 > 0));
				// shouldDraw &= (w3 == 0 ? ((triEdgeScreen3.elem(1) == 0 && triEdgeScreen3.elem(0) < 0) ||  triEdgeScreen3.elem(1) < 0) : (w3 > 0));
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
					size_t depthIndex = (y*mTextureSizeX) + x;
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
					double ambient = 0.15;
					double diffuse = std::max(0.0, pixToLight.dot(norm));
					double light = std::min(1.0, ambient + diffuse);

					uint8_t ir = color1[0]*w1 + color2[0]*w2 + color3[0]*w3;
					uint8_t ig = color1[1]*w1 + color2[1]*w2 + color3[1]*w3;
					uint8_t ib = color1[2]*w1 + color2[2]*w2 + color3[2]*w3;

					int arrayOffset = (y*mTextureSizeX*4) + (x*4);
					mPixels[arrayOffset + 0] = ib * light;
					mPixels[arrayOffset + 1] = ig * light;
					mPixels[arrayOffset + 2] = ir * light;
					mPixels[arrayOffset + 3] = SDL_ALPHA_OPAQUE;
				}

			}
		}
	}

}

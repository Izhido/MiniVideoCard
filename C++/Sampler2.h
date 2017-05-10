//
//  Sampler2.h
//  MiniVideoCard
//
//  Created by Heriberto Delgado on 5/02/17.
//  Copyright � 2017 Heriberto Delgado. All rights reserved.
//

#pragma once

#include "Texture2.h"
#include "Filter.h"
#include "Wrap.h"
#include "Vector4.h"

namespace MiniVideoCard
{

	class Sampler2
	{
	public:
		Filter MinificationFilter();

		void SetMinificationFilter(Filter filter);

		Filter MagnificationFilter();

		void SetMagnificationFilter(Filter filter);

		Wrap HorizontalWrap();

		void SetHorizontalWrap(Wrap wrap);

		Wrap VerticalWrap();

		void SetVerticalWrap(Wrap wrap);

		Vector4& Border();

		void GenerateMipmaps(Texture2& texture);

		double Sample(Texture2& texture, Vector2& position, Fragment& fragment);

		void Sample(Texture2& texture, Vector2& position, Fragment& fragment, Vector2& result);

		void Sample(Texture2& texture, Vector2& position, Fragment& fragment, Vector3& result);

		void Sample(Texture2& texture, Vector2& position, Fragment& fragment, Vector4& result);

	private:
		Filter minificationFilter = Bilinear;

		Filter magnificationFilter = NearestNeighbor;

		Wrap horizontalWrap = Repeat;

		Wrap verticalWrap = Repeat;

		Vector4 border;

        void FilterAndLevelFor(Texture2& texture, Vector2& position, Fragment& fragment, Filter& filter, size_t& level);
        
        void CoordinateFor(double& coordinate, bool& outOfBounds, bool& inverted, double size, Wrap wrap);
        
        void CoordinatesFor(double& x, double& y, bool& xOutOfBounds, bool& yOutOfBounds, bool& xInverted, bool& yInverted, double width, double height);
	};
}

#pragma once
#include "Graphics.h"
#include "RectF.h"

class Beveller
{
public:
	Beveller() = default;
	Beveller(Color baseColor);
	void SetBaseColor(Color base);
	void DrawOuterBevel(const RectF& rect, int size, Graphics& gfx) const;
	void DrawInnerBevel(const RectF& rect, int size, Graphics& gfx) const;
	void DrawBevelledBrick(const RectF& rect, int size, Graphics& gfx) const;
	void DrawBevelledFrame(const RectF& rect, int size, Graphics& gfx) const;

private:
	Color baseColor;
	Color topColor;
	Color leftColor;
	Color rightColor;
	Color bottomColor;
};
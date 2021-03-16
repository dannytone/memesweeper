#pragma once
#include "Graphics.h"
#include "RectI.h"

class Beveller
{
public:
	Beveller() = default;
	Beveller(Color baseColor);
	void SetBaseColor(Color base);
	void DrawOuterBevel(const RectI& rect, int size, Graphics& gfx) const;
	void DrawInnerBevel(const RectI& rect, int size, Graphics& gfx) const;
	void DrawBevelledBrick(const RectI& rect, int size, Graphics& gfx) const;
	void DrawBevelledFrame(const RectI& rect, int size, Graphics& gfx) const;

private:
	Color baseColor;
	Color topColor;
	Color leftColor;
	Color rightColor;
	Color bottomColor;
};
#define NOMINMAX
#include"Beveller.h"

Beveller::Beveller(Color baseColor)
{
	SetBaseColor(baseColor);
}

void Beveller::DrawOuterBevel(const RectF& rect, int size, Graphics& gfx) const
{
	float top = rect.top;
	float left = rect.left;
	float right = rect.right;
	float bottom = rect.bottom;
	//top
	gfx.DrawTriangleTR(left + size, top, size, topColor);
	gfx.DrawRect(left + size, top, right - size, top + size, topColor);
	gfx.DrawTriangleTL(right - size, top, size, topColor);
	//left
	gfx.DrawTriangleBL(left, top + size-1, size, leftColor);
	gfx.DrawRect(left, top + size, left + size, bottom - size, leftColor);
	gfx.DrawTriangleTL(left, bottom - size, size, leftColor);
	//right
	gfx.DrawTriangleBR(right-1, top + size-1, size, rightColor);
	gfx.DrawRect(right - size, top + size, right, bottom - size, rightColor);
	gfx.DrawTriangleTR(right-1, bottom - size, size, rightColor);
	//bottom
	gfx.DrawTriangleBR(left + size-1, bottom-1, size, bottomColor);
	gfx.DrawRect(left + size, bottom - size, right - size, bottom, bottomColor);
	gfx.DrawTriangleBL(right - size, bottom-1, size, bottomColor);
}

void Beveller::DrawInnerBevel(const RectF& rect, int size, Graphics& gfx) const
{
	float top = rect.top;
	float left = rect.left;
	float right = rect.right;
	float bottom = rect.bottom;
	//top
	gfx.DrawTriangleTR(left + size, top, size, bottomColor);
	gfx.DrawRect(left + size, top, right - size, top + size, bottomColor);
	gfx.DrawTriangleTL(right - size, top, size, bottomColor);
	//left
	gfx.DrawTriangleBL(left, top + size - 1, size, rightColor);
	gfx.DrawRect(left, top + size, left + size, bottom - size, rightColor);
	gfx.DrawTriangleTL(left, bottom - size, size, rightColor);
	//right
	gfx.DrawTriangleBR(right - 1, top + size - 1, size, leftColor);
	gfx.DrawRect(right - size, top + size, right, bottom - size, leftColor);
	gfx.DrawTriangleTR(right - 1, bottom - size, size, leftColor);
	//bottom
	gfx.DrawTriangleBR(left + size - 1, bottom - 1, size, topColor);
	gfx.DrawRect(left + size, bottom - size, right - size, bottom, topColor);
	gfx.DrawTriangleBL(right - size, bottom - 1, size, topColor);
}

void Beveller::DrawBevelledBrick(const RectF& rect, int size, Graphics& gfx) const
{
	DrawOuterBevel(rect, size, gfx);
	gfx.DrawRect(rect.GetExpanded(-size), baseColor);
}

void Beveller::DrawBevelledFrame(const RectF& rect, int size, Graphics& gfx) const
{
	RectF inner = rect.GetExpanded(size);
	RectF outer = rect.GetExpanded(size*2);
	DrawInnerBevel(inner, size, gfx);
	DrawOuterBevel(outer, size, gfx);
}

void Beveller::SetBaseColor(Color base)
{
	int lr = std::min(int(base.GetR()) + 100, 255);
	int lg = std::min(int(base.GetG()) + 100, 255);
	int lb = std::min(int(base.GetB()) + 100, 255);
	int rr = std::max(int(base.GetR()) - 100, 0);
	int rg = std::max(int(base.GetG()) - 100, 0);
	int rb = std::max(int(base.GetB()) - 100, 0);
	int tr = std::min(int(base.GetR()) + 50, 255);
	int tg = std::min(int(base.GetG()) + 50, 255);
	int tb = std::min(int(base.GetB()) + 50, 255);
	int br = std::max(int(base.GetR()) - 50, 0);
	int bg = std::max(int(base.GetG()) - 50, 0);
	int bb = std::max(int(base.GetB()) - 50, 0);
	baseColor = base;
	leftColor = Color(lr, lg, lb);
	topColor = Color(tr, tg, tb);
	rightColor = Color(rr, rg, rb);
	bottomColor = Color(br, bg, bb);
}


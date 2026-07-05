// physics/game/sprite/sprite.h
#pragma once
#include "../../core/main.h"
#include "../../renderer/renderer.h"
#include "../object/color.h"

void InitSprite();
void UninitSprite();
void UpdateSprite();
void DrawSprite(float x, float y, float width, float height);
void DrawSpriteColor(float x, float y, float width, float height, Color4 color);
void DrawLine(float x1, float y1, float x2, float y2, float bold = 10.0f);
// 追加: SDF 円描画 (cx,cy=中心, radius=半径)
void DrawCircleColor(float cx, float cy, float radius, Color4 color);

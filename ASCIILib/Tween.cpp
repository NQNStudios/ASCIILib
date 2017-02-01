#include "Tween.h"

#include <algorithm>
using std::max;

Tween::Tween(Surface* surface, Point source, Point dest, unsigned int totalMS)
    : mSurface(surface), mCurrentPos(source), mDestPos(dest), mElapsedMS(0)
{
    // Delta between source and destination (+ or -)
    const int dx = dest.x - source.x;
    const int dy = dest.y - source.y;

    // Distance between source and destination (positive)
    const unsigned int sx = abs(dx);
    const unsigned int sy = abs(dy);

    // Calculate the movement that must be taken in each direction every step
    mStepX = sx == 0 ? 0 : dx / (int)sx;
    mStepY = sy == 0 ? 0 : dy / (int)sy;

    // The number of steps that must be taken
    const int steps = max(sx, sy);

    // Calculate the time per step
    mStepMS = totalMS / steps;
}

void Tween::Update(int deltaMS)
{
    mElapsedMS += deltaMS;

    if (mElapsedMS >= mStepMS)
    {
        mElapsedMS -= mStepMS;

        // take a step
        if (mCurrentPos.x != mDestPos.x)
            mCurrentPos.x += mStepX;

        if (mCurrentPos.y != mDestPos.y)
            mCurrentPos.y += mStepY;
    }
}

void Tween::Draw(Graphics& graphics)
{
    graphics.blitSurface(mSurface, mCurrentPos.x, mCurrentPos.y);
}

void Tween::SkipAnimation()
{
    mCurrentPos = mDestPos;
}

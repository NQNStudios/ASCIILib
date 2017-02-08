#pragma once

#include <functional>
using namespace std;

#include "Surface.h"
#include "Graphics.h"
#include "Point.h"
using namespace ascii;


namespace ascii
{

// Represents an ongoing surface tween, as an ASCII surface moves from one
// point to another.
class Tween
{
    public:
        Tween(Surface* surface, Point source, Point dest, unsigned int totalMS);

        void Update(int deltaMS);
        void Draw(Graphics& graphics);

        void SkipAnimation();

        bool IsFinished() const { return mCurrentPos == mDestPos; }

    private:
        Surface* mSurface;

        Point mCurrentPos;
        Point mDestPos;

        int mStepX;
        int mStepY;

        unsigned int mElapsedMS;
        unsigned int mStepMS;
};

struct TweenIsFinished : public unary_function<const Tween&, bool>
{
    bool operator()(const Tween& tween) const
    { 
        return tween.IsFinished();
    }
};

}

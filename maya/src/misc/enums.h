

#ifndef PaintingEnums_H
#define PaintingEnums_H

class PaintingEnums
{

public:
    enum TargetDisplay
    {
        kTargetsNone,
        kTargetsPoint,
        kTargetsLine,
        kTargetsMatrix
    };

    enum TargetColorsDisplay
    {
        kTargetColorsOff,
        kTargetColorsWhite,
        kTargetColorsRGB,
        kTargetColorsBlend,
        kTargetColorsWait
    };

    enum BrushRotateOrder
    {
        kTwistTiltBank,
        kTiltBankTwist,
        kBankTwistTilt,
        kTiltTwistBank,
        kTwistBankTilt,
        kBankTiltTwist,
    };
};

#endif

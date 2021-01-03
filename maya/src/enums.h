

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
        kTargetColorsBlend
    };
};

#endif

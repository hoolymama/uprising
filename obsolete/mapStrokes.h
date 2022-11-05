
#ifndef _mapStrokes_H
#define _mapStrokes_H

#include "strokeMutator.h"
#include "stroke.h"

class mapStrokes : public strokeMutator
{
public:
  mapStrokes();
  virtual ~mapStrokes();

  static void *creator();
  static MStatus initialize();
  virtual void postConstructor();
  virtual bool isAbstractClass() const { return false; }
  virtual MStatus mutate(
    const MPlug &plug, MDataBlock &data, std::vector<Stroke> *strokes) const;
  static MTypeId id;

private:

  void getPaintPotIds( MDataBlock &data, std::vector<std::pair<int,int>> &paintsPots) const ;
  static MObject aPaintId;
  static MObject aPotId;
  static MObject aPaintPot;
  static MObject aPaintPotTexture;



};

#endif
// UIToggleGroup.h is part of Noggit3, licensed via GNU General Publiicense (version 3).
// Bernd Lörwald <bloerwald+noggit@googlemail.com>
// Stephan Biegel <project.modcraft@googlemail.com>

#ifndef __TOGGLEGROUP_H
#define __TOGGLEGROUP_H

#include <map>

//! \note There is a circular include otherwise. I wish I could use ::Ptr.
class UICheckBox;

class UIToggleGroup
{
public:
  typedef UIToggleGroup* Ptr;

private:
  int* mTarget;
  typedef std::map<int, UICheckBox*> Frames;
  Frames mFrames;

public:
  explicit UIToggleGroup( int * pTarget );

  void Add( UICheckBox* pFrame, int pValue );

  void Activate( UICheckBox* pFrame );
  void Activate( int pID );
};
#endif

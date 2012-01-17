// Misc.h is part of Noggit3, licensed via GNU General Publiicense (version 3).
// Bernd Lörwald <bloerwald+noggit@googlemail.com>
// Stephan Biegel <project.modcraft@googlemail.com>
// Tigurius <bstigurius@googlemail.com>

#ifndef MISC_H
#define MISC_H

#include <string>

// namespace for static helper functions.

namespace misc
{
  void find_and_replace( std::string& source, const std::string& find, const std::string& replace );
  int FtoIround(float d);
  char roundc(float a);
  float frand();
  float randfloat(float lower, float upper);
  int randint(int lower, int upper);
  int getADTCord(float cord);
};


#endif

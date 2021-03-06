// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#pragma once

class MapChunk;
class UIFrame;
class UIMapViewGUI;

namespace noggit
{
  class blp_texture;
}

class UITexturingGUI
{
public:
  static UIFrame* createSelectedTexture();
  static UIFrame* createTexturePalette(int rows, int cols,UIMapViewGUI* setgui);
  static UIFrame* createTilesetLoader();
  static UIFrame* createTextureFilter();
  static UIFrame* createMapChunkWindow();
  static void setSelectedTexture(noggit::blp_texture* t);
  static noggit::blp_texture* getSelectedTexture();
  static void setChunk(MapChunk *chunk);
  static void setChunkWindow(MapChunk *chunk);
  static void updateSelectedTexture();
  static noggit::blp_texture* selectedTexture;
};

// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#pragma once

#include <QWidget>

class QKeyEvent;
class QMouseEvent;
class QListWidgetItem;

class World;

namespace noggit
{
  namespace ui
  {
    class minimap_widget;
  }
}

namespace math
{
  class vector_3d;
}

class Menu : public QWidget
{
Q_OBJECT

public:
  Menu (QWidget* parent = nullptr);
  virtual ~Menu();

private slots:
  void show_map_list_item (QListWidgetItem* item);
  void show_bookmark_list_item (QListWidgetItem* item);
  void open_bookmark_list_item (QListWidgetItem* item);

  void minimap_clicked (const World *world, const ::math::vector_3d&);

signals:
  void create_world_view_request (World*, float, float);

private:
  World *load_map (unsigned int mapID);
  void enter_world_at (World *world, const ::math::vector_3d& pos, bool auto_height = true, float av = -30.0f, float ah = -90.0f);

  noggit::ui::minimap_widget* _minimap;
  //World* _world;
};

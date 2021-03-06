/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 * 
 * This file is part of Omnidome.
 * 
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OMNI_UI_WARP_H_
#define OMNI_UI_WARP_H_

#include <memory>
#include <QGroupBox>

namespace omni
{
  class Session;
  class WarpGrid;
  
  namespace ui
  {
    namespace Ui
    {
      class Warp;
    }

    class Warp : public QGroupBox
    {
      Q_OBJECT
    public:
      Warp(QWidget* = nullptr);
      ~Warp();

      Session const* session() const;
      void setSession(Session* _session);

    public slots:
      void resetWarpGrid();
      void resizeWarpGrid(bool);
      void resizeWarpGrid();
      void updateWarpGrid();

    signals:
      void warpGridChanged();

    private:
      omni::WarpGrid const* warpGrid() const;
      omni::WarpGrid* warpGrid();

      bool locked_ = false;
      Session* session_ = nullptr;
      std::unique_ptr<Ui::Warp> ui_;
    };
  }
}

#endif /* OMNI_UI_WARP_H_ */

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

#include <omni/ui/Export.h>

#include <QFileDialog>
#include <QMessageBox>
#include <omni/Session.h>
#include <omni/util.h>

#include "ui_omni_ui_Export.h"

namespace omni
{
  namespace ui
  {
    Export::Export(QWidget* _parent) :
      QWidget(_parent),
      ui_(new Ui::Export)
    {
      ui_->setupUi(this);
      
      connect(ui_->btnExport,SIGNAL(clicked()),this,SLOT(exportToFile()));
    }
    
    Export::Export(Session* _session, QWidget* _parent) :
      QWidget(_parent),
      ui_(new Ui::Export)
    {
      ui_->setupUi(this);
      connect(ui_->btnExport,SIGNAL(clicked()),this,SLOT(exportToFile()));
      setSession(_session);
    }

    Export::~Export()
    {
    }
      
    Session const* Export::session() const
    {
      return session_;
    }
  
    void Export::setSession(Session* _session)
    {
      session_=_session;
    }

    void Export::exportToFile()
    {
      RenderOptions _options = getRenderOptions();

      QString _filename = QFileDialog::getSaveFileName(this, "Save Calibration Image...", ".", "Calibration Images (*.png)");
      if( _filename.isEmpty() ) return;

      session_->renderToFile(_filename,_options);
      ui_->editExportFilename->setText(_filename);
      QMessageBox::information(this,"Session exported.",QString("Session has been exported to '%1'").arg(_filename));
    }

    omni::RenderOptions Export::getRenderOptions()
    {
      RenderOptions _renderOptions;

      if (ui_->rbSepNone->isChecked())
      {
        _renderOptions.setSeparationMode(SeparationMode::NONE);
      } else
      if (ui_->rbSepScreens->isChecked())
      {
        _renderOptions.setSeparationMode(SeparationMode::SCREENS);
      } else
      if (ui_->rbSepProj->isChecked())
      {
        _renderOptions.
 setSeparationMode(SeparationMode::PROJECTORS);
      }

      if (ui_->rbUVWMap->isChecked())
      {
        _renderOptions.setMappingOutputMode(mapping::OutputMode::UVW);
      } else
      if (ui_->rbTextureCoordinates->isChecked())
      {
        _renderOptions.setMappingOutputMode(mapping::OutputMode::TEXCOORDS);
      } else
      if (ui_->rbMappedInput->isChecked())
      {
        _renderOptions.setMappingOutputMode(mapping::OutputMode::MAPPED_INPUT);
      }
      return _renderOptions;
    }
  }
}

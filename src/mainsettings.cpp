#include "mainsettings.h"

#include <QSettings>


MainSettings::MainSettings()
{    
    m_inifile = new QSettings(MAINSETTINGS_INI_FILE, QSettings::IniFormat);
}


MainSettings::~MainSettings()
{
    delete m_inifile;
}

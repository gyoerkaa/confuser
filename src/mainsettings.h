#ifndef MAINSETTINGS_H
#define MAINSETTINGS_H

#define MAINSETTINGS_INI_FILE "confuser.ini"

class QSettings;

class MainSettings
{
public:
    MainSettings();
    ~MainSettings();

private:
    QSettings* m_inifile;


};

#endif // MAINSETTINGS_H

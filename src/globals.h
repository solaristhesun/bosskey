#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>

#include "softwareversion.h"

class Globals
{
public:
    static const QString ApplicationName;
    static const QString ApplicationFullName;
    static const SoftwareVersion ApplicationVersion;
    static const QString ApplicationWebsite;
    static const quint32 ApplicationRevision;
    static bool bIsPremium;
};

#endif // GLOBALS_H

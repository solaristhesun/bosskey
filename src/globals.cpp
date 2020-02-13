#include "globals.h"

const QString Globals::ApplicationName = "BossKey";
const SoftwareVersion Globals::ApplicationVersion = SoftwareVersion("2020.1a");
const QString Globals::ApplicationFullName = Globals::ApplicationName + " " + Globals::ApplicationVersion.toString();
const QString Globals::ApplicationWebsite = "https://scheler.com/bosskey/";
const quint32 Globals::ApplicationRevision = REVISION;
bool Globals::bIsPremium = false;

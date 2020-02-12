#ifndef ENGINEINTERFACE_H
#define ENGINEINTERFACE_H

#include <QStringList>

class EngineInterface
{
public:
    virtual ~EngineInterface() {}

    virtual void hideWindows(QStringList patterns) =0;
    virtual void showWindows() =0;
    virtual QStringList getWindowList() =0;
};

#endif // ENGINEINTERFACE_H

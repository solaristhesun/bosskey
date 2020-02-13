#include <QRegularExpression>
#include <QDebug>

#include "softwareversion.h"

SoftwareVersion::SoftwareVersion(QString versionString)
{
    QRegularExpression re("^([0-9]+)\\.([0-9]+)([a-z]?)$");
    QRegularExpressionMatch match = re.match(versionString);

    if (match.hasMatch())
    {
        versionMajor_ = match.captured(1).toUShort();
        versionMinor_ = match.captured(2).toUShort();
        versionPatch_ = match.captured(3)[0].toLatin1();
    }
}

bool SoftwareVersion::isValid() const
{
    return versionMajor_ >= 1000 && versionMajor_ <= 9999 &&
           versionMinor_ <= 12 &&
           versionPatch_ >= 'a' && versionPatch_ <= 'z';
}

QString SoftwareVersion::toString() const
{
    return QString("%1.%2%3").arg(versionMajor_, 4).arg(versionMinor_).arg(versionPatch_);
}

quint16 SoftwareVersion::versionMajor() const
{
    return versionMajor_;
}

quint16 SoftwareVersion::versionMinor() const
{
    return versionMinor_;
}

char SoftwareVersion::versionPatch() const
{
    return versionPatch_;
}

bool SoftwareVersion::operator==(const SoftwareVersion& other)
{
    return (versionMajor() == other.versionMajor() &&
            versionMinor() == other.versionMinor() &&
            versionPatch() == other.versionPatch());
}

bool SoftwareVersion::operator!=(const SoftwareVersion& other)
{
    return !(*this == other);
}

bool SoftwareVersion::operator>(const SoftwareVersion& other)
{
    if (versionMajor() > other.versionMajor())
    {
        return true;
    }
    else if (versionMajor() == other.versionMajor())
    {
        if (versionMinor() > other.versionMinor())
        {
            return true;
        }
        else if (versionMinor() == other.versionMinor())
        {
            if (versionPatch() > other.versionPatch())
            {
                return true;
            }
        }
    }

    return false;
}

bool SoftwareVersion::operator<(const SoftwareVersion& other)
{
    return !(*this > other) && ! (*this == other);
}

bool SoftwareVersion::operator<=(const SoftwareVersion& other)
{
    return !(*this > other);
}

bool SoftwareVersion::operator>=(const SoftwareVersion& other)
{
    return !(*this < other);
}

QDebug operator<<(QDebug dbg, const SoftwareVersion& version)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << version.toString();
    return dbg;
}

// EOF <stefan@scheler.com>

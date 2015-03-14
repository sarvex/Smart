#ifndef AT_H
#define AT_H

#include <QString>

class At
{
public:
    static const QString at;
    static const QString dial;
    static const QString toneDial;

    static const QString ok;
    static const QString error;
    static const QString dormant0;
    static const QString dormant1;

    static const QString echoOff;
    static const QString echoOn;

    static const QString enableFlashMessage;
    static const QString enableMessage;
    static const QString flashMessageResponse;
    static const QString messageResponse;

    static const QString programmingSuccessful;
    static const QString programmingUnsuccessful;

    static const QString signalStrength;
    static const QString signalStrengthResponse;

    static const QString systemInformation;
    static const QString systemInformationResponse;

    static const QString modemVersion;
    static const QString modemVersionResponse;
    static const QString modemVersionResponseCompilationDate;
    static const QString modemVersionResponseSoftwareVersion;

    static const QString hardwareVersion;
    static const QString hardwareVersionResponse;

    static const QString sid;
    static const QString sidResponse;

    static const QString nid;
    static const QString nidResponse;

    static const QString sectorId;
    static const QString sectorIdResponse;

    static const QString hdrc;
    static const QString hdrcResponse;

    static const QString cdmaEcio;
    static const QString cdmaEcioResponse;

    static const QString hdrEcio;
    static const QString hdrEcioResponse;

    static const QString meid;
    static const QString meidResponse;

    static const QString cdmaRssi;
    static const QString cdmaRssiResponse;

    static const QString cdmaPnOffset;
    static const QString cdmaPnOffsetResponse;

    static const QString walshCode;
    static const QString walshCodeResponse;

    static const QString channelNumber;
    static const QString channelNumberResponse;

    static const QString bandClass;
    static const QString bandClassResponse;

    static const QString txPower;
    static const QString txPowerResponse;

    static const QString rxPower;
    static const QString rxPowerResponse;

    static const QString bsPrevious;
    static const QString bsPreviousResponse;

    static const QString msPrevious;
    static const QString msPreviousResponse;

    static const QString previousInUse;
    static const QString previousInUseResponse;

    static const QString packetZoneId;
    static const QString packetZoneIdResponse;

    static const QString lastActiveCallState;
    static const QString lastActiveCallStateResponse;

    static const QString hdrRssi;
    static const QString hdrRssiResponse;

    static const QString per;
    static const QString perResponse;

    static const QString atState;
    static const QString atStateResponse;

    static const QString dormantState;
    static const QString dormantResponse;

    static const QString activeStatePnOffset;
    static const QString activeStatePnOffsetResponse;

    static const QString neighborSetPnOffset;
    static const QString neighborSetPnOffsetResponse;

    static const QString neighborSetEcio;
    static const QString neighborSetEcioResponse;

    static const QString esn;
    static const QString esnResponse;

    static const QString prl;
    static const QString prlResponse;

    static const QString defaultMin;
    static const QString defaultMinResponse;

    static const QString setModeZero;
    static const QString setModeOne;
    static const QString setModeTwo;
    static const QString setModeThree;

    static const QString end;
};

#endif // AT_H

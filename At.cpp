#include "At.h"


const QString At::at("AT");
const QString
At::end("\x0D\x0A");
const QString At::dial("AT+CDV");
const QString At::toneDial("AT$VTS=");

const QString
At::error("ERROR");

const QString
At::dormant0("DORMANT:0");

const QString
At::dormant1("DORMANT:1");

const QString At::echoOff("ATE0");
const QString At::echoOn("ATE1");

const QString At::enableFlashMessage("AT+CNMI=1,2,0,1,0");
const QString At::enableMessage("AT$QCCNMI=1,2,2,1,1");

const QString At::programmingSuccessful("Programming Successful");
const QString At::programmingUnsuccessful("Programming Unsuccessful");

const QString At::signalStrength("AT+CSQ");
const QString At::signalStrengthResponse(", 99");

const QString At::systemInformation("AT^SYSINFO");
const QString At::systemInformationResponse("^SYSINFO:");

const QString At::modemVersion("AT$VER");
const QString At::modemVersionResponse("$VER:");
const QString At::modemVersionResponseCompilationDate("rel_time:");
const QString At::modemVersionResponseSoftwareVersion("version:");

const QString At::hardwareVersion("AT^HWVER");
const QString At::hardwareVersionResponse("Part=1280  version=");

const QString At::meid("AT^GSN");
const QString At::meidResponse("0x");

const QString At::sid("AT+CSS");
const QString At::sidResponse("?,");

const QString At::nid("AT$NID?");
const QString At::nidResponse("$NID:");

const QString At::prl("AT$PRL?");
const QString At::prlResponse("$PRL:");

const QString At::sectorId("AT$SECTORID?");
const QString At::sectorIdResponse("$SECTORID:");

const QString At::hdrc("AT$QCHDRC?");
const QString At::hdrcResponse("$QCHDRC:");

const QString At::cdmaEcio("AT$CECIO?");
const QString At::cdmaEcioResponse("$CECIO:");

const QString At::hdrEcio("AT$HAECIO?");
const QString At::hdrEcioResponse("$HAECIO:");


const QString At::cdmaRssi("AT$CRSSI?");
const QString At::cdmaRssiResponse("$CRSSI:");

const QString At::cdmaPnOffset("AT$CPNOFFSET?");
const QString At::cdmaPnOffsetResponse("$CPNOFFSET:");

const QString At::walshCode("AT$WALSH?");
const QString At::walshCodeResponse("$CWALSH:");

const QString At::channelNumber("AT$CCHANNEL?");
const QString At::channelNumberResponse("$CCHANNEL:");

const QString At::bandClass("AT$CBAND?");
const QString At::bandClassResponse("$CBAND:");

const QString At::txPower("AT$HTXPOWER?");
const QString At::txPowerResponse("$HTXPOWER:");

const QString At::rxPower("AT$HRXPOWER?");
const QString At::rxPowerResponse("$HRXPOWER:");

const QString At::bsPrevious("AT$CBSPREV?");
const QString At::bsPreviousResponse("$CBSPREV:");

const QString At::msPrevious("AT$CMSPREV?");
const QString At::msPreviousResponse("$CMSPREV:");

const QString At::previousInUse("AT$CPREVINUSE?");
const QString At::previousInUseResponse("$CPREVINUSE:");

const QString At::packetZoneId("AT$CPZONEID?");
const QString At::packetZoneIdResponse("$CPZONEID:");

const QString At::lastActiveCallState("AT$CALLSTATE?");
const QString At::lastActiveCallStateResponse("$CALLSTATE:");

const QString At::hdrRssi("AT$HRSSI?");
const QString At::hdrRssiResponse("$HRSSI:");

const QString At::per("AT$HPER?");
const QString At::perResponse("$HPER:");

const QString At::atState("AT$HATSTATE?");
const QString At::atStateResponse("$HATSTATE:");

const QString At::dormantState("AT$HDORMSTATE?");
const QString At::dormantResponse("$HDORMSTATE:");

const QString At::activeStatePnOffset("AT$HAPNOFFSET?");
const QString At::activeStatePnOffsetResponse("$HAPNOFFSET:");

const QString At::neighborSetPnOffset("AT$HNPNOFFSET?");
const QString At::neighborSetPnOffsetResponse("$HNPNOFFSET:");

const QString At::neighborSetEcio("AT$HNECIO?");
const QString At::neighborSetEcioResponse("$HNPNECIO:");

const QString At::esn("ATI");
const QString At::esnResponse("ESN:");

const QString At::defaultMin("AT+CIMI");
const QString At::defaultMinResponse("404");

const QString At::flashMessageResponse("$CFT: \"REC UNREAD\",");
const QString At::messageResponse("$QCMT: \"REC UNREAD\",");

const QString At::setModeZero("AT^PREFMODE=8");
const QString At::setModeOne("AT^PREFMODE=2");
const QString At::setModeTwo("AT^PREFMODE=4");
const QString At::setModeThree("AT^PREFMODE=4");

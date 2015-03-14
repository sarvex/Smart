#ifndef DEFINE_H
#define DEFINE_H

#define LOG_FILE_NAME "inter"
#define APPLICATION_ICON ":/logo.png"
#define SPLASH_SCREEN ":/splash.png"
#define SLASH '/'

#define APPLICATION_NAME "SMARTFREN Dialer"
#define SPLASH_SCREEN_TIMEOUT 1500

#define PORT_TIMEOUT 500
#define SIGNAL_STRENGTH_TIMEOUT 5000
#define THREAD_SLEEP_TIMEOUT 200

#define OLIVE_MODEM_NAME "Olive Modem"
#define OLIVE_DEVICE_TYPE "RASDT_Modem"
#define MODEM "USB\\VID_22F4&PID_0026&MI_03"
#define MODEM_REVISION "USB\\VID_22F4&PID_0026&Rev_0000&MI_03"

#define DIALUP_CONNECTION_NAME "Olive Connection"
#define DIALUP_NUMBER "#777"
#define DIALUP_USERNAME "internet"
#define DIALUP_PASSWORD "internet"

#define VENDOR_ID 0x22f4
#define PRODUCT_ID 0x0034
#define INTERFACE_PORT "Olive PCUI"

#define DORMANT0 "^DSDORMANT:0"
#define DORMANT1 "^DSDORMANT:1"
#define ERROR_STRING "ERROR"

#define HOME_PAGE "http://www.smartfren.com/"

#define LINE '\r'
#define COLON ':'
#define COMMA ','
#define EQUAL '='
#define DOT '.'
#define SLASH '/'

#define TIME_FORMAT "hh:mm:ss"
#define DATE_FORMAT "yy/MM/dd"
#define DATETIME_FORMAT "yy/MM/dd hh:mm:ss"
#define INPUT_DATETIME_FORMAT "yy/MM/dd hh:mm:ss"

#define DATABASE_TYPE "QSQLITE"
#define DATABASE_CONNECTION "database"
#define DATABASE_NAME "olive"
#define DATABASE_HOST "OliveHost"
#define DATABASE_USER "OliveUser"
#define DATABASE_PASSWORD "OlivePassword"

#define PRL_LABEL "PRL"
#define MEID_LABEL "MEID"
#define ESN_LABEL "pESN"
#define SOFTWARE_VERSION_LABEL "Software Version"
#define HARDWARE_VERSION_LABEL "Hardware Version"


#define PRL_KEY "PRL"
#define ESN_KEY "ESN"
#define PESN_KEY "MEID"
#define SOFTWARE_VERSION_KEY "SoftwareVersion"
#define HARDWARE_VERSION_KEY "HardwareVersion"

#define OK_STRING "OK"
#define CANCEL_STRING "Cancel"

#define GET_INDEX "SELECT last_insert_rowid() as last_insert"

#define CREATE_TABLE_DATA "CREATE TABLE IF NOT EXISTS data(download DOUBLE, upload DOUBLE, start DATETIME, identifier INTEGER PRIMARY KEY ASC AUTOINCREMENT)"
#define SELECT_DATA "SELECT download, upload, start, identifier FROM data"
#define READ_DATA "SELECT download, upload, start, identifier FROM data WHERE identifier = :identifier"
#define EDIT_DATA "UPDATE data SET download = :download, upload = :upload, start = :start WHERE identifier = :identifier"
#define INSERT_DATA "INSERT INTO data (download, upload, start) VALUES (:download, :upload, :start)"
#define DELETE_DATA "DELETE FROM data WHERE (identifier = :identifier)"
#define DELETE_ALL_DATA "DELETE FROM data"
#define CHECK_DATA "SELECT SUM(download), SUM(upload) FROM data WHERE 1 GROUP BY DATE(start) ORDER BY start DESC Limit 365"

#define DOWNLOAD ":download"
#define UPLOAD ":upload"
#define START ":start"

#endif // DEFINE_H

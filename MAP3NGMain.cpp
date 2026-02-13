/********************************************************************************************
 * File                    : MAP3NGMain.cpp 
 * Purpose                 : Handles SL socket communication, Send and receive message to/from TCAP
 *   
 ***********************************************************************************************/

//#include "Config.h"


#include "Errors.h"
#include "MAP3NGUnitTypes.h"
//#include "Globals.h"
#include "DataTypes.h"
#include "ParlayCallControl_GCCS_Data.h"
#include "ParlayCommonData.h"
#include "parser.h"
#include "lngStruct.h"
#include "DlgPool.h"

#include "Exception.h"
#include "MAP3NGMsgDisp.h"
#include "MAP3NGConfigReader.h"
#include "Logger.h"
#include "CxOsCriticalSection.h"
#include "ServerClient.h"
//#include "MAP3Datatypes.h"
//#include "DlgPool.h"
#include <TCAP_ext.h>
#include <TCAP_errors.h>
#include <TCAP_common.h>

#include <stdexcept>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
//#include <iostream.h>
#include <string>
#include <arpa/inet.h>
#include <signal.h>
#include <queue>

using namespace std;


//Logger * tsgLogger;
Logger g_NgLogger;
Logger g_ReprotLog;
Logger g_CallFailureLog;
NetworkServerConfigData g_NSConfigData;
queue <ServiceLogicRes> g_ResQueue; //commented aru
queue <ServiceLogicRes> g_ReqQueue;
pthread_mutex_t  g_ReqQueueMutex;
pthread_mutex_t  g_ResQueueMutex;  //aru com
//pthread_mutex_t iWaitReqQueueHandle; //aru com

queue <char*> g_SRFResQueue;
pthread_mutex_t  g_SRFResQueueMutex;

//TServiceConfigFile   m_pServiceConfigFile;  //commented by Arathi1
//int shutDownInfinet();

//string g_errLengthResp; 
//struct rlimit lim;

void readValue (char* parent, char* child, char* a_cpReturnValue);
int SetMaximumFiles(long);
int GetMaximumFiles(long *);
extern int gMSISDNCnt;

char OA[1000][50]={0};
char OB[1000][17]={0};
char IM[1000][17]={0};
char MS[1000][17]={0};

int pipefdsInput[2];
int pipefdsOutput[2];

int main()
{
	cout<<"welome to  main=================================="<<endl;
  //SetMaximumFiles(1000);
  try
  {
	
    // Reading the Config File  
    MAP3ConfigReader MAP3ConfigReaderObj;
    //MAP3ConfigReaderObj.init();  
#if 1
 char *m_filepath = new char[80];
  memset(m_filepath, '\0', 80);

 printf("[INFO]: ConfigReader() : Reading Configuration file.\n");
  try
  {
    strcpy(m_filepath,"InputData.xml");
  }
  catch(...)
  {
    cout << "ENVIRONMENT VARIABLE NOT SET";
    return false;
  }
  if(!strcmp(m_filepath,""))
  {
    cout <<"CONFIGURATION FILE NOT FOUND ";
    return false;
  }
  else cout << m_filepath << endl;

  char l_chXMLFileString[1900]={0};
  parser::readXMLFile (m_filepath, l_chXMLFileString);

  char l_chCapRetVal[900]={0};
  parser::getChildString (l_chXMLFileString, "MAP",l_chCapRetVal);

  //Begin: LOGGER CONFIG DATA retrieval
  char l_chTemp[256]={0};
  char l_chLogger[900]={0};
  parser::getChildString(l_chCapRetVal, "LOGGER", l_chLogger);
  memset(l_chTemp, '\0', 100);
  readValue(l_chLogger, "FILEPATH", l_chTemp);
  strcpy(g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_FilePath, l_chTemp);
  //cout << " File Path = " << g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_FilePath<<endl;
  
  memset(l_chTemp, '\0', 100);
  readValue(l_chLogger, "LOGLEVEL", l_chTemp);
  g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_LogLevel = atoi(l_chTemp);
  //cout << " Log Level = " <<g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_LogLevel<<endl;
  
  memset(l_chTemp, '\0', 100);
  //
  memset(l_chTemp, '\0', 100);
  readValue(l_chLogger, "LOGMEDIA", l_chTemp);
  g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_LogMedia  = atoi(l_chTemp);
  //cout << " Log Media  = " <<g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_LogMedia<<endl;
 
  memset(l_chTemp, '\0', 100);
  readValue(l_chLogger, "PREFIX", l_chTemp);
  strcpy(g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_Prefix, l_chTemp);
  //cout << " Prefix  = " <<g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_Prefix<<endl;

  memset(l_chTemp, '\0', 100);
  readValue(l_chLogger, "MAXFILESIZE", l_chTemp);
  g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_MaxFileSize = atoi(l_chTemp);
  //cout << " MaxFile size = " <<g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_MaxFileSize<<endl;
 
  memset(l_chTemp, '\0', 100);
  readValue(l_chLogger, "ARCHIVALPATH", l_chTemp);
  strcpy(g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_ArchivalDirPath ,l_chTemp);
  //cout << "Archival Dir Path = " <<g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_ArchivalDirPath<<endl;
  //End: Finished Retrieval of Logger details.

   //Begin: For Release Cause
   char l_chReleaseCauseCodes[900]={0};
   parser::getChildString(l_chXMLFileString, "RELEASECAUSE", l_chReleaseCauseCodes);
   memset(l_chTemp, '\0', 100);
   readValue(l_chReleaseCauseCodes, "SWITCHTYPE", l_chTemp);
   strcpy(g_NSConfigData.i_releaseCauseCodes.s_switchType, l_chTemp);

   char l_chSwithType[100]={0};
   parser::getChildString(l_chReleaseCauseCodes, g_NSConfigData.i_releaseCauseCodes.s_switchType, l_chSwithType);
   //cout << "SwitchType = " << g_NSConfigData.i_releaseCauseCodes.s_switchType << endl;

   char l_chNormalEnd[100]={0};
   readValue(l_chReleaseCauseCodes, "NORMALEND", l_chNormalEnd);
   g_NSConfigData.i_releaseCauseCodes.i_normalEnd = strtol(l_chNormalEnd, NULL, 16);
   char s_abnormal[20]={0};
   readValue (l_chReleaseCauseCodes, "ABNORMALEND",s_abnormal);
   g_NSConfigData.i_releaseCauseCodes.i_abnormalEnd = strtol((char *)s_abnormal,NULL,16);;
   //printf(" ABNormal End = %x \n",g_NSConfigData.i_releaseCauseCodes.i_abnormalEnd);
   //
   //  /*********************************************************************/
    char serviceKey[3] = {0};
    char locationNumber[30] = {0};
    char originalCalledPartyId[30] = {0};
    char OrigCallPtyId[20] = {0};
    char IMSI[30] = {0};
    char imSi[20] = {0};
    char locNum[20] = {0};
    char FlgOrig[2] = {0};
    char Flg[2] = {0};
    char initialDP[900]={0};

     parser::getChildString (l_chXMLFileString, "INTIALDP",initialDP);
     readValue(initialDP, "SERVICE_KEY", serviceKey);
  
    cout << "@@@@@@@@@@@@@@@@@@@@@@@ SERVICE KEY " << atoi(serviceKey) << endl;
 
    parser::getChildString (initialDP, "LOCATION_NUMBER",locationNumber);
  
    readValue(locationNumber, "FLAG", Flg);
  
    if(atoi(Flg) != 0)
    {
       cout << "Location Information is Included in the InitialDP" << endl;

       readValue(locationNumber, "VALUE", locNum);
       cout << "##### Location number : " << locNum << endl;
    }
    else
    {
       cout << "Location Information is not Included in the InitialDP" << endl;
    }
 
                     memset(initialDP,0,900);
                                                                  parser::getChildString (l_chXMLFileString, "INTIALDP",initialDP);
                                                                    parser::getChildString (initialDP, "ORIGINAL_CALLEDPARTY_ID",originalCalledPartyId);
  

readValue(originalCalledPartyId, "FLAG", FlgOrig);

  if(atoi(FlgOrig) != 0)
  {
    cout << "OriginalCalledPartyId is Included in the InitialDP" << endl;

    readValue(originalCalledPartyId, "VALUE", OrigCallPtyId);
    cout << "##### OrigicalCalledPartyID : " << OrigCallPtyId << endl;
  }
  else
  {
    cout << "OriginalCalledPartyId is not Included in the InitialDP " << atoi(FlgOrig) << endl;
  }


  parser::getChildString (initialDP, "IMSI",IMSI);

  memset(FlgOrig,0,2);
  readValue(IMSI, "FLAG", FlgOrig);

  if(atoi(FlgOrig) != 0)
  {
    cout << "IMSI is Included in the InitialDP" << endl;

    readValue(IMSI, "VALUE", imSi);
    cout << "##### IMSI : " << imSi << endl;
  }
  else
  {
    cout << "IMSI is not Included in the InitialDP " << atoi(FlgOrig) << endl;
  }

  /*********************************************************************/
  m_filepath = NULL;

#endif 
    // Initializing Logger
    cout<<"initialize logger started========================"<<endl;
    g_NgLogger.initialize(g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_LogLevel,
        g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_LogMedia,
        (char*)g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_FilePath,
        (char*)g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_Prefix,
        (char*)g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_ArchivalDirPath,
        g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_MaxFileSize,0) ;
    cout<<"initialize  RoRING tarted========================"<<endl;



    cout<<" INITIALIZATION OF LOGGER SUCCESS"<<endl;
    pthread_mutex_init(&g_ReqQueueMutex , NULL);
    //pthread_mutex_init(&g_SRFResQueueMutex , NULL);

    cout<<"Reading Exported Parameter .."<<endl;

    initTransList();
    g_NgLogger.log (LOG_LEVEL_DEBUG, (char *)"Successfully Read the Exported Parameters");
    cout<<"Successfully read the Exported Parameter .."<<endl;


    
    
    signal(SIGUSR1, SigUserHandle); // To handle kill -10
    signal(SIGINT, SigUserHandle);
    signal(SIGTSTP, SIG_IGN);
    int returnstatus;
    returnstatus = pipe(pipefdsInput);

    if (returnstatus == -1) {
      g_NgLogger.log(LOG_LEVEL_CRITICAL, "%s<Line: %d> Unable to create input pipe. Exit", __FUNCTION__, __LINE__);
      exit;
    }
	
    returnstatus = pipe(pipefdsOutput);
	

    if (returnstatus == -1) {
      g_NgLogger.log(LOG_LEVEL_CRITICAL, "%s<Line: %d> Unable to create output pipe. Exit", __FUNCTION__, __LINE__);
      exit;
    }
    pthread_t thr;
    pthread_create(&thr,NULL, handleRestViaServerClient,NULL);
    
	//char reply[60]; // 
	
	
    doServer(); 
  }
  catch(exception &e)
  {
    g_NgLogger.log(LOG_LEVEL_CRITICAL, "%s<Line: %d> Exception occured %s", __FUNCTION__, __LINE__,(char *)e.what());
    cout<<"Exception occured .."<<endl;
  }

  return 0;

}
void readValue (char* parent, char* child, char* a_cpReturnValue)
{
  char v_chChildString[500]={0};
  char szChildString[500]={0};
  parser::getChildString(parent, child,szChildString);
  strcpy(v_chChildString, szChildString);
  char v_chValue[100]={0};
  char szChValue[100]={0};
  parser::getValue(v_chChildString, child,szChValue);
  strcpy(v_chValue, szChValue);
  strcpy(a_cpReturnValue, v_chValue);
  return ;
}


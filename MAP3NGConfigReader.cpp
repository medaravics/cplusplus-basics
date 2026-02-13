#include "MAP3NGConfigReader.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include<stdlib.h>
//#include "parser.h"
extern NetworkServerConfigData g_NSConfigData;

MAP3ConfigReader::MAP3ConfigReader()
{
}

MAP3ConfigReader::~MAP3ConfigReader()
{
}

/*string MAP3ConfigReader::readValue (string parent, string child)
  {
  string n = parser::getChildString (parent, child);
  string v = parser::getValue (n, child);
  return v;
  }
  */
void MAP3ConfigReader::readValue (char* parent, char* child, char* a_cpReturnValue)
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


Boolean MAP3ConfigReader::init()
{
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
  //cout << " File Path = " << g_NSConfigData.i_CAP3ConfigData.LoggerCD.i_FilePath<<endl;

  memset(l_chTemp, '\0', 100);
  readValue(l_chLogger, "LOGLEVEL", l_chTemp);
  g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_LogLevel = atoi(l_chTemp);
  //cout << " Log Level = " <<g_NSConfigData.i_CAP3ConfigData.LoggerCD.i_LogLevel<<endl;

  memset(l_chTemp, '\0', 100);
  readValue(l_chLogger, "LOGMEDIA", l_chTemp);
  g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_LogMedia  = atoi(l_chTemp);
  //cout << " Log Media  = " <<g_NSConfigData.i_CAP3ConfigData.LoggerCD.i_LogMedia<<endl;

  memset(l_chTemp, '\0', 100);
  readValue(l_chLogger, "PREFIX", l_chTemp);
  strcpy(g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_Prefix, l_chTemp);
  //cout << " Prefix  = " <<g_NSConfigData.i_CAP3ConfigData.LoggerCD.i_Prefix<<endl;

  memset(l_chTemp, '\0', 100);
  readValue(l_chLogger, "MAXFILESIZE", l_chTemp);
  g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_MaxFileSize = atoi(l_chTemp);
  //cout << " MaxFile size = " <<g_NSConfigData.i_CAP3ConfigData.LoggerCD.i_MaxFileSize<<endl;

  memset(l_chTemp, '\0', 100);
  readValue(l_chLogger, "ARCHIVALPATH", l_chTemp);
  strcpy(g_NSConfigData.i_MAP3ConfigData.LoggerCD.i_ArchivalDirPath ,l_chTemp);
  //cout << "Archival Dir Path = " <<g_NSConfigData.i_CAP3ConfigData.LoggerCD.i_ArchivalDirPath<<endl;
  //End: Finished Retrieval of Logger details.


  delete m_filepath;
  m_filepath = NULL;
  return true;
}

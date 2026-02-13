#define SQLCA_STORAGE_CLASS extern
#include <string.h>
#include <iostream>
#include <list>
#include "UpdateDb.h"
#include "DataFromDb.h"
#include "ActiveLogger.h" 
#include "ESqlRes.h" 
#include "BCDEncoder.h"
#include "DbConnHandler.h"
//#include "SmsDbFunction.h"
#include "SmsmDbService.h"
#include "Subscriber.h"
#include "DbUtil.h" //xrp-2049
using namespace std;

ESqlRes SmsmDbService::isMsisdnFoundInMwd(void *dbConnObj,nNumber msisdn ) 
{
  ESqlRes result;
  std::string query;
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Entering|%s",__FUNCTION__);
  if( msisdn.getOriginalLength() <= 0 || msisdn.getOriginalLength() > MSISDN_SIZE / 2)
  {
    loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0,"Msisdn Check failed  |%s",__FUNCTION__);
    loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Leaving isMsisdnFoundInMwd  |%s",__FUNCTION__); 
    return CODE_EXCEPTION;	
  }
  string decodedMsisdn = decodePipcMessage(msisdn);
  try
  {
    //int recordCount = DbUtil::getRowCount("SELECT Count(1) FROM HLR_TR_MWD_INFO WHERE MSISDN = '12345' AND STATUS = 'N'"); 
    query = "SELECT Count(1) FROM HLR_TR_MWD_INFO "
      "WHERE MSISDN = '" + decodedMsisdn + "' AND STATUS = 'N'";
    int recordCount = DbUtil::getAValue(query);
    if(recordCount>0) 
    {
      loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " MSISDN found[%s]| %s",decodedMsisdn,__FUNCTION__); 
      return CODE_SQL_SUCCESS;
    }
    loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"MSISDN not found[%s]|%s",decodedMsisdn, __FUNCTION__);
    result = ROWS_NOT_FOUND;
  }
  catch (const std::runtime_error& ex)
  {
    loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0,	"Runtime error:  [%s]-query[%s]|%s", ex.what(), query,__FUNCTION__);
    result =CODE_EXCEPTION;
  } 
  catch (const std::exception& ex)
  {
    loggerObj.log(LM_CRITICAL,__FILE__,__LINE__,0,0, "General exception: %s-query[%s]|%s", ex.what(), query, __FUNCTION__);
    result = CODE_EXCEPTION;
  }
  catch(...)
  {
    loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Default Exception-query[%s]|%s",query,__FUNCTION__);
    result =CODE_EXCEPTION;
  }
  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Leaving|%s", __FUNCTION__);    
  return result;
}
ESqlRes SmsmDbService::isScAddressFoundInMwd(void* dbConnObj, nNumber scAddress, nNumber msisdn)
{
  ESqlRes result;
  std::string query;

  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"Entering isScAddressFoundInMwd |%s", __FUNCTION__);

  int scaddr_len = scAddress.getOriginalLength();
  if (scaddr_len <= 0 || scaddr_len > SCADDRESS_SIZE / 2)
  {
    loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0,"SCAddress length is inconsistent |%s", __FUNCTION__);
    return CODE_EXCEPTION;
  }

  int msisdn_len = msisdn.getOriginalLength();
  if (msisdn_len <= 0 || msisdn_len > MSISDN_SIZE / 2)
  {
    loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0,"MSISDN length is inconsistent |%s", __FUNCTION__);
    return CODE_EXCEPTION;
  }

  string decodedMsisdn = decodePipcMessage(msisdn);
  string decodedScAddr = decodePipcMessage(scAddress);

  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"Decoded MSISDN[%s], SCAddress[%s] |%s",decodedMsisdn.c_str(), decodedScAddr.c_str(), __FUNCTION__);
  try
  {
    query = "SELECT COUNT(1) FROM HLR_TR_MWD_INFO "
    "WHERE MSISDN = '" + decodedMsisdn +
    "' AND SVC_CENTRE_ADDRESS = '" + decodedScAddr +
    "' AND STATUS = 'N'";
    int recordCount = DbUtil::getAValue(query);
    if (recordCount > 0)
    {
      loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"SCAddress[%s] found for MSISDN[%s] | Count=%d |%s",
      decodedScAddr.c_str(), decodedMsisdn.c_str(),recordCount, __FUNCTION__);
      return CODE_SQL_SUCCESS;
    }
    loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"SCAddress[%s] NOT found for MSISDN[%s] |%s",decodedScAddr.c_str(), decodedMsisdn.c_str(), __FUNCTION__);
    result = ROWS_NOT_FOUND;
  }
  catch (const std::runtime_error &ex)
  {
    loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0,"Runtime error: [%s] Query[%s] |%s",ex.what(), query.c_str(), __FUNCTION__);
    result = CODE_EXCEPTION;
  }
  catch (const std::exception &ex)
  {
    loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0,"General exception: [%s] Query[%s] |%s",
    ex.what(), query.c_str(), __FUNCTION__);
    result = CODE_EXCEPTION;
  }
  catch (...)
  {
    loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0,"Default Exception Query[%s] |%s",query.c_str(), __FUNCTION__);
    result = CODE_EXCEPTION;
  }
  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"Leaving |%s", __FUNCTION__);
  return result;
}
ESqlRes SmsmDbService::getScAdresses(void* dbConnObj, nNumber& msisdn, list<nNumber>& scad) 
{

  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "Entering |%s",__FUNCTION__);
  ESqlRes result=CODE_SQL_SUCCESS;
  std::string query;
  int msisdn_len = msisdn.getOriginalLength();
  if( msisdn_len <= 0 ||  msisdn_len > MSISDN_SIZE / 2)
  {
     loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0, " Msisdn Check failed | %s |",__FUNCTION__  );
	 loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving getScAdresses |%s",__FUNCTION__);
	 return CODE_EXCEPTION;	
  }

  string decodedMsisdn = decodePipcMessage(msisdn);
  
  char scd_temp[ SCADDRESS_SIZE + 1 ] ;
  int scd_len = -1;
  unsigned short int l_scAddress_c_len;
  int count=0;
  int affectedRows = 0;
  Subscriber subscriber;
  try
  { 
   query= "SELECT COALESCE(SVC_CENTRE_ADDRESS, '0') FROM HLR_TR_MWD_INFO WHERE MSISDN = $1 AND STATUS = 'N';";
   std::vector<std::vector<std::string>> results;
   std::vector<std::string> params = { decodedMsisdn };
   DbUtil::executePrepared("scAddresses", query, params, results, affectedRows);
	 for (size_t i = 0; i < results.size(); ++i) 
     {
	    string l_scAddress_c =  results[i][0]; // first column
		if (stoi(l_scAddress_c) != 0) 
        {
		  nNumber scaddr(30);
		  subscriber.encode1(&l_scAddress_c, &scaddr);
		  scad.push_back(scaddr);
		  count++;  
		}
	 }
   if (count>0){ 
      loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "No. of scaddress[%d]  | %s",count, __FUNCTION__); 
      result= CODE_SQL_SUCCESS;
	 }
   else if(count==0){
			loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "scaddress is not obtained|%s",__FUNCTION__);
			result= ROWS_NOT_FOUND;
	 }else {
      loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "Code Should never reach here|%s",__FUNCTION__);
		  result= CODE_EXCEPTION;
	 }
  }
  catch (const std::runtime_error& ex) {
     loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0,	"Runtime error:  [%s]-query[%s]|%s", ex.what(), query,__FUNCTION__);
	 result =CODE_EXCEPTION;
  } 
  catch (const std::exception& ex) {
     loggerObj.log(LM_CRITICAL,__FILE__,__LINE__,0,0, "General exception: %s-query[%s]|%s", ex.what(), query, __FUNCTION__);
     result =CODE_EXCEPTION;
  }
  catch(...){
     loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Default Exception-query[%s]|%s",query,__FUNCTION__);
     result =CODE_EXCEPTION;
  }
  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Leaving|%s", __FUNCTION__);    
  return result; 
 }

ESqlRes SmsmDbService::getDistinctMsisdnInMwd(void* dbConnObj, list<nNumber> &msdn)//xrp-2049
{
   loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"Entering|%s", __FUNCTION__);
   ESqlRes result=CODE_SQL_SUCCESS;
   Subscriber subscriber;
   try
   { 
     
      std::string query = "SELECT DISTINCT MSISDN FROM HLR_TR_MWD_INFO WHERE STATUS = 'N';"; 
      DbUtil::ResultSet results = DbUtil::executeQuery(query);

      if (results.empty())
      {
        loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"ROWS NOT Found in HLR_TR_MWD_INFO | %s", __FUNCTION__);
		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Leaving|%s", __FUNCTION__);
        return ROWS_NOT_FOUND;
      }
	  int count=0;
	  for (const auto& row : results) {
		  string msisdn = row.at("msisdn"); // throws if key not found  MSISDN
		  if(std::stoi(msisdn) != 0) {
		    nNumber msisdn_number(30);
		    subscriber.encode1(&msisdn, &msisdn_number);
		    msdn.push_back(msisdn_number);
		    count++;
		  }   
    }
	  
	  if (count > 0) {   
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "List of msisdn found[%d] | %s", count, __FUNCTION__);
	 }
   } 
   catch (const std::exception& ex)
   {
     loggerObj.log(LM_CRITICAL,__FILE__,__LINE__,0,0, "%s|%s", ex.what() , __FUNCTION__);
     result =CODE_EXCEPTION;
   }
   loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Leaving |%s",__FUNCTION__);
   return result;
}

/*
*  Inputs to the function:
*   Inputs 
*    Following are read from first argument DataFromDb  
*    1. getSmscNumber() 
*	 2. getRoamingStatus() 
*    3. getMsisdn()
*    4. scAddress - comes in second Argument
*   All the inputs are of type/class Number and data inside that will be encoded format. 
*    	
*   routeSmsToHplmnSmsc return success 
*    if 1. SMS_FILTER is ENABLED, 
*       2. roamingStatus is true, 
*       3. NON_HPLMN_MT_SMS_LI_REQUIRED (Intercept) is enabled
*       4. CaseID is a non-zero positive number (from HLR_TT_LAWFULL_INTERCPT_INFO)
*       5. SMS Originated SMSC Number(received from object of first argument) 
*             and HPLMN SMSC number received in second argument.
*          
*
*/
ESqlRes SmsmDbService::routeSmsToHplmnSmsc(DataFromDb *obj,nNumber&  scAddress)
{
  string smsFilterEnabledQuery = "SELECT CAST(Value AS INTEGER) FROM HLR_MT_SYS_PARAMS where parameter='SMS_FILTER_ENABLED_IN_SMSC';";
  string interceptEnableParamQuery = "SELECT CAST(Value AS INTEGER) FROM HLR_MT_SYS_PARAMS where parameter='NON_HPLMN_MT_SMS_LI_REQUIRED';";
 
  ESqlRes result;
  BCDEncoder BCDEncoderobj;
  try{
    string msisdn= decodePipcMessage(obj->getMsisdn()); // "524343425432";  //decodePipcMessage(obj->getMsisdn());
    char l_msisdn_c[MSISDN_SIZE+1] = {'\0'};
    int interceptParameter = DbUtil::getAValue(interceptEnableParamQuery);
 
    if( (obj->getSmscNumber().getOriginalLength() > 1) 
     //Compare SmscNumber and scAddress in BCD format only - This is critical to testing. 
     && memcmp(scAddress.getValue(), obj->getSmscNumber().getValue(), obj->getSmscNumber().getOriginalLength())
     && (DbUtil::getAValue(smsFilterEnabledQuery)==1) //isSMSFilterEnabled 
     && ((interceptParameter==2) && !(((obj->getRoamingStatus().getValue())[0])=='0')) //if intercept is then roaming status cannot be 0.
     && (interceptCaseId(msisdn) > 0) ) {
      loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "Return Sucess|%s",__FUNCTION__);
      return CODE_SQL_SUCCESS;
    }
	
	//log problem if any
    loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "Originated SMSC Number & HPLMN numbers are same. Compare Result[%d]|%s",
		    memcmp(scAddress.getValue(), obj->getSmscNumber().getValue(), obj->getSmscNumber().getOriginalLength()),
		    __FUNCTION__);
    loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "Received Roaming Status [%d]-0 means intercept Not required|%s", 
		    obj->getRoamingStatus().getValue(), __FUNCTION__);
	result = ROWS_NOT_FOUND;
  }    
  catch (const std::exception& ex) {
    loggerObj.log(LM_CRITICAL,__FILE__,__LINE__,0,0, "%s|%s", ex.what() , __FUNCTION__);
    result = CODE_EXCEPTION;
  }
  catch(...)
  {
    loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Leaving |%s",__FUNCTION__);
    result = CODE_EXCEPTION;
  }
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Leaving |%s",__FUNCTION__);
  return result;
}

int SmsmDbService::interceptCaseId(string msisdn) {
  int caseId=0;								
  std::vector<std::vector<std::string>> results;
  std::vector<std::string> params = { msisdn };
  int affectedRows;
  DbUtil::executePrepared(
    "count_case_id",
    interceptCaseIdQuery,
    params,
    results,
    affectedRows
  );

  if (!results.empty()) {
     caseId = atoi(results[0][0].c_str());
  }
  return caseId;

}

/*
* decodePipcMessage take object as input_iterator
* convert BCD to string, returns string value
*/
string SmsmDbService::decodePipcMessage(nNumber object) {
	char pipcmessage_encoded[512]={'\0'};
	char pipcmessage_decoded[512]={'\0'};
	BCDEncoder BCDEncoderobj;
	//Why 512, Just a random number, and valid if data being fits into variable size declared

	if(object.getOriginalLength() > 512) 
		throw std::runtime_error("Unexpected Length of Data-This error should not occur ");
	
	memcpy(pipcmessage_encoded,  object.getValue(), object.getOriginalLength());
	int  decode_string_len = BCDEncoderobj.BCDToChar(pipcmessage_decoded, &pipcmessage_encoded[0], object.getOriginalLength());
	pipcmessage_decoded[decode_string_len] = '\0';
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "Decoded Message[%s], Len[%d]|%s", pipcmessage_decoded,decode_string_len,__FUNCTION__);
  return string(pipcmessage_decoded);
}

ESqlRes SmsmDbService::updateSmsInfo(UpdateDb *obj)
{
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Entering|%s",__FUNCTION__);
  if (obj == NULL) 
  {   
    loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0,"obj is null  |%s",__FUNCTION__);
    loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0,"Leaving updateSmsInfo |%s",__FUNCTION__);
    return CODE_EXCEPTION;
  }

  int imsi_len =  obj->getImsi().getOriginalLength();
  int msisdn_len =  obj->getMsisdn().getOriginalLength();

  if ( imsi_len <= 0 && msisdn_len <=0 || imsi_len > IMSI_SIZE / 2 || msisdn_len > MSISDN_SIZE / 2 )
  {
    loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0,"IMSI[Len:%d]  and MSISDN[Len:%d] check failed|%s",
		imsi_len,msisdn_len,__FUNCTION__);
    loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Leaving updateSmsInfo|%s",__FUNCTION__); 
    return CODE_EXCEPTION;
  }
  try{
    Subscriber subscriber;

    //Collect Encode "obj" values into subscriber in Number Format
    subscriber.readObject(obj);

    // This is Critical method. Without decoding the fields you cannot process further
    subscriber.decode(); 

    if (obj->getHlrTrSubscriberInfoUpdated()) {
      subscriber.updateSubscriberGsmInfo();
    }

    //update GprsInfo only if Record exists in HLR_TR_SUB_GPRS_INFO tables for given MSISDN or IMSI
    if ( DbUtil::getRowCount(subscriber.getTrGprsQueryForSuperCharger()) > 0 &&
		    obj->getHlrTrGprsProfileUpdated())
    {
      subscriber.updateSubscriberGprsInfo();
    }
  } 
  catch (const std::exception& ex) {
	loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0,	"Exception[%s]|%s", ex.what(), __FUNCTION__); 
	return PROC_CODE_ERROR;
  }
  catch(...)
  {

	loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Caught Exception  |%s", __FUNCTION__);
	loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Leaving updateSmsInfo  |%s", __FUNCTION__);    
	return CODE_EXCEPTION;
  }
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Leaving |%s",__FUNCTION__);
  return CODE_SQL_SUCCESS;
}

ESqlRes SmsmDbService::updateMwdList(void* a,nNumber &msisdn, nNumber &scAddress, nNumber &status)
{
  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Entering|%s", __FUNCTION__);
  int msisdn_len = msisdn.getOriginalLength();
  if( msisdn_len <= 0 || msisdn_len > MSISDN_SIZE / 2)
  {
	loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "MSISDN length is inconsistent   |%s", __FUNCTION__);
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Leaving |%s", __FUNCTION__); 
	return CODE_EXCEPTION;	
  }
  int scaddr_len = scAddress.getOriginalLength();
  if( scaddr_len <= 0 || scaddr_len > SCADDRESS_SIZE / 2)
  {
	loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "SCAddress check failed  |%s", __FUNCTION__);
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Leaving |%s", __FUNCTION__);
	return CODE_EXCEPTION;	
  }
  string msisdnString = decodePipcMessage(msisdn);
  string scAddressString = decodePipcMessage(scAddress);
  int affectedRows;
  char	l_status_c[STATUS_SIZE +1]={0};
  memcpy(l_status_c ,status.getValue(), status.getOriginalLength());
  int l_status_c_len = status.getOriginalLength();
  l_status_c[l_status_c_len] = '\0';

  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "MSISDN[%s],SCAddress[%s],Status[%s]|%s",
		  msisdnString.c_str(), scAddressString.c_str(),l_status_c,__FUNCTION__);
  int   l_count=0;
  std::vector<std::string> params = { msisdnString, scAddressString,l_status_c};
  std::vector<std::string> params2 = { msisdnString, scAddressString};
  try
  {
    std::vector<std::vector<std::string>> results;
    DbUtil::executePrepared( "mwdQuery", mwdQuery, params2, results, affectedRows);
    if (!results.empty()) {
      l_count = atoi(results[0][0].c_str());
    }
    results.clear();
    //Insert into table if Record does not exist. Otherwise, update Status
    if (l_count <= 0) {
      results.clear();
      DbUtil::executePrepared( "mwdInsertQuery", mwdInsertQuery, params, results, affectedRows);
    }
    else {
    results.clear(); 
    DbUtil::executePrepared( "mwdUpdateQuery", mwdUpdateQuery, params, results, affectedRows);
    } 
      
    loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Affected Rows[%d] | %s",affectedRows, __FUNCTION__);
    loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Leaving updateMwdList | %s", __FUNCTION__); 
    return CODE_SQL_SUCCESS;
  }
  catch (const std::runtime_error& ex) {
   loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0,"Runtime error:  [%s]|%s", ex.what(), __FUNCTION__);
   return CODE_EXCEPTION;
  } 
  catch (const std::exception& ex) {
   loggerObj.log(LM_CRITICAL,__FILE__,__LINE__,0,0, "General exception: %s|%s", ex.what(), __FUNCTION__);
   return CODE_EXCEPTION;
  }
  catch(...){
   loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Default Exception|%s", __FUNCTION__);
   return CODE_EXCEPTION;
  }
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Leaving |%s",__FUNCTION__);
}

ESqlRes SmsmDbService::getSmsInfo(DataFromDb *obj, int trGprsTeledeleteFlag)
{
  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Entering|%s", __FUNCTION__);
  int imsi_len =  obj->getImsi().getOriginalLength();
	cout <<"Imsi length : "<<imsi_len<<endl;
  int msisdn_len =  obj->getMsisdn().getOriginalLength();
	cout <<"Msisdn length : "<<msisdn_len<<endl;

	
  if ( imsi_len <= 0 && msisdn_len <=0 || imsi_len > IMSI_SIZE / 2 || msisdn_len > MSISDN_SIZE / 2 )
  {
    loggerObj.log(LM_CRITICAL,__FILE__,__LINE__,0,0,"IMSI[Len:%d]  and MSISDN[Len:%d] check failed|%s",imsi_len,msisdn_len,__FUNCTION__);
    loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Leaving |%s",__FUNCTION__); 
    return CODE_EXCEPTION;
  }

  int count=0;
  Subscriber subscriber(obj);
  try
  {
    //subscriber.imsi = decodePipcMessage(obj->getImsi());
    //subscriber.msisdn = decAodePipcMessage(obj->getMsisdn());
    subscriber.decodeMsisdn();
    subscriber.loadSubscriberFromDatabase();

    loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"DEBUG Subscriber Value [%s]|%s", subscriber.toString().c_str(), __FUNCTION__);

    subscriber.toDataFromDb(obj);
  }
  catch (const std::runtime_error& ex) {
   loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0,"Runtime error:  [%s]|%s", ex.what(), __FUNCTION__);
   return CODE_EXCEPTION;
  }
  catch (const std::exception& ex) {
   loggerObj.log(LM_CRITICAL,__FILE__,__LINE__,0,0, "General exception: %s|%s", ex.what(), __FUNCTION__);
   return CODE_EXCEPTION;
  }
  catch(...){
   loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Default Exception|%s", __FUNCTION__);
   return CODE_EXCEPTION;
  }
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Leaving |%s",__FUNCTION__);
  return CODE_SQL_SUCCESS;
}
 

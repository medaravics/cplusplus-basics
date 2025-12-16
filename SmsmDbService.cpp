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
#include "DbUtil.h" //xrp-2049
using namespace std;

//=========================================================================================================================================
/*ESqlRes SmsmDbService::isMsisdnFoundInMwd(void *dbConnObj,nNumber msisdn ) 
  {
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Entering isMsisdnFoundInMwd |%s",__FUNCTION__);
  DBConnHandler* pDBHandler = DBConnHandler::GetInstance();
  Connection*    conn;
  BCDEncoder BCDEncoderobj;
  Statement* stmt;
  ResultSet* rs;
  if (pDBHandler == NULL )
  {
  loggerObj.log(LM_CRITICAL,__FILE__,__LINE__,0,0,"DB connection is null  |%s",__FUNCTION__);
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Leaving isMsisdnFoundInMwd   |%s",__FUNCTION__);
  return CODE_EXCEPTION;
  }

  char msisdn_temp[MSISDN_SIZE +1];
  memset (msisdn_temp,'\0',sizeof(msisdn_temp));
  int msisdn_len = msisdn.getOriginalLength();

  if( msisdn_len <= 0 || msisdn_len > MSISDN_SIZE / 2)
  {
  loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0,"Msisdn Check failed  |%s",__FUNCTION__);
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Leaving isMsisdnFoundInMwd  |%s",__FUNCTION__); 
  return CODE_EXCEPTION;	
  }
  memcpy(msisdn_temp,  msisdn.getValue(), msisdn_len);
  msisdn_temp[msisdn_len] = '\0';
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"MSISDN Len =  |%d |%s",msisdn_len,__FUNCTION__);

  char	l_msisdn_c[MSISDN_SIZE +1]={0};
  int		l_count_i;
  memset( &l_msisdn_c,'\0',sizeof(l_msisdn_c) );
  l_count_i = 0;
  try
  {

  int l_msisdn_c_len = BCDEncoderobj.BCDToChar(l_msisdn_c ,msisdn_temp, msisdn_len);
  l_msisdn_c[l_msisdn_c_len] = '\0';
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "Char MSISDN = %s Len = %d |%s",l_msisdn_c, l_msisdn_c_len,__FUNCTION__);

  stmt = pDBHandler->GetStatement((conn=pDBHandler->GetConnection()),
  "SELECT COUNT(1) FROM HLR_TR_MWD_INFO WHERE MSISDN = :1 AND STATUS = 'N'");

  stmt->setString(1, l_msisdn_c);
  rs = stmt->executeQuery();
  if (rs->next()) {
  l_count_i = rs->getInt(1);
  }
  if(l_count_i)
  {
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Given MSISDN is  found | %s",__FUNCTION__); 
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving isMsisdnFoundInMwd |%s",__FUNCTION__);
  pDBHandler->Close(rs, stmt, conn);
  return CODE_SQL_SUCCESS;
  }
  else
  {
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Given MSISDN is not found in DataBase  | %s",__FUNCTION__); 
  pDBHandler->Close(rs, stmt, conn);
  goto DATA_NOT_FOUND;
  }
  }
  catch (SQLException& ex) {
  std::cerr << "SQL Error: " << ex.getMessage() << std::endl;
  loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Oracle Error code and Message |%d|%s|%s", ex.getErrorCode(), ex.getMessage().c_str(), __FUNCTION__); 
  pDBHandler->CloseStatement(stmt, conn);
  loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Oracle SQL_ERROR_OCCURED  |%s", __FUNCTION__);
  return PROC_CODE_ERROR;

  }
  catch (...)
{
	loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0," Caught an Exception |%s",__FUNCTION__);
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving isMsisdnFoundInMwd |%s",__FUNCTION__);
	pDBHandler->Close(rs, stmt, conn);
	return CODE_EXCEPTION;
}

DATA_NOT_FOUND:  
{	
	loggerObj.log(LM_INFO,__FILE__,__LINE__,0,0, " DATA_NOT_FOUND |%s",__FUNCTION__);
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving isMsisdnFoundInMwd |%s",__FUNCTION__);
	return ROWS_NOT_FOUND;
}
}*/

/*ESqlRes SmsmDbService::isMsisdnFoundInMwd(void *dbConnObj, nNumber msisdn) //XRP-2049
{
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, 
			"Entering isMsisdnFoundInMwd |%s", __FUNCTION__);

	try
	{
		BCDEncoder BCDEncoderobj;
		char msisdn_temp[MSISDN_SIZE + 1];
		memset(msisdn_temp, '\0', sizeof(msisdn_temp));

		int msisdn_len = msisdn.getOriginalLength();

		if(msisdn_len <= 0 || msisdn_len > MSISDN_SIZE / 2)
		{
			loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0,
					"Msisdn Check failed | MSISDN length: %d |%s", msisdn_len, __FUNCTION__);
			return CODE_EXCEPTION;	
		}

		memcpy(msisdn_temp, msisdn.getValue(), msisdn_len);
		msisdn_temp[msisdn_len] = '\0';

		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
				"MSISDN Len = %d |%s", msisdn_len, __FUNCTION__);

		char l_msisdn_c[MSISDN_SIZE + 1] = {0};
		memset(&l_msisdn_c, '\0', sizeof(l_msisdn_c));

		// Convert BCD to char
		int l_msisdn_c_len = BCDEncoderobj.BCDToChar(l_msisdn_c, msisdn_temp, msisdn_len);
		l_msisdn_c[l_msisdn_c_len] = '\0';

		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
				"Char MSISDN = %s Len = %d |%s", l_msisdn_c, l_msisdn_c_len, __FUNCTION__);

		// New DbUtil query - Added schema name hlrdb_qc
		std::string query =
			"SELECT COUNT(1) AS msisdn_count FROM hlrdb_qc.HLR_TR_MWD_INFO "
			"WHERE MSISDN = '" + std::string(l_msisdn_c) + "' AND STATUS = 'N'";

		DbUtil::ResultSet rs = DbUtil::executeQuery(query);

		if (rs.empty())
		{
			loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,
					"DATA_NOT_FOUND | MSISDN: %s |%s", l_msisdn_c, __FUNCTION__);
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"Leaving isMsisdnFoundInMwd |%s", __FUNCTION__);
			return ROWS_NOT_FOUND;
		}

		// Get count from query result
		string countStr = rs[0].at("msisdn_count");
		int l_count_i = std::stoi(countStr);

		if(l_count_i > 0)
		{
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"Given MSISDN is found | Count: %d |%s", l_count_i, __FUNCTION__);
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"Leaving isMsisdnFoundInMwd |%s", __FUNCTION__);
			return CODE_SQL_SUCCESS;
		}
		else
		{
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"Given MSISDN is not found in Database | Count: %d |%s", l_count_i, __FUNCTION__);
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"Leaving isMsisdnFoundInMwd |%s", __FUNCTION__);
			return ROWS_NOT_FOUND;
		}
	}
	catch (const std::exception& ex)
	{
		loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0,
				"Exception occurred in isMsisdnFoundInMwd: Message [%s] |%s",
				ex.what(), __FUNCTION__);
		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
				"Leaving isMsisdnFoundInMwd |%s", __FUNCTION__);
		return CODE_EXCEPTION;
	}
}*/

/*ESqlRes SmsmDbService::isScAddressFoundInMwd(void* dbConnObj, nNumber scAddress,nNumber msisdn ) 
{
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Entering isScAddressFoundInMwd |%s",__FUNCTION__);
	DBConnHandler* pDBHandler = DBConnHandler::GetInstance();
	Connection*    conn;
	Statement* stmt;
	ResultSet* rs;
	BCDEncoder BCDEncoderobj; 
	if (pDBHandler == NULL )
	{
		loggerObj.log(LM_CRITICAL,__FILE__,__LINE__,0,0,"DB connection is null  |%s",__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Leaving isScAddressFoundInMwd   |%s",__FUNCTION__);
		return CODE_EXCEPTION;
	}

	char	l_scAddress_c[SCADDRESS_SIZE +1];
	char    l_msisdn_c[SCADDRESS_SIZE +1];
	int		l_count_i = 0;;
	memset(&l_scAddress_c, '\0', sizeof(l_scAddress_c));
	memset(&l_msisdn_c,'\0', sizeof(l_msisdn_c));

	char scaddr_temp[SCADDRESS_SIZE +1] ;
	char msisdn_temp[SCADDRESS_SIZE +1] ;
	int scaddr_len = scAddress.getOriginalLength();
	int msisdn_len = msisdn.getOriginalLength();
	memset( scaddr_temp, '\0', sizeof(scaddr_temp) );
	if( scaddr_len <= 0 ||  scaddr_len > SCADDRESS_SIZE / 2)
	{
		loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0, " SCAddress Check failed |%s",__FUNCTION__ );
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving isScAddressFoundInMwd |%s",__FUNCTION__);
		return CODE_EXCEPTION;	
	}

	if( msisdn_len <= 0 ||  msisdn_len > SCADDRESS_SIZE / 2)
	{
		loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0, " MSISDN Check failed |%s",__FUNCTION__ );
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving isScAddressFoundInMwd |%s",__FUNCTION__);
		return CODE_EXCEPTION;
	}

	memcpy(scaddr_temp, scAddress.getValue(), scaddr_len);
	memcpy(msisdn_temp, msisdn.getValue(), msisdn_len);

	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " SCAddress Len = %d |%s",scaddr_len,__FUNCTION__);
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " msisdn Len = %d |%s",msisdn_len,__FUNCTION__);

	try
	{
		int l_scAddress_c_len = BCDEncoderobj.BCDToChar(l_scAddress_c ,scaddr_temp, scaddr_len);
		int l_msisdn_c_len = BCDEncoderobj.BCDToChar(l_msisdn_c ,msisdn_temp, msisdn_len);

		l_scAddress_c[l_scAddress_c_len] = '\0';
		l_msisdn_c[l_msisdn_c_len] = '\0';

		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " CHAR SCADDR = %s Len = %d |%s",l_scAddress_c,l_scAddress_c_len,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " CHAR MSISDN = %s Len = %d |%s",l_msisdn_c,l_msisdn_c_len,    __FUNCTION__);

		stmt = pDBHandler->GetStatement((conn=pDBHandler->GetConnection()),
				"SELECT COUNT(1) FROM HLR_TR_MWD_INFO WHERE MSISDN = :1 AND SVC_CENTRE_ADDRESS = :2 AND STATUS = 'N'");
		stmt->setString(1, l_msisdn_c);
		stmt->setString(2,l_scAddress_c);
		rs = stmt->executeQuery();
		if (rs->next()) {
			l_count_i = rs->getInt(1);
		}
		if(l_count_i)
		{
			loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "Given ScAddress|%s| is  found for msisdn |%s|%s",l_scAddress_c,l_msisdn_c,__FUNCTION__ ); 
			loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving isScAddressFoundInMwd |%s",__FUNCTION__);
			pDBHandler->Close(rs, stmt, conn);
			return CODE_SQL_SUCCESS;
		}
		else
		{
			loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Given MSISDN is not found in DataBase  | %s",__FUNCTION__); 
			pDBHandler->Close(rs, stmt, conn);
			goto DATA_NOT_FOUND;
		}
	}
	catch (SQLException& ex) {
		std::cerr << "SQL Error: " << ex.getMessage() << std::endl;
		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Oracle Error code and Message |%d|%s|%s", ex.getErrorCode(), ex.getMessage().c_str(), __FUNCTION__); 
		pDBHandler->CloseStatement(stmt, conn);
		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Oracle SQL_ERROR_OCCURED  |%s", __FUNCTION__);
		return PROC_CODE_ERROR;

	}

	catch(...)
	{
		loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0, "Caught Exceptions|%s",__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving isScAddressFoundInMwd |%s",__FUNCTION__);
		pDBHandler->Close(rs, stmt, conn);
		return CODE_EXCEPTION;
	}
DATA_NOT_FOUND: 
	{
		loggerObj.log(LM_INFO,__FILE__,__LINE__,0,0, "No Data found|%s",__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving isScAddressFoundInMwd |%s",__FUNCTION__);
		return ROWS_NOT_FOUND;
	}
}*/

/*ESqlRes SmsmDbService::isScAddressFoundInMwd(void* dbConnObj, nNumber scAddress, nNumber msisdn) //xrp-2049
  {
  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, 
  "Entering isScAddressFoundInMwd |%s", __FUNCTION__);

  try
  {
  BCDEncoder BCDEncoderobj;
  char l_scAddress_c[SCADDRESS_SIZE + 1];
  char l_msisdn_c[SCADDRESS_SIZE + 1];
  char scaddr_temp[SCADDRESS_SIZE + 1];
  char msisdn_temp[SCADDRESS_SIZE + 1];

  memset(&l_scAddress_c, '\0', sizeof(l_scAddress_c));
  memset(&l_msisdn_c, '\0', sizeof(l_msisdn_c));
  memset(scaddr_temp, '\0', sizeof(scaddr_temp));
  memset(msisdn_temp, '\0', sizeof(msisdn_temp));

  int scaddr_len = scAddress.getOriginalLength();
  int msisdn_len = msisdn.getOriginalLength();

  if(scaddr_len <= 0 || scaddr_len > SCADDRESS_SIZE / 2)
  {
  loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0,
  "SCAddress Check failed | Length: %d |%s", scaddr_len, __FUNCTION__);
  return CODE_EXCEPTION;	
  }

  if(msisdn_len <= 0 || msisdn_len > SCADDRESS_SIZE / 2)
  {
  loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0,
  "MSISDN Check failed | Length: %d |%s", msisdn_len, __FUNCTION__);
  return CODE_EXCEPTION;
  }

  memcpy(scaddr_temp, scAddress.getValue(), scaddr_len);
  memcpy(msisdn_temp, msisdn.getValue(), msisdn_len);

  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
  "SCAddress Len = %d |%s", scaddr_len, __FUNCTION__);
  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
  "MSISDN Len = %d |%s", msisdn_len, __FUNCTION__);

// Convert BCD to char
int l_scAddress_c_len = BCDEncoderobj.BCDToChar(l_scAddress_c, scaddr_temp, scaddr_len);
int l_msisdn_c_len = BCDEncoderobj.BCDToChar(l_msisdn_c, msisdn_temp, msisdn_len);

l_scAddress_c[l_scAddress_c_len] = '\0';
l_msisdn_c[l_msisdn_c_len] = '\0';

loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
"CHAR SCADDR = %s Len = %d |%s", l_scAddress_c, l_scAddress_c_len, __FUNCTION__);
loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
"CHAR MSISDN = %s Len = %d |%s", l_msisdn_c, l_msisdn_c_len, __FUNCTION__);

// New DbUtil query - Added schema name hlrdb_qc
std::string query =
"SELECT COUNT(1) AS address_count FROM hlrdb_qc.HLR_TR_MWD_INFO "
"WHERE MSISDN = '" + std::string(l_msisdn_c) + "' "
"AND SVC_CENTRE_ADDRESS = '" + std::string(l_scAddress_c) + "' "
"AND STATUS = 'N'";

DbUtil::ResultSet rs = DbUtil::executeQuery(query);

if (rs.empty())
{
loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,
"No Data found | SCAddress: %s, MSISDN: %s |%s", 
l_scAddress_c, l_msisdn_c, __FUNCTION__);
loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
"Leaving isScAddressFoundInMwd |%s", __FUNCTION__);
return ROWS_NOT_FOUND;
}

// Get count from query result
string countStr = rs[0].at("address_count");
int l_count_i = std::stoi(countStr);

if(l_count_i > 0)
{
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
			"Given ScAddress|%s| is found for msisdn |%s| | Count: %d |%s",
			l_scAddress_c, l_msisdn_c, l_count_i, __FUNCTION__);
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
			"Leaving isScAddressFoundInMwd |%s", __FUNCTION__);
	return CODE_SQL_SUCCESS;
}
else
{
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
			"Given SCAddress|%s| is not found for msisdn |%s| in Database | Count: %d |%s",
			l_scAddress_c, l_msisdn_c, l_count_i, __FUNCTION__);
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
			"Leaving isScAddressFoundInMwd |%s", __FUNCTION__);
	return ROWS_NOT_FOUND;
}
}
catch (const std::exception& ex)
{
	loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0,
			"Exception occurred in isScAddressFoundInMwd: Message [%s] |%s",
			ex.what(), __FUNCTION__);
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
			"Leaving isScAddressFoundInMwd |%s", __FUNCTION__);
	return CODE_EXCEPTION;
}
}*/
ESqlRes SmsmDbService::getScAdresses(void* dbConnObj, nNumber& msisdn, list<nNumber>& scad ) 
{
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "Entering  getScAdresses |%s",__FUNCTION__);
	DBConnHandler* pDBHandler = DBConnHandler::GetInstance();
	Connection* conn;
	Statement* stmt;
	ResultSet* rs;
	int count=0;
	BCDEncoder BCDEncoderobj;
	if (pDBHandler == NULL )
	{
		loggerObj.log(LM_CRITICAL,__FILE__,__LINE__,0,0,"DB connection is null  |%s",__FUNCTION__);
		return CODE_EXCEPTION;
	}

	char msisdn_temp[MSISDN_SIZE +1] ={ 0 };
	int msisdn_len = msisdn.getOriginalLength();

	if( msisdn_len <= 0 ||  msisdn_len > MSISDN_SIZE / 2)
	{
		loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0, " Msisdn Check failed | %s |",__FUNCTION__  );
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving getScAdresses |%s",__FUNCTION__);
		return CODE_EXCEPTION;	
	}

	memcpy(msisdn_temp,  msisdn.getValue(), msisdn_len);  
	msisdn_temp[msisdn_len]='\0';

	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " MSISDN Len = %d |%s",msisdn_len,__FUNCTION__);
	char		l_msisdn_c[MSISDN_SIZE +1];
	char		l_scAddress_c[SCADDRESS_SIZE +1];
	memset(&l_msisdn_c,  '\0',  sizeof(l_msisdn_c));
	memset(&l_scAddress_c, '\0',  sizeof(l_scAddress_c));
	unsigned short int l_scAddress_c_len;
	try
	{
		int l_msisdn_c_len = BCDEncoderobj.BCDToChar(l_msisdn_c ,msisdn_temp, msisdn_len);
		l_msisdn_c[l_msisdn_c_len] = '\0';
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " CHAR l_msisdn_c = %s | msisdn_len = %d |%s",l_msisdn_c ,l_msisdn_c_len,__FUNCTION__ );

		char scd_temp[ SCADDRESS_SIZE + 1 ] ;
		int scd_len = -1; 

		conn = pDBHandler->GetConnection();
		stmt = conn->createStatement("SELECT NVL(SVC_CENTRE_ADDRESS, '0') FROM HLR_TR_MWD_INFO WHERE MSISDN = :1 AND STATUS = 'N'");
		stmt->setString(1, l_msisdn_c);
		ResultSet* rs = stmt->executeQuery();
		while (rs->next()) {
			memset(scd_temp, '\0', sizeof(scd_temp));  
			scd_len = -1;  
			string l_scAddress_ = rs->getString(1); 
			strncpy(l_scAddress_c, l_scAddress_.c_str(), sizeof(l_scAddress_c) - 1);
			l_scAddress_c[sizeof(l_scAddress_c) - 1] = '\0';  

			l_scAddress_c_len=strlen(l_scAddress_c);
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "l_scAddress_c = %s|%s", l_scAddress_c, __FUNCTION__);

			if (!(l_scAddress_c[0] ==  0x30 && l_scAddress_c_len == 1)) {
				scd_len = BCDEncoderobj.CharToBCD(scd_temp, l_scAddress_c, l_scAddress_c_len);
				nNumber scaddr(scd_len);
				scaddr.setValue(scd_temp, scd_len);
				scad.push_back(scaddr);
				count++;  
			}
		}
		if (count>0) {
			loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "scaddress Obtained|%s",__FUNCTION__);
			loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving getScAdresses |%s",__FUNCTION__);
			pDBHandler->Close(rs, stmt, conn);
			return CODE_SQL_SUCCESS;
		}
		if(count=0)
		{
			loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "scaddress is not obtained|%s",__FUNCTION__);
			pDBHandler->Close(rs, stmt, conn);
			goto DATA_NOT_FOUND;
		}
	}

	catch (SQLException& ex) {
		std::cerr << "SQL Error: " << ex.getMessage() << std::endl;
		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Oracle Error code and Message |%d|%s|%s", ex.getErrorCode(), ex.getMessage().c_str(), __FUNCTION__); 
		pDBHandler->CloseStatement(stmt, conn);
		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Oracle SQL_ERROR_OCCURED  |%s", __FUNCTION__);
		return PROC_CODE_ERROR;

	}

	catch(...)
	{
		loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0, "Caught Exceptions|%s",__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving getScAdresses |%s",__FUNCTION__);
		pDBHandler->Close(rs, stmt, conn);
		return CODE_EXCEPTION;
	}

DATA_NOT_FOUND: 
	{
		loggerObj.log(LM_INFO,__FILE__,__LINE__,0,0, " No Data found |%s",__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving getScAdresses |%s",__FUNCTION__);
		return ROWS_NOT_FOUND;
	}
}

/*ESqlRes SmsmDbService::getDistinctMsisdnInMwd(void* dbConnObj, list<nNumber> &msdn )
  {
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Entering getDistinctMsisdnInMwd |%s",__FUNCTION__);
  DBConnHandler* pDBHandler = DBConnHandler::GetInstance();  
  Connection*    conn;
  Statement* stmt;
  ResultSet* rs;
  int count=0;
  BCDEncoder BCDEncoderobj;
  if (pDBHandler == NULL )
  {
  loggerObj.log(LM_CRITICAL,__FILE__,__LINE__,0,0,"DB connection is null  |%s",__FUNCTION__);
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Leaving getDistinctMsisdnInMwd   |%s",__FUNCTION__);
  return CODE_EXCEPTION;
  }
  char l_msisdn_c[MSISDN_SIZE +1];

  memset( &l_msisdn_c, '\0', sizeof(l_msisdn_c));
  unsigned short int l_msisdn_c_len=0;
  try
  {
  char msdn_temp[MSISDN_SIZE +1];
  int msisdn_len;
  stmt = pDBHandler->GetStatement((conn = pDBHandler->GetConnection()),
  "SELECT DISTINCT MSISDN FROM HLR_TR_MWD_INFO WHERE STATUS = 'N'");
  ResultSet* rs = stmt->executeQuery();

  while (rs->next()) {
  memset(msdn_temp, '\0', sizeof(msdn_temp)); 
  string l_msisdn_ = rs->getString(1);

  strncpy(l_msisdn_c, l_msisdn_.c_str(), sizeof(l_msisdn_c) - 1);
  l_msisdn_c[sizeof(l_msisdn_c) - 1] = '\0';  // Ensure null termination

  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "l_msisdn_c = %s | %s", l_msisdn_c, __FUNCTION__);

  int l_msisdn_c_len = strlen(l_msisdn_c);
  if (!(l_msisdn_c[0] == 0x30 && l_msisdn_c_len == 1)) {
  msisdn_len = BCDEncoderobj.CharToBCD(msdn_temp, l_msisdn_c, l_msisdn_c_len);
  nNumber msisdn(msisdn_len);
  msisdn.setValue(msdn_temp, msisdn_len);
  msdn.push_back(msisdn);
  count++;
  }
  }

  if (count > 0) {   
  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "List of msisdn found | %s", __FUNCTION__);
  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Leaving getDistinctMsisdnInMwd | %s", __FUNCTION__);
  pDBHandler->Close(rs, stmt, conn);
  return CODE_SQL_SUCCESS;
  }


  if(count=0)
  { 	
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "scaddress is not obtained|%s",__FUNCTION__);
  pDBHandler->Close(rs, stmt, conn);
  goto DATA_NOT_FOUND;
  }
  }

  catch (SQLException& ex) {
  std::cerr << "SQL Error: " << ex.getMessage() << std::endl;
  loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Oracle Error code and Message |%d|%s|%s", ex.getErrorCode(), ex.getMessage().c_str(), __FUNCTION__); 

  pDBHandler->CloseStatement(stmt, conn);
  loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Oracle SQL_ERROR_OCCURED  |%s", __FUNCTION__);
  return PROC_CODE_ERROR;

  }
catch(...)
{
	loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0, "Caught Exception|%s",__FUNCTION__);
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving getDistinctMsisdnInMwd |%s",__FUNCTION__);
	pDBHandler->Close(rs, stmt, conn);
	return CODE_EXCEPTION;
}

DATA_NOT_FOUND: 
{

	loggerObj.log(LM_INFO,__FILE__,__LINE__,0,0, "No Data found|%s",__FUNCTION__);
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving getDistinctMsisdnInMwd |%s",__FUNCTION__);
	return ROWS_NOT_FOUND;
}

}*/

ESqlRes SmsmDbService::getDistinctMsisdnInMwd(void* dbConnObj, list<nNumber> &msdn)//xrp-2049
{
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, 
			"Entering getDistinctMsisdnInMwd |%s", __FUNCTION__);

	try
	{
		BCDEncoder BCDEncoderobj;
		int count = 0;
		char l_msisdn_c[MSISDN_SIZE + 1] = {0};

		// New DbUtil query - Added schema name hlrdb_qc
		std::string query =
			"SELECT DISTINCT MSISDN FROM hlrdb_qc.HLR_TR_MWD_INFO WHERE STATUS = 'N'";

		DbUtil::ResultSet rs = DbUtil::executeQuery(query);

		if (rs.empty())
		{
			loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,
					"No Data found |%s", __FUNCTION__);
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"Leaving getDistinctMsisdnInMwd |%s", __FUNCTION__);
			return ROWS_NOT_FOUND;
		}

		char msdn_temp[MSISDN_SIZE + 1];

		for (const auto& row : rs)
		{
			memset(msdn_temp, '\0', sizeof(msdn_temp));
			memset(l_msisdn_c, '\0', sizeof(l_msisdn_c));

			string l_msisdn_ = row.at("MSISDN");
			strncpy(l_msisdn_c, l_msisdn_.c_str(), sizeof(l_msisdn_c) - 1);
			l_msisdn_c[sizeof(l_msisdn_c) - 1] = '\0';

			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"l_msisdn_c = %s |%s", l_msisdn_c, __FUNCTION__);

			int l_msisdn_c_len = strlen(l_msisdn_c);

			// Check if MSISDN is not "0"
			if (!(l_msisdn_c[0] == 0x30 && l_msisdn_c_len == 1)) {
				int msisdn_len = BCDEncoderobj.CharToBCD(msdn_temp, l_msisdn_c, l_msisdn_c_len);
				nNumber msisdn(msisdn_len);
				msisdn.setValue(msdn_temp, msisdn_len);
				msdn.push_back(msisdn);
				count++;
			}
		}

		if (count > 0)
		{
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"List of MSISDN found | Count: %d |%s", count, __FUNCTION__);
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"Leaving getDistinctMsisdnInMwd |%s", __FUNCTION__);
			return CODE_SQL_SUCCESS;
		}
		else
		{
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"No valid MSISDN found |%s", __FUNCTION__);
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"Leaving getDistinctMsisdnInMwd |%s", __FUNCTION__);
			return ROWS_NOT_FOUND;
		}
	}
	catch (const std::exception& ex)
	{
		loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0,
				"Exception occurred in getDistinctMsisdnInMwd: Message [%s] |%s",
				ex.what(), __FUNCTION__);
		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
				"Leaving getDistinctMsisdnInMwd |%s", __FUNCTION__);
		return CODE_EXCEPTION;
	}
}

/*ESqlRes SmsmDbService::routeSmsToHplmnSmsc(DataFromDb *obj,nNumber&  scAddress)
  {
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Entering routeSmsToHplmnSmsc |%s",__FUNCTION__);
  DBConnHandler* pDBHandler = DBConnHandler::GetInstance();
  Connection*    conn;
  Statement* stmt;
  ResultSet* rs;
  int count=0;
  BCDEncoder BCDEncoderobj;

  if (pDBHandler == NULL )
  {
  loggerObj.log(LM_CRITICAL,__FILE__,__LINE__,0,0,"DB connection is null  |%s",__FUNCTION__);
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"Leaving routeSmsToHplmnSmsc  |%s",__FUNCTION__);
  return CODE_EXCEPTION;
  }

  char		l_msisdn_c[MSISDN_SIZE +1];
  char		l_imsi_c[IMSI_SIZE +1];
  char		l_param_c[VAR_STRING_SIZE +1];
  char		l_val_c[VAR_STRING_SIZE +1];
  int l_count=0;
  string l_val_;
  memset( &l_msisdn_c, '\0', sizeof(l_msisdn_c));
  memset( &l_imsi_c, '\0', sizeof(l_imsi_c));
  memset( &l_param_c, '\0', sizeof(l_param_c));
  memset( &l_val_c, '\0', sizeof(l_val_c));

  unsigned int queryIndex = 0;

  try
  {
  if(obj->getSmscNumber().getOriginalLength() > 1)
  {
  int l_param_c_len=strlen("SMS_FILTER_ENABLED_IN_SMSC");
  strcpy(l_param_c, "SMS_FILTER_ENABLED_IN_SMSC");
  l_param_c[sizeof(l_param_c)-1]='\0';
  queryIndex=1;
  stmt = pDBHandler->GetStatement((conn=pDBHandler->GetConnection()),
  "SELECT NVL(VALUE,' ') FROM HLR_MT_SYS_PARAMS WHERE PARAMETER = :1"); 
  stmt->setString(1, l_param_c);
  rs = stmt->executeQuery();
  if (rs->next()) 
  {
  l_val_ = rs->getString(1);
  strncpy(l_val_c,l_val_.c_str(),sizeof(l_val_c)-1);
  l_val_c[sizeof(l_val_c) - 1] = '\0';
  }

  else 
  {
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " no Datafound %d|%s",queryIndex,__FUNCTION__);
  pDBHandler->Close(rs,stmt,conn);
  }

  int filterSms=0, liInterceptReq=0;
  if(l_val_c[0] != ' ')
  filterSms=atoi(l_val_c);

  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " PARAM[%s] Val[%s] filterSms[%d]|%s", l_param_c, l_val_c, filterSms,__FUNCTION__);

  pDBHandler->Close(rs,stmt,conn);

  memset( &l_param_c, '\0', sizeof(l_param_c));
  l_param_c_len=strlen("NON_HPLMN_MT_SMS_LI_REQUIRED");
  strcpy(l_param_c, "NON_HPLMN_MT_SMS_LI_REQUIRED");
  l_param_c[strlen(l_param_c)]='\0';

  memset( &l_val_c, '\0', sizeof(l_val_c));
  queryIndex=2;
  stmt = pDBHandler->GetStatement((conn=pDBHandler->GetConnection()),"SELECT NVL(VALUE,' ')  FROM HLR_MT_SYS_PARAMS WHERE PARAMETER = :1") ; 
stmt->setString(1, l_param_c);
rs = stmt->executeQuery();
if (rs->next()) {
	l_val_ = rs->getString(1);
	strncpy(l_val_c,l_val_.c_str(),sizeof(l_val_c)-1);
	l_val_c[sizeof(l_val_c) - 1] = '\0';
}
else 
{
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " no Datafound %d|%s",queryIndex,__FUNCTION__);
	pDBHandler->Close(rs,stmt,conn);
}
if(l_val_c[0] != ' ')
liInterceptReq=atoi(l_val_c);

loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " PARAM[%s] Val[%s] liInterceptReq[%d]|%s", l_param_c, l_val_c, liInterceptReq,__FUNCTION__);

char roamingStatus= (obj->getRoamingStatus().getValue())[0];

if(liInterceptReq == 2) 
{
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " RoamingStatus [%d]|%s", roamingStatus,__FUNCTION__);
	if(roamingStatus == '0') 
		liInterceptReq=0;
}
if(liInterceptReq) 
{
	char msisdn_temp[MSISDN_SIZE +1]={'\0'};

	memcpy(msisdn_temp,  obj->getMsisdn().getValue(), obj->getMsisdn().getOriginalLength());
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " BCD MSISDN Len = %d|%s", obj->getMsisdn().getOriginalLength(),__FUNCTION__);

	int  l_msisdn_c_len = BCDEncoderobj.BCDToChar( l_msisdn_c, &msisdn_temp[0], obj->getMsisdn().getOriginalLength());
	l_msisdn_c[sizeof(l_msisdn_c)-1] = '\0';
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " CHAR MSISDN = %s| and MSISDN Len = %d |%s",l_msisdn_c,l_msisdn_c_len,__FUNCTION__);

	pDBHandler->Close(rs,stmt,conn);
	queryIndex=3;

	stmt = pDBHandler->GetStatement(
			(conn = pDBHandler->GetConnection()),
			"SELECT NVL(COUNT(CASE_ID), 0) "
			"FROM HLR_TT_LAWFULL_INTERCPT_INFO "
			"WHERE MSISDN = :1 "
			"OR (PREFIX = '1' AND MSISDN = SUBSTR(:1, 1, LENGTH(MSISDN)))");

	stmt->setString(1, l_msisdn_c);

	rs = stmt->executeQuery();

	if (rs->next()) {
		l_count = rs->getInt(1);  

	}

	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " LI Enabled count = %d for MSISDN [%s]|%s", l_count, l_msisdn_c, __FUNCTION__);
	pDBHandler->Close(rs,stmt,conn);

	if(l_count<1)
		liInterceptReq=0;
}
if( filterSms==1 || (liInterceptReq > 0) ) 
{
	char smscGtVal[20]={'\0'};
	BCDEncoderobj.BCDToChar(smscGtVal ,scAddress.getValue() , scAddress.getOriginalLength());

	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " SMS Originated SMSC %s HPLMN SMSC %s |%s", smscGtVal, obj->getSmscNumber().getValue(),__FUNCTION__);

	if(strlen(smscGtVal) == obj->getSmscNumber().getOriginalLength())
	{
		if(memcmp(smscGtVal, obj->getSmscNumber().getValue(), obj->getSmscNumber().getOriginalLength()) == 0)
		{   
			loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " SMS Originated SMSC %s HPLMN SMSC %s are same so inter SMSC routing is not required |%s", smscGtVal, obj->getSmscNumber().getValue(),__FUNCTION__);
			loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving routeSmsToHplmnSmsc |%s",__FUNCTION__);
			return ROWS_NOT_FOUND; 
		}
	}
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " SMS Originated SMSC %s HPLMN SMSC %s are not same so inter SMSC routing is required |%s", smscGtVal, obj->getSmscNumber().getValue(),__FUNCTION__);
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving routeSmsToHplmnSmsc |%s",__FUNCTION__);					
	return CODE_SQL_SUCCESS;
}
}
loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving routeSmsToHplmnSmsc |%s",__FUNCTION__);
return ROWS_NOT_FOUND;
}
catch (SQLException& ex) {
	std::cerr << "SQL Error: " << ex.getMessage() << std::endl;
	loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Oracle Error code and Message |%d|%s|%s", ex.getErrorCode(), ex.getMessage().c_str(), __FUNCTION__); 
	loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Oracle SQL_ERROR_OCCURED  |%s", __FUNCTION__);
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving routeSmsToHplmnSmsc |%s",__FUNCTION__);
	pDBHandler->CloseStatement(stmt, conn);
	return PROC_CODE_ERROR;

}

catch(...)
{
	loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0, "Query Index:%d Caught Exception|%s",queryIndex,__FUNCTION__);
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving routeSmsToHplmnSmsc |%s",__FUNCTION__);
	return CODE_EXCEPTION;
}
}*/

ESqlRes SmsmDbService::routeSmsToHplmnSmsc(DataFromDb *obj, nNumber& scAddress)//xrp-2049
{
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, 
			"Entering routeSmsToHplmnSmsc |%s", __FUNCTION__);

	try
	{
		BCDEncoder BCDEncoderobj;

		// Check if SMSC number is valid
		if(obj->getSmscNumber().getOriginalLength() <= 1)
		{
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"SMSC number length invalid, no routing required |%s", __FUNCTION__);
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"Leaving routeSmsToHplmnSmsc |%s", __FUNCTION__);
			return ROWS_NOT_FOUND;
		}

		char l_param_c[VAR_STRING_SIZE + 1] = {0};
		char l_val_c[VAR_STRING_SIZE + 1] = {0};
		int filterSms = 0, liInterceptReq = 0;

		// Query 1: Get SMS_FILTER_ENABLED_IN_SMSC parameter
		std::string query1 = 
			"SELECT NVL(VALUE,' ') AS param_value FROM hlrdb_qc.HLR_MT_SYS_PARAMS "
			"WHERE PARAMETER = 'SMS_FILTER_ENABLED_IN_SMSC'";

		DbUtil::ResultSet rs1 = DbUtil::executeQuery(query1);

		if (!rs1.empty())
		{
			string l_val_ = rs1[0].at("param_value");
			strncpy(l_val_c, l_val_.c_str(), sizeof(l_val_c) - 1);
			l_val_c[sizeof(l_val_c) - 1] = '\0';

			if(l_val_c[0] != ' ')
				filterSms = atoi(l_val_c);
		}

		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
				"PARAM[SMS_FILTER_ENABLED_IN_SMSC] Val[%s] filterSms[%d]|%s", 
				l_val_c, filterSms, __FUNCTION__);

		// Query 2: Get NON_HPLMN_MT_SMS_LI_REQUIRED parameter
		memset(l_val_c, '\0', sizeof(l_val_c));
		std::string query2 = 
			"SELECT NVL(VALUE,' ') AS param_value FROM hlrdb_qc.HLR_MT_SYS_PARAMS "
			"WHERE PARAMETER = 'NON_HPLMN_MT_SMS_LI_REQUIRED'";

		DbUtil::ResultSet rs2 = DbUtil::executeQuery(query2);

		if (!rs2.empty())
		{
			string l_val_ = rs2[0].at("param_value");
			strncpy(l_val_c, l_val_.c_str(), sizeof(l_val_c) - 1);
			l_val_c[sizeof(l_val_c) - 1] = '\0';

			if(l_val_c[0] != ' ')
				liInterceptReq = atoi(l_val_c);
		}

		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
				"PARAM[NON_HPLMN_MT_SMS_LI_REQUIRED] Val[%s] liInterceptReq[%d]|%s", 
				l_val_c, liInterceptReq, __FUNCTION__);

		// Process roaming status for LI requirement
		char roamingStatus = (obj->getRoamingStatus().getValue())[0];
		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
				"RoamingStatus [%d]|%s", roamingStatus, __FUNCTION__);

		if(liInterceptReq == 2 && roamingStatus == '0') 
		{
			liInterceptReq = 0;
		}

		// Check Lawful Interception if required
		if(liInterceptReq > 0) 
		{
			char l_msisdn_c[MSISDN_SIZE + 1] = {0};
			char msisdn_temp[MSISDN_SIZE + 1] = {'\0'};

			memcpy(msisdn_temp, obj->getMsisdn().getValue(), obj->getMsisdn().getOriginalLength());
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"BCD MSISDN Len = %d|%s", obj->getMsisdn().getOriginalLength(), __FUNCTION__);

			int l_msisdn_c_len = BCDEncoderobj.BCDToChar(l_msisdn_c, &msisdn_temp[0], obj->getMsisdn().getOriginalLength());
			l_msisdn_c[sizeof(l_msisdn_c) - 1] = '\0';
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"CHAR MSISDN = %s| and MSISDN Len = %d |%s", l_msisdn_c, l_msisdn_c_len, __FUNCTION__);

			// Query 3: Check Lawful Interception
			std::string query3 = 
				"SELECT NVL(COUNT(CASE_ID), 0) AS li_count "
				"FROM hlrdb_qc.HLR_TT_LAWFULL_INTERCPT_INFO "
				"WHERE MSISDN = '" + std::string(l_msisdn_c) + "' "
				"OR (PREFIX = '1' AND MSISDN = SUBSTR('" + std::string(l_msisdn_c) + "', 1, LENGTH(MSISDN)))";

			DbUtil::ResultSet rs3 = DbUtil::executeQuery(query3);

			int l_count = 0;
			if (!rs3.empty())
			{
				string countStr = rs3[0].at("li_count");
				l_count = std::stoi(countStr);
			}

			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"LI Enabled count = %d for MSISDN [%s]|%s", l_count, l_msisdn_c, __FUNCTION__);

			if(l_count < 1)
				liInterceptReq = 0;
		}

		// Check if SMS filtering or LI is required
		if(filterSms == 1 || liInterceptReq > 0) 
		{
			char smscGtVal[20] = {'\0'};
			BCDEncoderobj.BCDToChar(smscGtVal, scAddress.getValue(), scAddress.getOriginalLength());

			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"SMS Originated SMSC %s HPLMN SMSC %s |%s", 
					smscGtVal, obj->getSmscNumber().getValue(), __FUNCTION__);

			// Compare SMSC addresses
			if(strlen(smscGtVal) == obj->getSmscNumber().getOriginalLength())
			{
				if(memcmp(smscGtVal, obj->getSmscNumber().getValue(), obj->getSmscNumber().getOriginalLength()) == 0)
				{   
					loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
							"SMS Originated SMSC %s HPLMN SMSC %s are same so inter SMSC routing is not required |%s", 
							smscGtVal, obj->getSmscNumber().getValue(), __FUNCTION__);
					loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
							"Leaving routeSmsToHplmnSmsc |%s", __FUNCTION__);
					return ROWS_NOT_FOUND; 
				}
			}

			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"SMS Originated SMSC %s HPLMN SMSC %s are not same so inter SMSC routing is required |%s", 
					smscGtVal, obj->getSmscNumber().getValue(), __FUNCTION__);
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"Leaving routeSmsToHplmnSmsc |%s", __FUNCTION__);
			return CODE_SQL_SUCCESS;
		}

		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
				"No SMS filtering or LI required |%s", __FUNCTION__);
		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
				"Leaving routeSmsToHplmnSmsc |%s", __FUNCTION__);
		return ROWS_NOT_FOUND;
	}
	catch (const std::exception& ex)
	{
		loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0,
				"Exception occurred in routeSmsToHplmnSmsc: Message [%s] |%s",
				ex.what(), __FUNCTION__);
		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
				"Leaving routeSmsToHplmnSmsc |%s", __FUNCTION__);
		return CODE_EXCEPTION;
	}
}

ESqlRes SmsmDbService::updateSmsInfo(UpdateDb *obj) 
{
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Entering updateSmsInfo |%s",__FUNCTION__);
  BCDEncoder BCDEncoderobj;
  UpdateSubscriberDb Usd;

  if (obj == NULL) 
  {   
  loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0,"obj is null  |%s",__FUNCTION__);
  loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0,"Leaving updateSmsInfo |%s",__FUNCTION__);
  return CODE_EXCEPTION;
  }

  char imsi_temp[IMSI_SIZE +1] ;
  char msisdn_temp[MSISDN_SIZE +1] ;

  nNumber imsi = obj->getImsi();
  int imsi_len =  imsi.getOriginalLength();

  nNumber msisdn=obj->getMsisdn();
  int msisdn_len =  msisdn.getOriginalLength();
  
 // msisdn=522222798068;//add by sai for success blow condition 
  msisdn_len=16;
  
  
  
    /*if ( imsi_len <= 0 && msisdn_len <=0 || imsi_len > IMSI_SIZE / 2 || msisdn_len > MSISDN_SIZE / 2 )
  {
  cout<<"1034**********"<<endl;
  loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0,"IMSI  and MSISDN check failed   |%s",__FUNCTION__);
  loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0,"Leaving updateSmsInfo|%s",__FUNCTION__); 
  return CODE_EXCEPTION;
  }*/// commmendtdb  for flow for forword
  
  memset(imsi_temp,  '\0',  sizeof(imsi_temp) );
  memset(msisdn_temp, '\0', sizeof(msisdn_temp) );
  memcpy(imsi_temp, imsi.getValue(), imsi_len);
  
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"imsi_temp = %s|%s",imsi_temp,__FUNCTION__);
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"imsi_len = %d|%s",imsi_len,__FUNCTION__);
  

  memcpy(msisdn_temp,  msisdn.getValue(), msisdn_len);
  
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"msisdn_temp= %s |%s",msisdn_temp,__FUNCTION__);
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"MSISDN Len = %d |%s",msisdn_len,__FUNCTION__);

  char		l_imsi_c[IMSI_SIZE +1];
  char		l_msisdn_c[MSISDN_SIZE +1];
  char		l_lmsi_c[LMSI_SIZE +1];
  char		l_mnrg_c[MNRG_SIZE +1];
  char		l_mnrf_c[MNRF_SIZE + 1] ; 
  char		l_mnrr_c_gsm[MNRR_SIZE +1] = {'\0'};
  char		l_mnrr_c_gprs[MNRR_SIZE +1] = {'\0'};
  char		l_mcef_c[MCEF_SIZE +1];
  char		l_sgsnAddress_c[SGSNADDR_SIZE +1]; 
  char		l_sgsnNumber_c[SGSNNUM_SIZE + 1];
  char		l_notreachableGsm_c[ NOTREACHABLE_SIZE +1];
  char		l_notreachableGprs_c[ NOTREACHABLE_SIZE + 1];
//Begin spell-2100
  char		l_mspurgedNonGprs_c[ MSPURGENONGPRS_SIZE + 1];
  char		l_mspurgedGprs_c[ MSPURGEGPRS_SIZE + 1];
  char		l_superCharger_c_gsm[SUPERCHARGERSUPPORTSINVLR_SIZE +1] ; 
  char		l_superCharger_c_gprs[SUPERCHARGERSUPPORTSINSGSN_SIZE +1];
//End spell-2100

//copy input params to the host variables

  memset(&l_imsi_c,  '\0',  sizeof(l_imsi_c) );
  memset(&l_msisdn_c, '\0', sizeof(l_msisdn_c) );
  memset(&l_lmsi_c, '\0',   sizeof(l_lmsi_c) );
  memset(&l_mnrg_c,  '\0',  sizeof(l_mnrg_c) );
  memset(&l_mnrf_c,  '\0',  sizeof(l_mnrf_c) );
  memset(&l_mcef_c,  '\0',  sizeof(l_mcef_c) );
  //  memset(&l_mnrr_c,  '\0',  sizeof(l_mnrr_c) );
  memset(&l_sgsnAddress_c,  '\0',  sizeof(l_sgsnAddress_c));
  memset(&l_sgsnNumber_c,  '\0',  sizeof(l_sgsnNumber_c));
  memset(&l_notreachableGsm_c,  '\0',  sizeof(l_notreachableGsm_c) );
  memset(&l_notreachableGprs_c,  '\0',  sizeof(l_notreachableGprs_c) );


  memset(&l_mspurgedGprs_c,  '\0',  sizeof(l_mspurgedGprs_c) );
  memset(&l_mspurgedNonGprs_c,  '\0',  sizeof(l_mspurgedNonGprs_c) );

  memset(&l_superCharger_c_gsm, '\0', sizeof(l_superCharger_c_gsm));
  memset(&l_superCharger_c_gprs, '\0', sizeof(l_superCharger_c_gprs));


DBConnHandler* pDBHandler = DBConnHandler::GetInstance();
Connection*    conn;
Statement* stmt;

if (pDBHandler == NULL )
{
	loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0,"DB connection is null  |%s",__FUNCTION__);
	loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0,"Leaving updateSmsInfo   |%s",__FUNCTION__);
	return CODE_EXCEPTION;
}

int queryIndex=0; 
try
{ 
	int l_imsi_c_len = BCDEncoderobj.BCDToChar(l_imsi_c ,imsi_temp, imsi_len);
	l_imsi_c[sizeof(l_imsi_c)-1] = '\0';
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"CHAR IMSI = %s| Len = %d |%s",l_imsi_c,l_imsi_c_len,__FUNCTION__);
 
	int l_msisdn_c_len = BCDEncoderobj.BCDToChar(l_msisdn_c ,msisdn_temp, msisdn_len);
	l_msisdn_c[sizeof(l_msisdn_c)-1] = '\0';
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0," CHAR MSISDN = %s| Len = %d |%s",l_msisdn_c,l_msisdn_c_len,__FUNCTION__);
  
  //cout<<"&&&&&&&&&&&& BCDToChar"<<l_msisdn_c<<endl;

	nNumber lmsi = obj->getLmsi();   
	memcpy( l_lmsi_c, lmsi.getValue(), lmsi.getOriginalLength());
	int l_lmsi_c_len = lmsi.getOriginalLength();
	l_lmsi_c[sizeof(l_lmsi_c)-1] = '\0';
  
  
  
	nNumber mnrf = obj->getMnrf();
	memcpy( l_mnrf_c, mnrf.getValue(), mnrf.getOriginalLength());
	int l_mnrf_c_len = mnrf.getOriginalLength();
	l_mnrf_c[sizeof(l_mnrf_c)-1] = '\0';
  //Usd.l_mnrf_c=mnrf.getValue();

	nNumber mcef = obj->getMcef();
	memcpy( l_mcef_c, mcef.getValue(), mcef.getOriginalLength());
	int l_mcef_c_len = mcef.getOriginalLength();
	l_mcef_c[sizeof(l_mcef_c)-1] =  '\0';
  
 
 
	nNumber mnrr_gsm = obj->getMnrrGsm();
	memcpy(l_mnrr_c_gsm, mnrr_gsm.getValue(), mnrr_gsm.getOriginalLength());
	int l_mnrr_c_gsm_len = mnrr_gsm.getOriginalLength();
	l_mnrr_c_gsm[sizeof(l_mnrr_c_gsm)-1] =  '\0';
  

   
	nNumber mnrg = obj->getMnrg();
	memcpy(l_mnrg_c, mnrg.getValue(), mnrg.getOriginalLength());
	int l_mnrg_c_len = mnrg.getOriginalLength();
	l_mnrg_c[sizeof(l_mnrg_c)-1] =  '\0';
  
  
   
	nNumber msNotReachableFlagForGsm = obj->getMsNotReachableFlagForGsm();
	memcpy( l_notreachableGsm_c, msNotReachableFlagForGsm.getValue(), msNotReachableFlagForGsm.getOriginalLength() );
	int l_notreachableGsm_c_len = msNotReachableFlagForGsm.getOriginalLength();
	l_notreachableGsm_c[sizeof(l_notreachableGsm_c)-1] =  '\0';
  


	nNumber msNotReachableFlagForGprs = obj->getMsNotReachableFlagForGprs();
	memcpy( l_notreachableGprs_c, msNotReachableFlagForGprs.getValue(), msNotReachableFlagForGprs.getOriginalLength() );
	int l_notreachableGprs_c_len = msNotReachableFlagForGprs.getOriginalLength();
	l_notreachableGprs_c[sizeof(l_notreachableGprs_c)-1] =  '\0';

 

	char sgsnAddr_temp[SGSNADDR_SIZE+1];
	nNumber sgsnaddr = obj->getSgsnAddress();

	int sgsnaddrlen = sgsnaddr.getOriginalLength();   
	memcpy(sgsnAddr_temp, sgsnaddr.getValue(),sgsnaddrlen); 
	int l_sgsnAddress_c_len = BCDEncoderobj.BCDToChar(l_sgsnAddress_c ,sgsnAddr_temp, sgsnaddrlen);
	l_sgsnAddress_c[sizeof(l_sgsnAddress_c)-1] = '\0';
  


	char sgsnNum_temp[SGSNNUM_SIZE +1];
	nNumber sgsnnum = obj->getSgsnNumber();

	int sgsnlen = sgsnnum.getOriginalLength();
	memcpy(sgsnNum_temp, sgsnnum.getValue(),  sgsnlen);
	int l_sgsnNumber_c_len = BCDEncoderobj.BCDToChar(l_sgsnNumber_c ,sgsnNum_temp, sgsnlen);
	l_sgsnNumber_c[sizeof(l_sgsnNumber_c)-1] = '\0';
  

  
	int trrowsUpdated=0;
	int gprsrowsUpdated=0;

	nNumber mspurgedNonGPRS = obj->getMsPurgedNonGprsFlag();
	memcpy( l_mspurgedNonGprs_c, mspurgedNonGPRS.getValue(), mspurgedNonGPRS.getOriginalLength());
	int l_mspurgedNonGprs_c_len = mspurgedNonGPRS.getOriginalLength();
	l_mspurgedNonGprs_c[sizeof(l_mspurgedNonGprs_c)-1] = '\0';
  
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"l_mspurgedNonGprs_c = %s|%s",l_mspurgedNonGprs_c,__FUNCTION__);
  


	nNumber mspurgedGPRS = obj->getMsPurgedGPRSFlag();
	memcpy( l_mspurgedGprs_c, mspurgedGPRS.getValue(), mspurgedGPRS.getOriginalLength());
	int l_mspurgedGprs_c_len = mspurgedGPRS.getOriginalLength();
	l_mspurgedGprs_c[sizeof(l_mspurgedGprs_c)-1] = '\0';
  
 
 /* memset(l_mspurgedNonGprs_c, '\0', sizeof(l_mspurgedNonGprs_c));
  memset(l_notreachableGsm_c, '\0', sizeof(l_notreachableGsm_c));
  memset(l_mnrf_c, '\0', sizeof(l_mnrf_c));
  memset(l_mcef_c, '\0', sizeof(l_mcef_c));
  memset(l_mnrr_c_gsm, '\0', sizeof(l_mnrr_c_gsm));
  memset(l_imsi_c, '\0', sizeof(l_imsi_c));
  memcpy(l_msisdn_c, "522222798068\0", 12);*/
  
  
  Usd.l_mnrf_c = mnrf.getValue();
  Usd.l_mcef_c = mcef.getValue();
  Usd.l_mnrr_c_gsm = mnrr_gsm.getValue();
  Usd.l_mnrg_c = mnrg.getValue();
  Usd.l_notreachableGsm_c = msNotReachableFlagForGsm.getValue();
  Usd.l_notreachableGprs_c = msNotReachableFlagForGprs.getValue();
  Usd.l_sgsnAddress_c = sgsnAddr_temp;
  Usd.l_sgsnNumber_c = sgsnNum_temp;
  Usd.l_imsi_c = l_imsi_c;
  Usd.l_msisdn_c = l_msisdn_c;
  Usd.l_lmsi_c = l_lmsi_c;
  Usd.l_mspurgedNonGprs_c = mspurgedNonGPRS.getValue();
  // Usd.l_mspurgedGprs_c = mspurgedGPRS.getValue();
  
  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0,"l_mspurgedNonGprs_c ===== %s|%s",Usd.l_mspurgedNonGprs_c,__FUNCTION__);
  
  Usd.loadObjectForUpDateData();
  cout<<"----------------------"<<endl;
  return CODE_SQL_SUCCESS;

	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "CHAR Imsi = %s| Len = %d |%s", l_imsi_c, l_imsi_c_len, __FUNCTION__);
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "CHAR Msisdn = %s| Len = %d |%s", l_msisdn_c, l_msisdn_c_len, __FUNCTION__);
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "CHAR lmsi = %s| Len = %d |%s", l_lmsi_c, l_lmsi_c_len, __FUNCTION__);
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "CHAR lmsi = %s| Len = %d |%s", l_mspurgedNonGprs_c, l_mspurgedNonGprs_c_len, __FUNCTION__);
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "CHAR mnrf = %s| Len = %d |%s", l_mnrf_c, l_mnrf_c_len, __FUNCTION__);
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "CHAR mcef = %s| Len = %d |%s", l_mcef_c, l_mcef_c_len, __FUNCTION__);
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "CHAR mnrr = %s| Len = %d |%s", l_mnrr_c_gsm, l_mnrr_c_gsm_len, __FUNCTION__);
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "CHAR mnrg = %s| Len = %d |%s", l_mnrg_c, l_mnrg_c_len, __FUNCTION__);
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "CHAR notreachableGsm = %s| Len = %d |%s", l_notreachableGsm_c, l_notreachableGsm_c_len, __FUNCTION__);
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "CHAR notreachableGprs = %s| Len = %d |%s", l_notreachableGprs_c, l_notreachableGprs_c_len, __FUNCTION__);
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "CHAR sgsnNumber = %s| Len = %d |%s", l_sgsnNumber_c, l_sgsnNumber_c_len, __FUNCTION__);
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "CHAR sgsnAddress = %s| Len = %d |%s", l_sgsnAddress_c, l_sgsnAddress_c_len, __FUNCTION__);



	if (obj->getHlrTrSubscriberInfoUpdated())
	{ 
		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Entering |%s", __FUNCTION__);
		if (obj->getVlrSupportsSuperCharger())
		{	
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Entering |%s", __FUNCTION__);
			queryIndex=1;	
			stmt = pDBHandler->GetStatement((conn = pDBHandler->GetConnection()),
					"UPDATE HLR_TR_SUB_GSM_INFO "
					"SET MSPURGED_NONGPRS_FLAG = :1, NOTREACHABLE_FLAG = :2, "
					"MNRF = :3, MCEF = :4, MNRR = :5 "
					"WHERE (IMSI = :6 OR MSISDN = :7)");

			stmt->setString(1, l_mspurgedNonGprs_c);
			stmt->setString(2, l_notreachableGsm_c);
			stmt->setString(3, l_mnrf_c);
			stmt->setString(4, l_mcef_c);
			stmt->setString(5, l_mnrr_c_gsm);
			stmt->setString(6, l_imsi_c);
			stmt->setString(7, l_msisdn_c);
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Entering |%s", __FUNCTION__);
			trrowsUpdated = stmt->executeUpdate();
			if (trrowsUpdated > 0)
			{   conn->commit();
				loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Successfully updated rows in HLR_TR_SUBSCRIBER_INFO table |%s", __FUNCTION__);
				pDBHandler->CloseStatement(stmt, conn);
			}
			if(trrowsUpdated=0)
			{
				loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "not updated rows in HLR_TR_SUBSCRIBER_INFO table |%s", __FUNCTION__);
				pDBHandler->CloseStatement(stmt, conn);
				goto STMT1;
			}

		}
		else
		{   queryIndex=2;
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Entering |%s", __FUNCTION__);
			stmt = pDBHandler->GetStatement((conn = pDBHandler->GetConnection()), 
					"UPDATE HLR_TR_SUB_GSM_INFO "
					"SET MNRF = :1, MCEF = :2, MNRR = :3 "
					"WHERE (IMSI = :4 OR MSISDN = :5)");

			stmt->setString(1, l_mnrf_c);
			stmt->setString(2, l_mcef_c);
			stmt->setString(3, l_mnrr_c_gsm);
			stmt->setString(4, l_imsi_c);
			stmt->setString(5, l_msisdn_c);
			gprsrowsUpdated=stmt->executeUpdate();
			loggerObj.log(LM_CRITICAL,__FILE__,__LINE__,0,0,"Getting DB connection succesfully|%s|",__FUNCTION__);

			if (gprsrowsUpdated > 0)
			{   
				conn->commit();
				pDBHandler->CloseStatement(stmt, conn);
				loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Successfully updated rows in HLR_TR_GPRS_INFO table |%s", __FUNCTION__);
			}
			if(gprsrowsUpdated=0)
			{
				loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "not updated rows in HLR_TR_GPRS_INFO table |%s", __FUNCTION__);
				pDBHandler->CloseStatement(stmt, conn);
				goto STMT1;
			}

		}
	}
STMT1:
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Entering |%s", __FUNCTION__);
	queryIndex = 3;
	stmt = pDBHandler->GetStatement((conn = pDBHandler->GetConnection()),
			"SELECT NVL(SGSN_SUPPORTS_SUPER_CHARGER, 0) FROM HLR_TR_SUB_GPRS_INFO "
			"WHERE IMSI = :1 OR MSISDN = :2");

	stmt->setString(1, l_imsi_c);  // Set IMSI
	stmt->setString(2, l_msisdn_c);  // Set MSISDN

	ResultSet *rs = stmt->executeQuery();  

	if (rs->next()) 
	{
		string l_superCharger = rs->getString(1); 
		strncpy(l_superCharger_c_gprs,l_superCharger.c_str(),sizeof(l_superCharger_c_gprs)-1);
		pDBHandler->Close(rs, stmt, conn);
	}
	else
	{ 
		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "no data found at queryindex %d |%s",queryIndex, __FUNCTION__);
		pDBHandler->Close(rs, stmt, conn);
		goto STMT2;
	}

	if (obj->getHlrTrGprsProfileUpdated())

	{

		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Entering |%s", __FUNCTION__);
		if (obj->getSgsnSupportsSuperCharger())  // If SGSN supports SuperCharger
		{
			queryIndex = 4;
			stmt = pDBHandler->GetStatement((conn = pDBHandler->GetConnection()),
					"UPDATE HLR_TR_SUB_GPRS_INFO "
					"SET MSPURGED_FOR_GPRS = :1, NOTREACHABLE_GPRS_FLAG = :2, "
					"MNRG = :3, MNRR = :4, MCEF = :5 "
					"WHERE IMSI = :6 OR MSISDN = :7");

			stmt->setString(1, l_mspurgedGprs_c);
			stmt->setString(2, l_notreachableGprs_c);
			stmt->setString(3, l_mnrg_c);
			stmt->setString(4, l_mnrr_c_gprs);
			stmt->setString(5, l_mcef_c);
			stmt->setString(6, l_imsi_c);
			stmt->setString(7, l_msisdn_c);

			gprsrowsUpdated = stmt->executeUpdate();


			// Log the successful update for SuperCharger case
			if (gprsrowsUpdated > 0)
			{
				conn->commit();
				loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Successfully updated rows in HLR_TR_SUB_GPRS_INFO with SuperCharger data |%s", __FUNCTION__);
				pDBHandler->CloseStatement(stmt, conn);
			}
			else
			{
				loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Not updated rows in HLR_TR_SUB_GPRS_INFO with SuperCharger data |%s", __FUNCTION__);
				pDBHandler->CloseStatement(stmt, conn);
				goto STMT2;
			}

		}
		else  // If SGSN does not support SuperCharger
		{
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Entering |%s", __FUNCTION__);
			queryIndex = 5;
			stmt = pDBHandler->GetStatement((conn = pDBHandler->GetConnection()),
					"UPDATE HLR_TR_SUB_GPRS_INFO "
					"SET MNRG = :1, MNRR = :2, MCEF = :3 "
					"WHERE IMSI = :4 OR MSISDN = :5");

			stmt->setString(1, l_mnrg_c);
			stmt->setString(2, l_mnrr_c_gprs);
			stmt->setString(3, l_mcef_c);
			stmt->setString(4, l_imsi_c);
			stmt->setString(5, l_msisdn_c);

			gprsrowsUpdated = stmt->executeUpdate();


			// Log the successful update for non-SuperCharger case
			if (gprsrowsUpdated > 0)
			{   conn->commit();
				loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Successfully updated rows in HLR_TR_SUB_GPRS_INFO without SuperCharger data |%s", __FUNCTION__);
				pDBHandler->CloseStatement(stmt, conn);
			}
			if(gprsrowsUpdated=0)
			{
				loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Not updated rows in HLR_TR_SUB_GPRS_INFO without SuperCharger data |%s", __FUNCTION__);
				pDBHandler->CloseStatement(stmt, conn);
				goto STMT2;
			}
		}
	}

STMT2:
	if ( (obj->getHlrTrSubscriberInfoUpdated()) || (obj->getHlrTrGprsProfileUpdated()) )
	{  
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "Commited successfully |%s",__FUNCTION__);
	}
	else{ 
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "Both the Flags are FALSE |%s",__FUNCTION__);
	}
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving updateSmsInfo |%s",__FUNCTION__);
	return CODE_SQL_SUCCESS;
}

catch (SQLException& ex) {
	std::cerr << "SQL Error: " << ex.getMessage() << std::endl;
	loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Oracle Error code and Message |%d|%s|%s", ex.getErrorCode(), ex.getMessage().c_str(), __FUNCTION__); 
	loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Oracle SQL_ERROR_OCCURED  |%s", __FUNCTION__);
	pDBHandler->CloseStatement(stmt, conn);
	return PROC_CODE_ERROR;

}

catch(...)
{

	loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Caught Exception  |%s", __FUNCTION__);
	loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Leaving updateSmsInfo  |%s", __FUNCTION__);    
	pDBHandler->CloseStatement(stmt, conn);
	return CODE_EXCEPTION;
}
}

/*ESqlRes SmsmDbService::updateSmsInfo(UpdateDb *obj) //xrp-2049
{
	loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, 
			"Entering updateSmsInfo |%s", __FUNCTION__);

	try
	{
		BCDEncoder BCDEncoderobj;

		if (obj == NULL) 
		{   
			loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0,
					"obj is null |%s", __FUNCTION__);
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"Leaving updateSmsInfo |%s", __FUNCTION__);
			return CODE_EXCEPTION;
		}

		char imsi_temp[IMSI_SIZE + 1];
		char msisdn_temp[MSISDN_SIZE + 1];

		nNumber imsi = obj->getImsi();
		int imsi_len = imsi.getOriginalLength();

		nNumber msisdn = obj->getMsisdn();
		int msisdn_len = msisdn.getOriginalLength();

		if (imsi_len <= 0 && msisdn_len <= 0 || imsi_len > IMSI_SIZE / 2 || msisdn_len > MSISDN_SIZE / 2)
		{
			loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0,
					"IMSI and MSISDN check failed |%s", __FUNCTION__);
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"Leaving updateSmsInfo |%s", __FUNCTION__);
			return CODE_EXCEPTION;
		}

		memset(imsi_temp, '\0', sizeof(imsi_temp));
		memset(msisdn_temp, '\0', sizeof(msisdn_temp));
		memcpy(imsi_temp, imsi.getValue(), imsi_len);
		memcpy(msisdn_temp, msisdn.getValue(), msisdn_len);

		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
				"IMSI Len = %d |%s", imsi_len, __FUNCTION__);
		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
				"MSISDN Len = %d |%s", msisdn_len, __FUNCTION__);

		// Convert BCD to char for IMSI and MSISDN
		char l_imsi_c[IMSI_SIZE + 1] = {0};
		char l_msisdn_c[MSISDN_SIZE + 1] = {0};

		int l_imsi_c_len = BCDEncoderobj.BCDToChar(l_imsi_c, imsi_temp, imsi_len);
		l_imsi_c[sizeof(l_imsi_c) - 1] = '\0';

		int l_msisdn_c_len = BCDEncoderobj.BCDToChar(l_msisdn_c, msisdn_temp, msisdn_len);
		l_msisdn_c[sizeof(l_msisdn_c) - 1] = '\0';

		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
				"CHAR IMSI = %s | Len = %d |%s", l_imsi_c, l_imsi_c_len, __FUNCTION__);
		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
				"CHAR MSISDN = %s | Len = %d |%s", l_msisdn_c, l_msisdn_c_len, __FUNCTION__);

		// Prepare other parameters
		char l_lmsi_c[LMSI_SIZE + 1] = {0};
		char l_mnrg_c[MNRG_SIZE + 1] = {0};
		char l_mnrf_c[MNRF_SIZE + 1] = {0};
		char l_mnrr_c_gsm[MNRR_SIZE + 1] = {0};
		char l_mnrr_c_gprs[MNRR_SIZE + 1] = {0};
		char l_mcef_c[MCEF_SIZE + 1] = {0};
		char l_sgsnAddress_c[SGSNADDR_SIZE + 1] = {0};
		char l_sgsnNumber_c[SGSNNUM_SIZE + 1] = {0};
		char l_notreachableGsm_c[NOTREACHABLE_SIZE + 1] = {0};
		char l_notreachableGprs_c[NOTREACHABLE_SIZE + 1] = {0};
		char l_mspurgedNonGprs_c[MSPURGENONGPRS_SIZE + 1] = {0};
		char l_mspurgedGprs_c[MSPURGEGPRS_SIZE + 1] = {0};
		char l_superCharger_c_gprs[SUPERCHARGERSUPPORTSINSGSN_SIZE + 1] = {0};

		// Copy values from obj to local variables
		nNumber lmsi = obj->getLmsi();
		memcpy(l_lmsi_c, lmsi.getValue(), lmsi.getOriginalLength());

		nNumber mnrf = obj->getMnrf();
		memcpy(l_mnrf_c, mnrf.getValue(), mnrf.getOriginalLength());

		nNumber mcef = obj->getMcef();
		memcpy(l_mcef_c, mcef.getValue(), mcef.getOriginalLength());

		nNumber mnrr_gsm = obj->getMnrrGsm();
		memcpy(l_mnrr_c_gsm, mnrr_gsm.getValue(), mnrr_gsm.getOriginalLength());

		nNumber mnrg = obj->getMnrg();
		memcpy(l_mnrg_c, mnrg.getValue(), mnrg.getOriginalLength());

		nNumber msNotReachableFlagForGsm = obj->getMsNotReachableFlagForGsm();
		memcpy(l_notreachableGsm_c, msNotReachableFlagForGsm.getValue(), msNotReachableFlagForGsm.getOriginalLength());

		nNumber msNotReachableFlagForGprs = obj->getMsNotReachableFlagForGprs();
		memcpy(l_notreachableGprs_c, msNotReachableFlagForGprs.getValue(), msNotReachableFlagForGprs.getOriginalLength());

		nNumber mspurgedNonGPRS = obj->getMsPurgedNonGprsFlag();
		memcpy(l_mspurgedNonGprs_c, mspurgedNonGPRS.getValue(), mspurgedNonGPRS.getOriginalLength());

		nNumber mspurgedGPRS = obj->getMsPurgedGPRSFlag();
		memcpy(l_mspurgedGprs_c, mspurgedGPRS.getValue(), mspurgedGPRS.getOriginalLength());

		// Convert SGSN address and number
		char sgsnAddr_temp[SGSNADDR_SIZE + 1] = {0};
		nNumber sgsnaddr = obj->getSgsnAddress();
		int sgsnaddrlen = sgsnaddr.getOriginalLength();
		memcpy(sgsnAddr_temp, sgsnaddr.getValue(), sgsnaddrlen);
		BCDEncoderobj.BCDToChar(l_sgsnAddress_c, sgsnAddr_temp, sgsnaddrlen);

		char sgsnNum_temp[SGSNNUM_SIZE + 1] = {0};
		nNumber sgsnnum = obj->getSgsnNumber();
		int sgsnlen = sgsnnum.getOriginalLength();
		memcpy(sgsnNum_temp, sgsnnum.getValue(), sgsnlen);
		BCDEncoderobj.BCDToChar(l_sgsnNumber_c, sgsnNum_temp, sgsnlen);

		bool updated = false;

		// Update HLR_TR_SUB_GSM_INFO if required
		if (obj->getHlrTrSubscriberInfoUpdated())
		{
			std::string gsmUpdateQuery;
			if (obj->getVlrSupportsSuperCharger())
			{
				gsmUpdateQuery =
					"UPDATE hlrdb_qc.HLR_TR_SUB_GSM_INFO "
					"SET MSPURGED_NONGPRS_FLAG = '" + std::string(l_mspurgedNonGprs_c) + "', "
					"NOTREACHABLE_FLAG = '" + std::string(l_notreachableGsm_c) + "', "
					"MNRF = '" + std::string(l_mnrf_c) + "', "
					"MCEF = '" + std::string(l_mcef_c) + "', "
					"MNRR = '" + std::string(l_mnrr_c_gsm) + "' "
					"WHERE (IMSI = '" + std::string(l_imsi_c) + "' OR MSISDN = '" + std::string(l_msisdn_c) + "')";
			}
			else
			{
				gsmUpdateQuery =
					"UPDATE hlrdb_qc.HLR_TR_SUB_GSM_INFO "
					"SET MNRF = '" + std::string(l_mnrf_c) + "', "
					"MCEF = '" + std::string(l_mcef_c) + "', "
					"MNRR = '" + std::string(l_mnrr_c_gsm) + "' "
					"WHERE (IMSI = '" + std::string(l_imsi_c) + "' OR MSISDN = '" + std::string(l_msisdn_c) + "')";
			}

			DbUtil::executeNonQuery(gsmUpdateQuery);
			updated = true;
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"Successfully updated HLR_TR_SUB_GSM_INFO |%s", __FUNCTION__);
		}

		// Check SGSN SuperCharger support
		std::string superChargerQuery =
			"SELECT NVL(SGSN_SUPPORTS_SUPER_CHARGER, '0') AS super_charger "
			"FROM hlrdb_qc.HLR_TR_SUB_GPRS_INFO "
			"WHERE IMSI = '" + std::string(l_imsi_c) + "' OR MSISDN = '" + std::string(l_msisdn_c) + "'";

		DbUtil::ResultSet rs = DbUtil::executeQuery(superChargerQuery);

		if (!rs.empty())
		{
			string l_superCharger = rs[0].at("super_charger");
			strncpy(l_superCharger_c_gprs, l_superCharger.c_str(), sizeof(l_superCharger_c_gprs) - 1);
			l_superCharger_c_gprs[sizeof(l_superCharger_c_gprs) - 1] = '\0';
		}

		// Update HLR_TR_SUB_GPRS_INFO if required
		if (obj->getHlrTrGprsProfileUpdated())
		{
			std::string gprsUpdateQuery;
			if (obj->getSgsnSupportsSuperCharger())
			{
				gprsUpdateQuery =
					"UPDATE hlrdb_qc.HLR_TR_SUB_GPRS_INFO "
					"SET MSPURGED_FOR_GPRS = '" + std::string(l_mspurgedGprs_c) + "', "
					"NOTREACHABLE_GPRS_FLAG = '" + std::string(l_notreachableGprs_c) + "', "
					"MNRG = '" + std::string(l_mnrg_c) + "', "
					"MNRR = '" + std::string(l_mnrr_c_gprs) + "', "
					"MCEF = '" + std::string(l_mcef_c) + "' "
					"WHERE IMSI = '" + std::string(l_imsi_c) + "' OR MSISDN = '" + std::string(l_msisdn_c) + "'";
			}
			else
			{
				gprsUpdateQuery =
					"UPDATE hlrdb_qc.HLR_TR_SUB_GPRS_INFO "
					"SET MNRG = '" + std::string(l_mnrg_c) + "', "
					"MNRR = '" + std::string(l_mnrr_c_gprs) + "', "
					"MCEF = '" + std::string(l_mcef_c) + "' "
					"WHERE IMSI = '" + std::string(l_imsi_c) + "' OR MSISDN = '" + std::string(l_msisdn_c) + "'";
			}

			DbUtil::executeNonQuery(gprsUpdateQuery);
			updated = true;
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"Successfully updated HLR_TR_SUB_GPRS_INFO |%s", __FUNCTION__);
		}

		if (updated)
		{
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"Committed successfully |%s", __FUNCTION__);
		}
		else
		{
			loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
					"No updates performed |%s", __FUNCTION__);
		}

		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
				"Leaving updateSmsInfo |%s", __FUNCTION__);
		return CODE_SQL_SUCCESS;
	}
	catch (const std::exception& ex)
	{
		loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0,
				"Exception occurred in updateSmsInfo: Message [%s] |%s",
				ex.what(), __FUNCTION__);
		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,
				"Leaving updateSmsInfo |%s", __FUNCTION__);
		return CODE_EXCEPTION;
	}
}*/

ESqlRes SmsmDbService::updateMwdList(void* a,nNumber &msisdn, nNumber &scAddress, nNumber &status)
{
loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Entering updateMwdList  |%s", __FUNCTION__);
	BCDEncoder BCDEncoderobj;
	DBConnHandler* pDBHandler = DBConnHandler::GetInstance();
	Connection*    conn;
	Statement* stmt;
	ResultSet* rs; 
	char msisdn_temp[MSISDN_SIZE +1];
	int msisdn_len = msisdn.getOriginalLength();
	memset( msisdn_temp, '\0', sizeof(msisdn_temp) );
	if( msisdn_len <= 0 || msisdn_len > MSISDN_SIZE / 2)
	{

		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "MSISDN check failed  |%s", __FUNCTION__);
		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Leaving updateMwdList  |%s", __FUNCTION__); 
		return CODE_EXCEPTION;	
	}
	memcpy(msisdn_temp,  msisdn.getValue(), msisdn_len);
	loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "MSISDN Len = %d  |%s", __FUNCTION__);
	char scaddr_temp[SCADDRESS_SIZE +1] ;
	int scaddr_len = scAddress.getOriginalLength();
	memset( scaddr_temp, '\0', sizeof(scaddr_temp) );
	if( scaddr_len <= 0 || scaddr_len > SCADDRESS_SIZE / 2)
	{
		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "SCAddress check failed  |%s", __FUNCTION__);
		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Leaving updateMwdList  |%s", __FUNCTION__);
		return CODE_EXCEPTION;	
	}
	memcpy(scaddr_temp, scAddress.getValue(), scaddr_len);
	loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "SCAddress Len = %d  |%s",scaddr_len, __FUNCTION__);
	char		l_msisdn_c[MSISDN_SIZE +1];
	char		l_scAddress_c[SCADDRESS_SIZE +1];
	char		l_status_c[STATUS_SIZE +1];
	//char 		l_status;
	int 		  l_count;
	memset(&l_msisdn_c,	'\0', sizeof(l_msisdn_c));
	memset(&l_scAddress_c,'\0', sizeof(l_scAddress_c));
	memset(&l_status_c,	'\0', sizeof(l_status_c));
	l_count = 0;
	int queryIndex=0;
	int index2,index3,index4,index5=0;
	try
	{

		int l_msisdn_c_len = BCDEncoderobj.BCDToChar(l_msisdn_c ,msisdn_temp, msisdn_len);
		l_msisdn_c[sizeof(l_msisdn_c)-1] = '\0';

		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "  MSISDN=%s and MSISDN Len = %d |%s",msisdn_temp,l_msisdn_c_len,__FUNCTION__);

		int l_scAddress_c_len  = BCDEncoderobj.BCDToChar( l_scAddress_c ,scaddr_temp, scaddr_len);
		l_scAddress_c[sizeof(l_scAddress_c)-1] = '\0';

		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " CHAR SCADDR = %s| Len = %d |%s",l_scAddress_c,l_scAddress_c_len,__FUNCTION__);

		memcpy(l_status_c ,status.getValue(), status.getOriginalLength());
		l_status_c[status.getOriginalLength()] = '\0';
		int l_status_c_len = status.getOriginalLength();

		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "  l_status_c = %s and length %d |%s",l_status_c,l_status_c_len,__FUNCTION__);

		memcpy(scaddr_temp, scAddress.getValue(), scaddr_len);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "  SCAddress Len = %d |%s", scaddr_len,__FUNCTION__);
		queryIndex = 1;
		stmt = pDBHandler->GetStatement((conn = pDBHandler->GetConnection()),
				"SELECT COUNT(1) FROM HLR_TR_MWD_INFO WHERE MSISDN = :1 AND SVC_CENTRE_ADDRESS = :2");
		stmt->setString(1, l_msisdn_c);
		stmt->setString(2, l_scAddress_c);
		rs = stmt->executeQuery();

		if (rs->next()) {
			l_count = rs->getInt(1);
		}
		if (l_count <=0) {
			loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "no rows updated at queryIndex = %d | %s", queryIndex, __FUNCTION__);
			pDBHandler->Close(rs,stmt,conn);
			goto DATA_NOT_FOUND;
		}
		else
		{
			loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "count is = %d at queryIndex = %d | %s", l_count,queryIndex, __FUNCTION__);
			pDBHandler->Close(rs,stmt,conn);
		}

		if (l_count <= 0 && l_status_c[0] == 'N') {
			queryIndex = 2;
			stmt = pDBHandler->GetStatement((conn = pDBHandler->GetConnection()),
					"INSERT INTO HLR_TR_MWD_INFO (MSISDN, SVC_CENTRE_ADDRESS, STATUS) VALUES (:1, :2, :3)");
			stmt->setString(1, l_msisdn_c);
			stmt->setString(2, l_scAddress_c);
			stmt->setString(3, l_status_c);
			index2 = stmt->executeUpdate();

			if (index2 == 0) {
				loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "no rows updated at queryIndex = %d | %s", queryIndex, __FUNCTION__);
				pDBHandler->CloseStatement(stmt, conn);
				goto DATA_NOT_FOUND;
			}
			else
			{   loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, " rows updated at queryIndex = %d | %s", queryIndex, __FUNCTION__);
				conn->commit();
				pDBHandler->CloseStatement(stmt, conn);
			}

			if (l_count >= 1) {
				queryIndex = 3;
				stmt = pDBHandler->GetStatement((conn = pDBHandler->GetConnection()),
						"UPDATE HLR_TR_MWD_INFO SET STATUS = :1 WHERE MSISDN = :2 AND SVC_CENTRE_ADDRESS = :3");
				stmt->setString(1, l_status_c);
				stmt->setString(2, l_msisdn_c);
				stmt->setString(3, l_scAddress_c);
				index3 = stmt->executeUpdate();

				if (index3 == 0) {
					loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "no rows affected at queryIndex = %d | %s", queryIndex, __FUNCTION__);
					pDBHandler->CloseStatement(stmt, conn);
					goto DATA_NOT_FOUND;
				}
				else
				{   loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, " rows updated at queryIndex = %d | %s", queryIndex, __FUNCTION__);
					conn->commit();
					pDBHandler->CloseStatement(stmt, conn);
				}

			} else {
				queryIndex = 4;
				stmt = pDBHandler->GetStatement((conn = pDBHandler->GetConnection()),
						"INSERT INTO HLR_TR_MWD_INFO(MSISDN, SVC_CENTRE_ADDRESS, STATUS, LAST_UPDATED_DATE) VALUES(:1, :2, :3, SYSDATE)");
				stmt->setString(1, l_msisdn_c);
				stmt->setString(2, l_scAddress_c);
				stmt->setString(3, l_status_c);
				index4 = stmt->executeUpdate();

				if (index4 == 0) {
					loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "no rows affected at queryIndex = %d | %s", queryIndex, __FUNCTION__);
					pDBHandler->CloseStatement(stmt, conn);
					goto DATA_NOT_FOUND;
				}
				else
				{   loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, " rows updated at queryIndex = %d | %s", queryIndex, __FUNCTION__);
					conn->commit();
					pDBHandler->CloseStatement(stmt, conn);
				}
			}
		}

		if (l_count && (l_status_c[0] == 'D')) {
			queryIndex = 5;
			stmt = pDBHandler->GetStatement((conn = pDBHandler->GetConnection()),
					"UPDATE HLR_TR_MWD_INFO SET STATUS = :1, LAST_UPDATED_DATE = SYSDATE WHERE MSISDN = :2 AND SVC_CENTRE_ADDRESS = :3");
			stmt->setString(1, l_status_c);
			stmt->setString(2, l_msisdn_c);
			stmt->setString(3, l_scAddress_c);
			index5 = stmt->executeUpdate();


			if (index5 == 0) {
				loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "no rows affected at queryIndex = %d | %s", queryIndex, __FUNCTION__);
				pDBHandler->CloseStatement(stmt, conn);
				goto DATA_NOT_FOUND;
			}
			else
			{   loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, " rows updated at queryIndex = %d | %s", queryIndex, __FUNCTION__);
				conn->commit();
				pDBHandler->CloseStatement(stmt, conn);
			}
		}
		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "STATUS UPDATED | %s", __FUNCTION__);
		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Leaving updateMwdList | %s", __FUNCTION__); 
		return CODE_SQL_SUCCESS;


	}
	catch (SQLException& ex) {
		std::cerr << "SQL Error: " << ex.getMessage() << std::endl;
		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Oracle Error code and Message |%d|%s|%s", ex.getErrorCode(), ex.getMessage().c_str(), __FUNCTION__); 
		pDBHandler->CloseStatement(stmt, conn);
		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Oracle SQL_ERROR_OCCURED  |%s", __FUNCTION__);
		return PROC_CODE_ERROR;

	}
	catch(...)
	{
		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Caught Exception  |%s", __FUNCTION__);
		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Leaving updateMwdList  |%s", __FUNCTION__);
		pDBHandler->Close(rs, stmt, conn);
		return CODE_EXCEPTION;
	}
DATA_NOT_FOUND: 
	{
		loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0, " no rows founded at queryIndex  =%d |%s",queryIndex,__FUNCTION__);
		return ROWS_NOT_FOUND;  
	}
}

ESqlRes SmsmDbService::getSmsInfo(DataFromDb *obj,int trGprsTeledeleteFlag) {
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Entering getSmsInfo |%s",__FUNCTION__);

	unsigned long m_Index=1;
	BCDEncoder BCDEncoderobj;

	char imsi_temp[IMSI_SIZE +1] ;
	char msisdn_temp[MSISDN_SIZE +1] ;

	nNumber imsi = obj->getImsi();
	int imsi_len =  imsi.getOriginalLength();

	nNumber msisdn=obj->getMsisdn();
	int msisdn_len =  msisdn.getOriginalLength();


	if ( imsi_len <= 0 && msisdn_len <=0  || imsi_len > IMSI_SIZE / 2 || msisdn_len > MSISDN_SIZE / 2 )
	{
		loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0, " IMSI and MSISDN check failed |%s",__FUNCTION__  );
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving getSmsInfo |%s",__FUNCTION__);
		return CODE_EXCEPTION;
	}

	memset(imsi_temp,  '\0',  sizeof(imsi_temp) );
	memset(msisdn_temp, '\0', sizeof(msisdn_temp) );

	memcpy(imsi_temp, imsi.getValue(), imsi_len);
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " BCD IMSI Len = %d|%s",imsi_len,__FUNCTION__);

	memcpy(msisdn_temp,  msisdn.getValue(), msisdn_len);
	loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " BCD MSISDN Len = %d|%s",msisdn_len,__FUNCTION__);

	int count_test=0;
	int 			l_profileid_i ;
	int 			l_cnt_i ;
	int       l_cnt_gprs_i;
	int		  	l_count_i;
	int			  l_color_i;
	int                    ipstatus; //added by Anand
	char               l_gcsf_a[MSISDN_SIZE +1];
	char		l_vlrNumber_c[VLR_SIZE +1];
	char		l_imsi_c[IMSI_SIZE +1];
	char		l_msisdn_c[MSISDN_SIZE +1];
	char		l_lmsi_c[LMSI_SIZE +1];
	char		l_mnrf_c[MNRF_SIZE + 1] ; 
	int		l_mcef_c;
	int		l_mcef_c_gprs;
	char		l_mnrr_c_gsm[MNRR_SIZE +1] = {'\0'};
	char		l_mnrr_c_gprs[MNRR_SIZE +1]= {'\0'};
	char		l_mscSupportsMtSms_c[MSCSUPMTSMS_SIZE +1];
	char		l_mscNumber_c[MSCNUM_SIZE +1];
	char		l_notreachableGsm_c[NOTREACHABLE_SIZE +1];
	char   l_msNotReachableInMscStatus[NOTREACHABLE_SIZE +1];//mohan added Mar15 2009
	char   l_mspurged_nongprs_flag[NOTREACHABLE_SIZE +1];//mohan added Mar15 2009
	char		l_notreachableGprs_c[NOTREACHABLE_SIZE +1];
	char   l_msNotReachableInSgsnStatus[NOTREACHABLE_SIZE +1];//mohan added Mar15 2009
	char   l_mspurged_gprs_flag[NOTREACHABLE_SIZE +1];//mohan added Mar15 2009
	char		l_sgsnNumber_c[SGSNNUM_SIZE + 1];
	char		l_sgsnSupportsMtSms_c[SGSNSUPMTSMS_SIZE +1];
	char		l_sgsnAddress_c[SGSNADDR_SIZE +1];
	char		l_mnrg_c[MNRG_SIZE +1];
	char		l_networkAccessMode_c[NTACCESSMODE_SIZE +1];
	char		l_ssbarring_c[SSBARR_SIZE +1];
	char   l_mtodbBarring_value[OPERBARR_SIZE];
	char   l_vlrNumberPresent_value[VLR_SIZE];
	char   l_sgsnNumberPresent_value[SGSNNUM_SIZE];
	char   l_substateActiveGsm_value[GSM_ACTIVE_STATE_SIZE];
	char   l_substateActiveGprs_value[GPRS_ACTIVE_STATE_SIZE];
	char   l_operatorBarringGsm_c[OPERBARR_SIZE+1];
	char   l_operatorBarringGprs_c[OPERBARR_SIZE]; //spell-2.0.13.1
	char		l_mtSmsProvisioned_c[MTSMSPRO_SIZE +1];
	char		l_imsi[IMSI_SIZE +1];
	char		l_msisdn[MSISDN_SIZE +1];
	unsigned int 		l_operatorBarring_value; //actually it is of size 10
	char			l_subscriber_state;
	char			l_trace_required[2];
	char		l_subscriber_state_gsm_c[GSM_ACTIVE_STATE_SIZE +1];
	char		l_subscriber_state_gprs_c[GPRS_ACTIVE_STATE_SIZE +1];

	char			l_trace_required_gsm[20];
	char			l_trace_required_gprs[2];
	unsigned int           l_gsm_active_roaming_subs_profile_id = 0; //2.0.10.0 - Roaming profile change
	unsigned int           l_gprs_active_roaming_subs_profile_id = 0; //2.0.10.0 - Roaming profile change
	char		 l_NetworkId[IMSI_SIZE] = {'\0'};
	char		 l_hlr_c[IMSI_SIZE] = {'\0'};
	char		 l_smsc_c[IMSI_SIZE] = {'\0'};
	char    l_roaming_status[3]  = {'\0'};
	char    l_gprs_roaming_status[3]  = {'\0'};
	char		l_superCharger_c_gsm[SUPERCHARGERSUPPORTSINVLR_SIZE +1] = {'\0'}; //spell-2100
	char		l_superCharger_c_gprs[SUPERCHARGERSUPPORTSINSGSN_SIZE +1] = {'\0'};
	//2.0.13.1 Begin

	unsigned int l_gprsOdbinducedBarring;		

	int tr_gsm_row_count = 0;
	int tr_gprs_row_count = 0;
	int queryIndex=0;
	memset(&l_vlrNumber_c,  '\0',sizeof(l_vlrNumber_c) );
	memset(&l_imsi_c,  '\0',  sizeof(l_imsi_c) );
	memset(&l_msisdn_c, '\0', sizeof(l_msisdn_c) );
	memset(&l_lmsi_c, '\0',   sizeof(l_lmsi_c) );
	memset(&l_mnrg_c,  '\0',  sizeof(l_mnrg_c) );
	memset(&l_mnrf_c,  '\0',  sizeof(l_mnrf_c) );
	l_mcef_c=0;
	l_mcef_c_gprs=0;
	memset(&l_sgsnAddress_c,  '\0',  sizeof(l_sgsnAddress_c));
	memset(&l_sgsnNumber_c,  '\0',  sizeof(l_sgsnNumber_c));
	memset(&l_mscSupportsMtSms_c,  '\0',  sizeof(l_mscSupportsMtSms_c));
	memset(&l_mscNumber_c,  '\0',  sizeof(l_mscNumber_c));
	memset(&l_gcsf_a,'\0',sizeof(l_gcsf_a));
	memset(&l_notreachableGsm_c,  '\0',  sizeof(l_notreachableGsm_c));
	memset(&l_msNotReachableInMscStatus,  '\0',  sizeof(l_msNotReachableInMscStatus));
	memset(&l_mspurged_nongprs_flag,  '\0',  sizeof(l_mspurged_nongprs_flag));
	memset(&l_notreachableGprs_c,  '\0',  sizeof(l_notreachableGprs_c));
	memset(&l_msNotReachableInSgsnStatus,  '\0',  sizeof(l_msNotReachableInSgsnStatus));
	memset(&l_mspurged_gprs_flag,  '\0',  sizeof(l_mspurged_gprs_flag));
	memset(&l_sgsnSupportsMtSms_c,  '\0',  sizeof(l_sgsnSupportsMtSms_c));
	memset(&l_networkAccessMode_c,  '\0',  sizeof(l_networkAccessMode_c));
	memset(&l_ssbarring_c,'\0',sizeof(l_ssbarring_c));
	memset(&l_mtodbBarring_value,'\0',sizeof(l_mtodbBarring_value)); //2.0.13.1_Rev1
	memset(&l_vlrNumberPresent_value,'\0',sizeof(l_vlrNumberPresent_value)); //2100
	memset(&l_sgsnNumberPresent_value,'\0',sizeof(l_sgsnNumberPresent_value)); //2100

	memset(&l_substateActiveGsm_value,'\0',sizeof(l_substateActiveGsm_value)); //2i.0.15.4
	memset(&l_substateActiveGprs_value,'\0',sizeof(l_substateActiveGprs_value)); //2.0.16.0
	memset(&l_operatorBarringGsm_c,  '\0',  sizeof(l_operatorBarringGsm_c));
	memset(&l_operatorBarringGprs_c,  '\0',  sizeof(l_operatorBarringGprs_c));
	memset(&l_mtSmsProvisioned_c,  '\0',  sizeof(l_mtSmsProvisioned_c));
	memset(&l_imsi,  '\0',  sizeof(l_imsi) );
	memset(&l_msisdn, '\0', sizeof(l_msisdn) );
	memset(&l_subscriber_state_gsm_c,  '\0',  sizeof(l_subscriber_state_gsm_c));
	memset(&l_subscriber_state_gprs_c,  '\0',  sizeof(l_subscriber_state_gprs_c));
	memset(&l_superCharger_c_gsm, '\0', sizeof(l_superCharger_c_gsm));
	memset(&l_superCharger_c_gprs, '\0', sizeof(l_superCharger_c_gprs));

	l_subscriber_state = '\0';


	l_operatorBarring_value = 0;
	l_color_i = 0;
	l_profileid_i  = 0;
	l_count_i = 0;
	l_cnt_i   =0;
	l_cnt_gprs_i = 0;
	nNumber subscriberState(30);
	bool flag_gsm=false;
	bool isJumpTrue=false;
	bool isgsmSubscriberiActive=false;
	bool isgprsSubscriberiActive=false;
	//l_gsmOdbinducedBarring = 0;		
	l_gprsOdbinducedBarring = 0;		

	DBConnHandler* pDBHandler = DBConnHandler::GetInstance();
	Connection*    conn;
	Statement* stmt;
	ResultSet* rs; 
	int index1,index2,index3,index4,index5,index6,index7,index8,index9,index10,index11,index12,index13,index14,index15,index16,index17=0;

	try
	{
		int l_imsi_c_len = BCDEncoderobj.BCDToChar(l_imsi_c ,imsi_temp, imsi_len);
		l_imsi_c[sizeof(l_imsi_c)-1] = '\0';
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " CHAR IMSI = %s| Len = %d |%s",l_imsi_c,l_imsi_c_len,__FUNCTION__);

		int l_msisdn_c_len = BCDEncoderobj.BCDToChar(l_msisdn_c, &msisdn_temp[1], msisdn_len-1);
		l_msisdn_c[sizeof(l_msisdn_c)-1] = '\0';
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " CHAR MSISDN = %s| and MSISDN Len = %d |%s",l_msisdn_c,l_msisdn_c_len,__FUNCTION__);

		memset(l_imsi_c, '\0' ,strlen(l_imsi_c)); //TODO: Remove line after BCDtoChar encoding is fixed.
		memcpy(l_msisdn_c, "522222798068\0", 12); //TODO: Remove line after BCDtoChar encoding is fixed.
		Subscriber subscriberInfo(l_imsi_c, l_msisdn_c);
		subscriberInfo.loadObjectFromDB();
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "SubscriberInfo: %s|%s",subscriberInfo.toString().c_str(),__FUNCTION__);
		return CODE_SQL_SUCCESS;

		getMtSubGsmInfo(l_imsi_c, l_msisdn_c, l_gsm_active_roaming_subs_profile_id);

GPRS_QUERY:

		loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0, " Active_roaming_profile_id for GPRS is =%d |%s",l_gprs_active_roaming_subs_profile_id,__FUNCTION__);
		queryIndex=5;
		stmt = pDBHandler->GetStatement(
				(conn = pDBHandler->GetConnection()),
				"SELECT NVL(IMSI, 0), NVL(MSISDN, 0), NVL(SUBSCRIBER_STATE, 0), "
				"NVL(TRACE_REQUIRED, 0), NVL(NETWORK_ACCESS_MODE, 0), NVL(NETWORK_ID, 0) "
				"FROM HLR_MT_SUB_GPRS_INFO "
				"WHERE (IMSI = :1 OR MSISDN = :2) AND ACTIVE_ROAMING_PROFILE_ID = :3");

		stmt->setString(1, l_imsi_c); 
		stmt->setString(2, l_msisdn_c);  
		stmt->setInt(3, l_gprs_active_roaming_subs_profile_id); 

		rs = stmt->executeQuery();


		if (rs->next()) {
			string l_imsi_result = rs->getString(1); 
			strncpy(l_imsi, l_imsi_result.c_str(), sizeof(l_imsi) - 1); 
			l_imsi[sizeof(l_imsi) - 1] = '\0';  

			string l_msisdn_result = rs->getString(2); 
			strncpy(l_msisdn, l_msisdn_result.c_str(), sizeof(l_msisdn) - 1);  
			l_msisdn[sizeof(l_msisdn) - 1] = '\0';  
			string l_subscriber_state_gprs_result = rs->getString(3);
			strncpy(l_subscriber_state_gprs_c, l_subscriber_state_gprs_result.c_str(), sizeof(l_subscriber_state_gprs_c) - 1);  
			l_subscriber_state_gprs_c[sizeof(l_subscriber_state_gprs_c) - 1] = '\0';  

			string l_trace_required_gprs_result = rs->getString(4);
			strncpy(l_trace_required_gprs, l_trace_required_gprs_result.c_str(), sizeof(l_trace_required_gprs) - 1); 
			l_trace_required_gprs[sizeof(l_trace_required_gprs) - 1] = '\0';
			string l_networkAccessMode_result = rs->getString(5);
			strncpy(l_networkAccessMode_c, l_networkAccessMode_result.c_str(), sizeof(l_networkAccessMode_c) - 1);  
			l_networkAccessMode_c[sizeof(l_networkAccessMode_c) - 1] = '\0';  

			string l_network_id_result = rs->getString(6);
			strncpy(l_NetworkId, l_network_id_result.c_str(), sizeof(l_NetworkId) - 1);  
			l_NetworkId[sizeof(l_NetworkId) - 1] = '\0';
			pDBHandler->Close(rs,stmt,conn); 
		}
		else
		{
			pDBHandler->Close(rs,stmt,conn);
			loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Data Not found at queryIndex=  %d |%s", queryIndex, __FUNCTION__);
			goto STAT2;
		}

		loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d |%s", queryIndex, __FUNCTION__);


		isJumpTrue=true;

		loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0, " Afetr Retrieving from DB table HLR_MT_SUB_GPRS_INFO <IMSI =%s>, <MSISDN=%s>, <networkAccessMode = %s>,<NetWorkId = %s>,<subscriber_state_gprs = %s,<trace_required_gsm = %c> |%s",l_imsi,l_msisdn,l_networkAccessMode_c,l_NetworkId,l_subscriber_state_gprs_c,l_trace_required_gprs,__FUNCTION__);

		if(l_hlr_c[0] == ' ') 
		{     queryIndex=6;
			/*	stmt = pDBHandler->GetStatement(
				(conn = pDBHandler->GetConnection()),
				"SELECT NVL(HLR_NUMBER, ' '), NVL(SMSC_NUMBER, ' ') "
				"FROM HLR_MT_PLMN_DETAILS "
				"WHERE NETWORK_ID = :1 "
				"AND CC || NDC = SUBSTR(:2, 1, LENGTH(CC || NDC))");*/

			stmt->setString(1, l_NetworkId);   
			stmt->setString(2, l_msisdn_c);    

			rs = stmt->executeQuery();


			if (rs->next()) 
			{
				string hlr_result = rs->getString(1); 
				strncpy(l_hlr_c, hlr_result.c_str(), sizeof(l_hlr_c) - 1);  
				l_hlr_c[sizeof(l_hlr_c) - 1] = '\0';  
				string smsc_result = rs->getString(2); 
				strncpy(l_smsc_c, smsc_result.c_str(), sizeof(l_smsc_c) - 1); 
				l_smsc_c[sizeof(l_smsc_c) - 1] = '\0';
				pDBHandler->Close(rs,stmt,conn); 
			}
			else
			{
				loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Data not founded at queryIndex %d |%s", queryIndex, __FUNCTION__);
				pDBHandler->Close(rs,stmt,conn); 
				goto STAT2;
			}


			loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d |%s", queryIndex, __FUNCTION__);


			l_hlr_c[strlen(l_hlr_c)] = '\0';
			if ( !((l_hlr_c[0] == ' ') && (strlen(l_hlr_c) == 1)) )
			{
				nNumber hlrid(30);
				hlrid.setValue(l_hlr_c, strlen(l_hlr_c));
				obj->setHlrNumber(hlrid);
				//obj->setHlrNumber(SmsmDbService::getNumberType(l_hlr_c));
			}
		}
		l_smsc_c[strlen(l_smsc_c)] = '\0';
		if ( !((l_smsc_c[0] == ' ') && (strlen(l_smsc_c) == 1)) )
		{
			nNumber smscid(30);
			smscid.setValue(l_smsc_c, strlen(l_smsc_c));
			obj->setSmscNumber(smscid);
		}

STAT2:
		if((isJumpTrue==false) && (flag_gsm==false))
		{
			goto DATA_NOT_FOUND;
		}
		l_networkAccessMode_c[sizeof(l_networkAccessMode_c)-1] = '\0';
		l_NetworkId[ sizeof(l_NetworkId)-1]=  '\0';



		l_subscriber_state_gsm_c[strlen(l_subscriber_state_gsm_c)] = '\0';
		l_subscriber_state_gprs_c[ strlen(l_subscriber_state_gprs_c)]= '\0';

		if(!((l_subscriber_state_gsm_c[0] == 0x30) && (strlen(l_subscriber_state_gsm_c) == 1)))
		{
			nNumber subStateGsm(30);
			subStateGsm.setValue(l_subscriber_state_gsm_c,strlen(l_subscriber_state_gsm_c));
			obj->setSubStateGsm(subStateGsm);
		}
		else
		{
			int l_substateActiveGsm_value_len=1;
			memcpy((char *)l_substateActiveGsm_value, "0",l_substateActiveGsm_value_len);
			nNumber gsmSubActive(30);
			gsmSubActive.setValue(l_substateActiveGsm_value,l_substateActiveGsm_value_len);
			obj->setSubStateGsm(gsmSubActive);
		}

		if(!((l_subscriber_state_gprs_c[0] == 0x30) && (strlen(l_subscriber_state_gprs_c) == 1)))
		{
			nNumber subStateGprs(30);
			subStateGprs.setValue(l_subscriber_state_gprs_c,strlen(l_subscriber_state_gprs_c));
			obj->setSubStateGprs(subStateGprs);
		}
		else
		{
			int l_substateActiveGprs_value_len=1;
			memcpy(l_substateActiveGprs_value, "0",l_substateActiveGprs_value_len);
			nNumber gprsSubActive(30);
			gprsSubActive.setValue(l_substateActiveGprs_value,l_substateActiveGprs_value_len);
			obj->setSubStateGprs(gprsSubActive);
		}

		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " flag= %d |%s",flag_gsm,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " isJumpTrue flag= %d |%s",isJumpTrue,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " l_subscriber_state_gsm = %s |%s",l_subscriber_state_gsm_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " l_subscriber_state_gprs= %s |%s",l_subscriber_state_gprs_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " l_trace_required_gsm = %d |%s",l_trace_required_gsm,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " l_subscriber_state_gprs = %d |%s",l_subscriber_state_gprs_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " l_trace_required_gprs = %d |%s",l_trace_required_gprs,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " networkAccessMode = %s|%s",l_networkAccessMode_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " l_NetworkId = %s|%s",l_NetworkId,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " l_hlr address = %s|%s",l_hlr_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " l_smsc address = %s|%s",l_smsc_c,__FUNCTION__);

		l_trace_required[0] = l_trace_required_gsm?l_trace_required_gsm[0]:l_trace_required_gprs[0];
		l_trace_required[1] = '\0';
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " l_subscriber_state = %c |%s",l_subscriber_state,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " l_trace_required = %c |%s",l_trace_required,__FUNCTION__);

		if( !((l_NetworkId[0] == 0x30) && (strlen(l_NetworkId) == 1)) )
		{
			nNumber nwid(30);
			nwid.setValue(l_NetworkId, strlen(l_NetworkId));
			obj->setNetworkId( nwid);
		}

		int q1_flag =0;
		int q2_flag =0;
		stmt = pDBHandler->GetStatement((conn = pDBHandler->GetConnection()),
				"SELECT NVL(LMSI, 0), NVL(MNRF, 0), NVL(MCEF, 0), NVL(MNRR, 0), NVL(MSC_NUMBER, 0), "
				"NVL(NOTREACHABLE_FLAG, 0), NVL(MSC_AREA_RESTRIC_FLAG, 0), NVL(MSISDN, 0), NVL(ROAMING_STATUS, 0), "
				"NVL(MSPURGED_NONGPRS_FLAG, 0), NVL(VLR_SUPPORTS_SUPER_CHARGER, 0), NVL(VLR_NUMBER, 0), NVL(ODB_INDUCED_BARR_DATA, 0) "
				"FROM HLR_TR_SUB_GSM_INFO "
				"WHERE (IMSI = :1 OR MSISDN = :2)");

		stmt->setString(1, l_imsi_c);  // Bind IMSI (or MSISDN) - :1 -> l_imsi_c
		stmt->setString(2, l_msisdn_c);  // Bind MSISDN - :2 -> l_msisdn_c

		rs = stmt->executeQuery();

		if (rs->next()) {
			string l_lmsi_result = rs->getString(1); 
			strncpy(l_lmsi_c, l_lmsi_result.c_str(), sizeof(l_lmsi_c) - 1);  
			l_lmsi_c[sizeof(l_lmsi_c) - 1] = '\0';  // Ensure null termination

			string l_mnrf_result = rs->getString(2); 
			strncpy(l_mnrf_c, l_mnrf_result.c_str(), sizeof(l_mnrf_c) - 1);  
			l_mnrf_c[sizeof(l_mnrf_c) - 1] = '\0'; 

			l_mcef_c = rs->getInt(3); 

			string l_mnrr_result = rs->getString(4); 
			strncpy(l_mnrr_c_gsm, l_mnrr_result.c_str(), sizeof(l_mnrr_c_gsm) - 1);  
			l_mnrr_c_gsm[sizeof(l_mnrr_c_gsm) - 1] = '\0'; 

			string l_mscNumber_result = rs->getString(5); 
			strncpy(l_mscNumber_c, l_mscNumber_result.c_str(), sizeof(l_mscNumber_c) - 1);  
			l_mscNumber_c[sizeof(l_mscNumber_c) - 1] = '\0'; 

			string l_notreachable_result = rs->getString(6); 
			strncpy(l_notreachableGsm_c, l_notreachable_result.c_str(), sizeof(l_notreachableGsm_c) - 1);  
			l_notreachableGsm_c[sizeof(l_notreachableGsm_c) - 1] = '\0'; 

			string l_msStatus_result = rs->getString(7); 
			strncpy(l_msNotReachableInMscStatus, l_msStatus_result.c_str(), sizeof(l_msNotReachableInMscStatus) - 1);  
			l_msNotReachableInMscStatus[sizeof(l_msNotReachableInMscStatus) - 1] = '\0'; 

			string l_msisdn_result = rs->getString(8); 
			strncpy(l_msisdn, l_msisdn_result.c_str(), sizeof(l_msisdn) - 1);  
			l_msisdn[sizeof(l_msisdn) - 1] = '\0'; 

			string l_roaming_result = rs->getString(9); 
			strncpy(l_roaming_status, l_roaming_result.c_str(), sizeof(l_roaming_status) - 1);  
			l_roaming_status[sizeof(l_roaming_status) - 1] = '\0'; 

			string l_mspurged_result = rs->getString(10); 
			strncpy(l_mspurged_nongprs_flag, l_mspurged_result.c_str(), sizeof(l_mspurged_nongprs_flag) - 1);  
			l_mspurged_nongprs_flag[sizeof(l_mspurged_nongprs_flag) - 1] = '\0'; 

			string l_superCharger_result = rs->getString(11); 
			strncpy(l_superCharger_c_gsm, l_superCharger_result.c_str(), sizeof(l_superCharger_c_gsm) - 1);  
			l_superCharger_c_gsm[sizeof(l_superCharger_c_gsm) - 1] = '\0'; 

			string l_vlrNumber_result = rs->getString(12); 
			strncpy(l_vlrNumber_c, l_vlrNumber_result.c_str(), sizeof(l_vlrNumber_c) - 1);  
			l_vlrNumber_c[sizeof(l_vlrNumber_c) - 1] = '\0'; 

			//l_gsmOdbinducedBarring = rs->getUInt(13); 

		}

		else
		{
			loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "no rows found  %d |%s", queryIndex, __FUNCTION__);
			pDBHandler->Close(rs,stmt,conn);
			goto QUERY2;
		}
		loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, 
				" <IMSI = %s><MSISDN = %s><MSC_NUMBER = %s><NOTREACHABLE_FLAG = %s>"
				"<MSC_AREA_RESTRIC_FLAG = %s><ROAMING_STATUS = %s><VLR_NUMBER = %s> "
				"from HLR_TR_SUB_GSM_INFO ...|%s",
				l_lmsi_c, l_msisdn, l_mscNumber_c, l_notreachableGsm_c, 
				l_msNotReachableInMscStatus, l_roaming_status, l_vlrNumber_c, __FUNCTION__);

		pDBHandler->Close(rs,stmt,conn);
		loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d |%s", queryIndex, __FUNCTION__);
		q1_flag =1;
		if (l_msNotReachableInMscStatus[0]!='1') //mohan added Mar15 2009
		{
			l_msNotReachableInMscStatus[0] = l_mspurged_nongprs_flag[0];
		}
		loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0, " <IMSI = %s><MSISDN=%s><MSC_NUMBER=%s><NOTREACHABLE_FLAG=%s><MSC_AREA_RESTRIC_FLAG=%s><ROAMING_STATUS=%s><VLR_NUMBER=%s> from HLR_TR_SUB_GSM_INFO ...|%s",l_lmsi_c,l_msisdn,l_mscNumber_c,l_notreachableGsm_c,l_msNotReachableInMscStatus,l_roaming_status,l_vlrNumber_c,__FUNCTION__);
QUERY2:

		queryIndex=8;
		stmt = pDBHandler->GetStatement(
				(conn = pDBHandler->GetConnection()),
				"SELECT NVL(SGSN_NUMBER, 0), NVL(SGSN_ADDRESS, 0), NVL(MNRR, 0), "
				"NVL(MNRG, 0), NVL(NOTREACHABLE_GPRS_FLAG, 0), NVL(SGSN_AREA_RESTRICTED_FLAG, 0), "
				"NVL(MSISDN, 0), NVL(MCEF, 0), NVL(ROAMING_STATUS, 0), NVL(MSPURGED_FOR_GPRS, 0), "
				"NVL(SGSN_SUPPORTS_SUPER_CHARGER, 0), NVL(ODB_INDUCED_BARR_DATA, 0) "
				"FROM HLR_TR_SUB_GPRS_INFO "
				"WHERE (IMSI = :1 OR MSISDN = :2)");

		stmt->setString(1, l_imsi_c);  
		stmt->setString(2, l_msisdn_c);  

		rs = stmt->executeQuery();

		if (rs->next()) {

			string l_sgsnNumber_result = rs->getString(1); 
			strncpy(l_sgsnNumber_c, l_sgsnNumber_result.c_str(), sizeof(l_sgsnNumber_c) - 1);  
			l_sgsnNumber_c[sizeof(l_sgsnNumber_c) - 1] = '\0';  // Ensure null termination

			string l_sgsnAddress_result = rs->getString(2); 
			strncpy(l_sgsnAddress_c, l_sgsnAddress_result.c_str(), sizeof(l_sgsnAddress_c) - 1);  
			l_sgsnAddress_c[sizeof(l_sgsnAddress_c) - 1] = '\0'; 

			string l_mnrr_result = rs->getString(3); 
			strncpy(l_mnrr_c_gprs, l_mnrr_result.c_str(), sizeof(l_mnrr_c_gprs) - 1);  
			l_mnrr_c_gprs[sizeof(l_mnrr_c_gprs) - 1] = '\0'; 

			string l_mnrg_result = rs->getString(4); 
			strncpy(l_mnrg_c, l_mnrg_result.c_str(), sizeof(l_mnrg_c) - 1);  
			l_mnrg_c[sizeof(l_mnrg_c) - 1] = '\0'; 

			string l_notreachable_result = rs->getString(5); 
			strncpy(l_notreachableGprs_c, l_notreachable_result.c_str(), sizeof(l_notreachableGprs_c) - 1);  
			l_notreachableGprs_c[sizeof(l_notreachableGprs_c) - 1] = '\0'; 

			string l_sgsn_area_restricted_result = rs->getString(6); 
			strncpy(l_msNotReachableInSgsnStatus, l_sgsn_area_restricted_result.c_str(), sizeof(l_msNotReachableInSgsnStatus) - 1);  
			l_msNotReachableInSgsnStatus[sizeof(l_msNotReachableInSgsnStatus) - 1] = '\0'; 

			string l_msisdn_result = rs->getString(7); 
			strncpy(l_msisdn, l_msisdn_result.c_str(), sizeof(l_msisdn) - 1);  
			l_msisdn[sizeof(l_msisdn) - 1] = '\0'; 

			l_mcef_c_gprs = rs->getInt(8); 

			string l_roaming_result = rs->getString(9); 
			strncpy(l_gprs_roaming_status, l_roaming_result.c_str(), sizeof(l_gprs_roaming_status) - 1);  
			l_gprs_roaming_status[sizeof(l_gprs_roaming_status) - 1] = '\0'; 

			string l_mspurged_result = rs->getString(10); 
			strncpy(l_mspurged_gprs_flag, l_mspurged_result.c_str(), sizeof(l_mspurged_gprs_flag) - 1);  
			l_mspurged_gprs_flag[sizeof(l_mspurged_gprs_flag) - 1] = '\0'; 

			string l_superCharger_result = rs->getString(11); 
			strncpy(l_superCharger_c_gprs, l_superCharger_result.c_str(), sizeof(l_superCharger_c_gprs) - 1);  
			l_superCharger_c_gprs[sizeof(l_superCharger_c_gprs) - 1] = '\0'; 

			l_gprsOdbinducedBarring = rs->getUInt(12); 

		}
		else
		{
			loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "no rows found  %d |%s", queryIndex, __FUNCTION__);
			pDBHandler->Close(rs,stmt,conn);
			goto QUERY3;
		}
		pDBHandler->Close(rs,stmt,conn);
		loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d |%s", queryIndex, __FUNCTION__);

		q2_flag =1;
		if (l_msNotReachableInSgsnStatus[0]!='1')
		{
			l_msNotReachableInSgsnStatus[0] = l_mspurged_gprs_flag[0];
		}

QUERY3:

		if (!(q1_flag || q2_flag))
			goto ABSENT_SUBSCRIBER;

		l_imsi[sizeof(l_imsi) - 1] = '\0';  

		l_msisdn[sizeof(l_msisdn)-1] = '\0';
		l_lmsi_c[sizeof(l_lmsi_c)-1] = '\0';
		l_mnrf_c[sizeof(l_mnrf_c)-1] = '\0';

		l_mnrr_c_gsm[sizeof(l_mnrr_c_gsm)-1] = '\0';
		l_mnrr_c_gprs[sizeof(l_mnrr_c_gprs)-1] = '\0';
		l_mscNumber_c[sizeof(l_mscNumber_c)-1] = '\0';
		l_gcsf_a[sizeof(l_gcsf_a)-1]='\0';


		l_vlrNumber_c[sizeof(l_vlrNumber_c)-1] = '\0';
		l_notreachableGsm_c[sizeof(l_notreachableGsm_c)-1] = '\0';
		l_msNotReachableInMscStatus[sizeof(l_msNotReachableInMscStatus)-1] = '\0';
		l_sgsnNumber_c[sizeof(l_sgsnNumber_c)-1] = '\0';
		l_sgsnAddress_c[sizeof(l_sgsnAddress_c)-1] = '\0';
		l_mnrg_c[sizeof(l_mnrg_c)-1] = '\0';
		l_notreachableGprs_c[sizeof(l_notreachableGprs_c)-1] = '\0';
		l_msNotReachableInSgsnStatus[sizeof(l_msNotReachableInSgsnStatus)-1] = '\0';
		l_roaming_status[sizeof(l_roaming_status)-1]='\0';
		l_gprs_roaming_status[sizeof(l_gprs_roaming_status)-1]='\0';

		l_mspurged_nongprs_flag[sizeof(l_mspurged_nongprs_flag)-1]='\0';
		l_mspurged_gprs_flag[sizeof(l_mspurged_gprs_flag)-1]='\0';

		l_superCharger_c_gsm[sizeof(l_superCharger_c_gsm)-1]='\0';
		l_superCharger_c_gprs[sizeof(l_superCharger_c_gprs)-1]='\0';



		if (l_networkAccessMode_c[0] != '1') {
			queryIndex = 9;
			conn = pDBHandler->GetConnection();  // Get the connection once

			/*stmt = pDBHandler->GetStatement(conn,
			  "SELECT COUNT(1) "
			  "FROM HLR_TR_GSM_BEARER_TELE_DATA "
			  "WHERE IMSI = :1 AND BEARER_TELE_CODE = 33 "
			  "AND BEARER_TELE_TYPE = 2 AND BEARER_TELE_STATUS = 1 "
			  "AND ACTIVE_ROAMING_PROFILE_ID = :2"
			  );*/

			stmt->setString(1, l_imsi);
			stmt->setInt(2, l_gsm_active_roaming_subs_profile_id);

			rs = stmt->executeQuery();

			if (rs->next()) {
				//string l_mscSupportsMtSms_c = rs->getString(1);
				//strncpy(l_mscSupportsMtSms_c, l_mscSupportsMtSms_.c_str(), sizeof(l_mscSupportsMtSms_c) - 1);
				//l_mscSupportsMtSms_c[sizeof(l_mscSupportsMtSms_c) - 1] = '\0';
			} else {
				loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Data not found at queryIndex %d | %s", queryIndex, __FUNCTION__);
				pDBHandler->Close(rs, stmt, conn);  // Properly release resources
				goto DATA_NOT_FOUND;
			}

			loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d | %s", queryIndex, __FUNCTION__);
			pDBHandler->Close(rs, stmt, conn);  // Ensure clean-up

			if (l_networkAccessMode_c[0] == '2') {
				queryIndex = 10;
				conn = pDBHandler->GetConnection();  // Get a new connection for the next query

				/*	stmt = pDBHandler->GetStatement(conn,
					"SELECT COUNT(1) "
					"FROM HLR_TR_GPRS_BEARER_TELE_DATA "
					"WHERE IMSI = :1 AND BEARER_TELE_CODE = 33 "
					"AND BEARER_TELE_TYPE = 2 AND BEARER_TELE_STATUS = 1 "
					"AND ACTIVE_ROAMING_PROFILE_ID = :2"
					);*/

				stmt->setString(1, l_imsi);
				stmt->setInt(2, l_gprs_active_roaming_subs_profile_id);

				rs = stmt->executeQuery();

				if (rs->next()) {
					string l_sgsnSupportsMtSms_ = rs->getString(1);
					strncpy(l_sgsnSupportsMtSms_c, l_sgsnSupportsMtSms_.c_str(), sizeof(l_sgsnSupportsMtSms_c) - 1);
					l_sgsnSupportsMtSms_c[sizeof(l_sgsnSupportsMtSms_c) - 1] = '\0';
				} else {
					loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Data not found at queryIndex %d | %s", queryIndex, __FUNCTION__);
					pDBHandler->Close(rs, stmt, conn);  // Properly release resources
					goto DATA_NOT_FOUND;
				}

				loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d | %s", queryIndex, __FUNCTION__);
				pDBHandler->Close(rs, stmt, conn);  // Ensure clean-up
			}
		}

		else
		{
			queryIndex=11;
			/*stmt = pDBHandler->GetStatement((conn = pDBHandler->GetConnection()),
			  "SELECT COUNT(1) "
			  "FROM HLR_TR_GPRS_BEARER_TELE_DATA "
			  "WHERE IMSI = :1 AND BEARER_TELE_CODE = 33 "
			  "AND BEARER_TELE_TYPE = 2 AND BEARER_TELE_STATUS = 1 "
			  "AND ACTIVE_ROAMING_PROFILE_ID = :2");*/

			stmt->setString(1, l_imsi); 
			stmt->setInt(2, l_gprs_active_roaming_subs_profile_id);  
			rs = stmt->executeQuery();

			if (rs->next()) {
				string l_sgsnSupportsMtSms_ = rs->getString(1);
				strncpy(l_sgsnSupportsMtSms_c,l_sgsnSupportsMtSms_.c_str(),sizeof(l_sgsnSupportsMtSms_c)-1);
				l_mscSupportsMtSms_c[sizeof(l_sgsnSupportsMtSms_c) - 1] = '\0';
			}
			else
			{
				loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Data not found at  queryIndex %d |%s", queryIndex, __FUNCTION__);
				pDBHandler->Close(rs,stmt,conn);
				goto DATA_NOT_FOUND;
			}

			loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d |%s", queryIndex, __FUNCTION__);
			pDBHandler->Close(rs,stmt,conn);
		}
		l_mscSupportsMtSms_c[sizeof(l_mscSupportsMtSms_c)-1] = '\0';
		l_sgsnSupportsMtSms_c[sizeof(l_sgsnSupportsMtSms_c)-1] = '\0';

		if(trGprsTeledeleteFlag)
		{
			if(l_networkAccessMode_c[0]!='1')
			{

				queryIndex=12;

				stmt = pDBHandler->GetStatement(
						(conn = pDBHandler->GetConnection()),
						"SELECT COUNT(1) "
						"FROM HLR_MT_SUB_GSM_INFO A, HLR_TR_GSM_BEARER_TELE_DATA B "
						"WHERE (A.IMSI = :1 OR A.MSISDN = :2) "
						"AND A.ACTIVE_ROAMING_PROFILE_ID = :3 "
						"AND A.IMSI = B.IMSI "
						"AND A.ACTIVE_ROAMING_PROFILE_ID = B.ACTIVE_ROAMING_PROFILE_ID "
						"AND B.BEARER_TELE_CODE = 33 "
						"AND B.BEARER_TELE_TYPE = '2'");

				stmt->setString(1, l_imsi_c);  
				stmt->setString(2, l_msisdn_c);  
				stmt->setInt(3, l_gsm_active_roaming_subs_profile_id); 


				rs = stmt->executeQuery();

				if (rs->next()) 
				{
					string l_mtSmsProvisioned_ = rs->getString(1);  
					strncpy(l_mtSmsProvisioned_c,l_mtSmsProvisioned_.c_str(),sizeof(l_mtSmsProvisioned_c)-1);
					l_mtSmsProvisioned_c[sizeof(l_mtSmsProvisioned_c) - 1] = '\0';
				}
				else
				{
					loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Data not found at  queryIndex %d |%s", queryIndex, __FUNCTION__);
					pDBHandler->Close(rs,stmt,conn);
					goto DATA_NOT_FOUND;
				}

				loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d |%s", queryIndex, __FUNCTION__);
				pDBHandler->Close(rs,stmt,conn);
				if(l_networkAccessMode_c[0]=='2')
				{
					stmt = pDBHandler->GetStatement(
							(conn = pDBHandler->GetConnection()),
							"SELECT COUNT(1) "
							"FROM HLR_MT_SUB_GPRS_INFO A, HLR_TR_GPRS_BEARER_TELE_DATA B "
							"WHERE (A.IMSI = :1 OR A.MSISDN = :2) "
							"AND A.ACTIVE_ROAMING_PROFILE_ID = :3 "
							"AND A.IMSI = B.IMSI "
							"AND A.ACTIVE_ROAMING_PROFILE_ID = B.ACTIVE_ROAMING_PROFILE_ID "
							"AND B.BEARER_TELE_CODE = 33 "
							"AND B.BEARER_TELE_TYPE = '2'");

					stmt->setString(1, l_imsi_c);  
					stmt->setString(2, l_msisdn_c); 
					stmt->setInt(3, l_gprs_active_roaming_subs_profile_id);  

					rs = stmt->executeQuery();

					if (rs->next()) {
						string l_mtSmsProvisioned_ = rs->getString(1);
						strncpy(l_mtSmsProvisioned_c,l_mtSmsProvisioned_.c_str(),sizeof(l_mtSmsProvisioned_c)-1);
						l_mtSmsProvisioned_c[sizeof(l_mtSmsProvisioned_c) - 1] = '\0';
					}
					else
					{
						loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Data not found at  queryIndex %d |%s", queryIndex, __FUNCTION__);
						pDBHandler->Close(rs,stmt,conn);
						goto DATA_NOT_FOUND;
					}

					loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d |%s", queryIndex, __FUNCTION__);
					pDBHandler->Close(rs,stmt,conn);
				}
			}
			else
			{
				queryIndex=13;
				stmt = pDBHandler->GetStatement((conn = pDBHandler->GetConnection()),
						"SELECT COUNT(1) "
						"FROM HLR_MT_SUB_GPRS_INFO A, HLR_TR_GPRS_BEARER_TELE_DATA B "
						"WHERE (A.IMSI = :1 OR A.MSISDN = :2) "
						"AND A.ACTIVE_ROAMING_PROFILE_ID = :3 "
						"AND A.IMSI = B.IMSI "
						"AND A.ACTIVE_ROAMING_PROFILE_ID = B.ACTIVE_ROAMING_PROFILE_ID "
						"AND B.BEARER_TELE_CODE = 33 "
						"AND B.BEARER_TELE_TYPE = '2'");

				stmt->setString(1, l_imsi_c);  
				stmt->setString(2, l_msisdn_c);  
				stmt->setInt(3, l_gprs_active_roaming_subs_profile_id);  

				rs = stmt->executeQuery();

				if (rs->next()) {
					string l_mtSmsProvisioned_ = rs->getString(1);  
					strncpy(l_mtSmsProvisioned_c,l_mtSmsProvisioned_.c_str(),sizeof(l_mtSmsProvisioned_c)-1);
					l_mtSmsProvisioned_c[sizeof(l_mtSmsProvisioned_c) - 1] = '\0';
				}
				else
				{
					loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Data not found at  queryIndex %d |%s", queryIndex, __FUNCTION__);
					pDBHandler->Close(rs,stmt,conn);
					goto DATA_NOT_FOUND;
				}

				loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d |%s", queryIndex, __FUNCTION__);
				pDBHandler->Close(rs,stmt,conn);
			}
		}
		else
		{
			if(l_networkAccessMode_c[0]!='1')
			{

				loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d |%s", queryIndex, __FUNCTION__);
				queryIndex = 14;
				stmt = pDBHandler->GetStatement(
						(conn = pDBHandler->GetConnection()),
						"SELECT COUNT(1) "
						"FROM HLR_MT_SUB_GSM_INFO A, HLR_TR_GSM_BEARER_TELE_DATA B "
						"WHERE (A.IMSI = :1 OR A.MSISDN = :2) "
						"AND A.ACTIVE_ROAMING_PROFILE_ID = :3 "
						"AND A.IMSI = B.IMSI "
						"AND A.ACTIVE_ROAMING_PROFILE_ID = B.ACTIVE_ROAMING_PROFILE_ID "
						"AND B.BEARER_TELE_CODE = 33 "
						"AND B.BEARER_TELE_TYPE = '2'");

				stmt->setString(1, l_imsi_c);
				stmt->setString(2, l_msisdn_c);
				stmt->setInt(3, l_gsm_active_roaming_subs_profile_id);

				rs = stmt->executeQuery();

				if (rs->next()) {

					int l_mtSmsProvisioned_ = rs->getInt(1);

					snprintf(l_mtSmsProvisioned_c, sizeof(l_mtSmsProvisioned_c), "%d", l_mtSmsProvisioned_);

					l_mtSmsProvisioned_c[sizeof(l_mtSmsProvisioned_c) - 1] = '\0';
				}
				else
				{
					loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Data not found at  queryIndex %d |%s", queryIndex, __FUNCTION__);
					pDBHandler->Close(rs,stmt,conn);
					goto DATA_NOT_FOUND;
				}

				loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d |%s", queryIndex, __FUNCTION__);
				pDBHandler->Close(rs,stmt,conn);


				if(l_networkAccessMode_c[0]=='2')
				{
					queryIndex=15;
					stmt = pDBHandler->GetStatement(
							(conn = pDBHandler->GetConnection()),
							"SELECT COUNT(1) "
							"FROM HLR_MT_SUB_GPRS_INFO A, HLR_MT_SUB_GPRS_BEAR_TELE_DATA B "
							"WHERE (A.IMSI = :1 OR A.MSISDN = :2) "
							"AND A.ACTIVE_ROAMING_PROFILE_ID = :3 "
							"AND A.IMSI = B.IMSI "
							"AND A.ACTIVE_ROAMING_PROFILE_ID = B.ACTIVE_ROAMING_PROFILE_ID "
							"AND B.BEARER_TELE_CODE = 33 "
							"AND B.BEARER_TELE_TYPE = '2'"
							);

					stmt->setString(1, l_imsi_c); 
					stmt->setString(2, l_msisdn_c);  
					stmt->setInt(3, l_gprs_active_roaming_subs_profile_id); 

					rs = stmt->executeQuery();

					if (rs->next()) {
						string l_mtSmsProvisioned_ = rs->getString(1); 
						strncpy(l_mtSmsProvisioned_c,l_mtSmsProvisioned_.c_str(),sizeof(l_mtSmsProvisioned_c)-1);
						l_mtSmsProvisioned_c[sizeof(l_mtSmsProvisioned_c) - 1] = '\0'; 

					}
					else
					{
						loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Data not found at  queryIndex %d |%s", queryIndex, __FUNCTION__);
						pDBHandler->Close(rs,stmt,conn);
						goto DATA_NOT_FOUND;
					}

					loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d |%s", queryIndex, __FUNCTION__);
					pDBHandler->Close(rs,stmt,conn);
				}
			}
			else
			{
				queryIndex=16;
				stmt = pDBHandler->GetStatement(
						(conn = pDBHandler->GetConnection()),
						"SELECT COUNT(1) "
						"FROM HLR_MT_SUB_GPRS_INFO A, HLR_MT_SUB_GPRS_BEAR_TELE_DATA B "
						"WHERE (A.IMSI = :1 OR A.MSISDN = :2) "
						"AND A.ACTIVE_ROAMING_PROFILE_ID = :3 "
						"AND A.IMSI = B.IMSI "
						"AND A.ACTIVE_ROAMING_PROFILE_ID = B.ACTIVE_ROAMING_PROFILE_ID "
						"AND B.BEARER_TELE_CODE = 33 "
						"AND B.BEARER_TELE_TYPE = '2'"
						);

				stmt->setString(1, l_imsi_c);  
				stmt->setString(2, l_msisdn_c);  
				stmt->setInt(3, l_gprs_active_roaming_subs_profile_id);  

				rs = stmt->executeQuery();

				if (rs->next()) {
					string l_mtSmsProvisioned_ = rs->getString(1); 
					strncpy(l_mtSmsProvisioned_c,l_mtSmsProvisioned_.c_str(),sizeof(l_mtSmsProvisioned_c)-1);
					l_mtSmsProvisioned_c[sizeof(l_mtSmsProvisioned_c) - 1] = '\0'; 
				}
				else
				{
					loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Data not found at  queryIndex %d |%s", queryIndex, __FUNCTION__);
					pDBHandler->Close(rs,stmt,conn);
					goto DATA_NOT_FOUND;
				}

				loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d |%s", queryIndex, __FUNCTION__);
				pDBHandler->Close(rs,stmt,conn);
			}
		}


		queryIndex=17;

		loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d |%s", queryIndex, __FUNCTION__);

		stmt = pDBHandler->GetStatement(
				(conn = pDBHandler->GetConnection()),
				"SELECT NVL(IPSMGW_STATUS, 0), NVL(GSMSCF_ADDRESS, '0') "
				"FROM HLR_TR_SUB_GSM_INFO "
				"WHERE MSISDN = :1"
				);

		// Bind the input parameter
		stmt->setString(1, l_msisdn_c);  // :1 -> l_msisdn_c

		// Execute the query
		rs = stmt->executeQuery();

		// Process the result
		if (rs->next()) {
			// Retrieve the results and store them in the respective variables
			ipstatus = rs->getInt(1);  // Get IPSMGW_STATUS (or 0 if NULL)
			string l_gcsf_= rs->getString(2); 
			strncpy(l_gcsf_a,l_gcsf_.c_str(),sizeof(l_gcsf_a)-1);
			l_gcsf_a[sizeof(l_gcsf_a) - 1] = '\0'; 
		}


		// Optional logging of query execution for debugging or performance tracking
		loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d |%s", queryIndex, __FUNCTION__);
		pDBHandler->Close(rs,stmt,conn);

		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " --> IPSMGW STATUS [%d] and GSMSCF_ADDRESS [%s] |%s",ipstatus,l_gcsf_a,__FUNCTION__);


		// End Release_2.1.8.0
		//spell-2100 End (07/02/2012)

		l_mtSmsProvisioned_c[sizeof(l_mtSmsProvisioned_c)-1] = '\0';
		if ( l_mtSmsProvisioned_c[0] > '1' )
		{
			l_mtSmsProvisioned_c[0] = '1'; 
		}
		if(l_networkAccessMode_c[0]!='1')
		{
			loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " execute l_ssbarring_c query...|%s",__FUNCTION__); 
			queryIndex=14;
			// Prepare the OCCI statement for execution
			stmt = pDBHandler->GetStatement(
					(conn = pDBHandler->GetConnection()),
					"SELECT COUNT(1) "
					"FROM HLR_MT_SUB_GSM_INFO A, HLR_MT_SUB_CALLBARR_INFO B, HLR_TR_SUB_GSM_INFO C "
					"WHERE A.MSISDN = :1 "
					"AND A.MSISDN = B.MSISDN "
					"AND A.MSISDN = C.MSISDN "
					"AND A.BARRING_SS = '1' "
					"AND (B.SS_CODE = 144 OR B.SS_CODE = 153 OR B.SS_CODE = 154 OR (B.SS_CODE = 155 AND C.ROAMING_STATUS != '0')) "
					"AND B.BASICSERVICEGROUP_CODE = 32 "
					"AND B.BASICSERVICEGROUP_TYPE = '2' "
					"AND B.SS_STATUS = 7 "
					"AND B.ACTIVE_ROAMING_PROFILE_ID = :2 "
					"AND A.ACTIVE_ROAMING_PROFILE_ID = :2"
					);

			// Bind input parameters
			stmt->setString(1, l_msisdn_c);  // :1 -> l_msisdn_c
			stmt->setInt(2, l_gsm_active_roaming_subs_profile_id);  // :2 -> l_gsm_active_roaming_subs_profile_id

			// Execute the query
			rs = stmt->executeQuery();

			// Process the result
			if (rs->next()) {
				string l_ssbarring_ = rs->getString(1); 
				strncpy(l_ssbarring_c,l_ssbarring_.c_str(),sizeof(l_ssbarring_c)-1);
				l_ssbarring_c[sizeof(l_ssbarring_c) - 1] = '\0'; // Get COUNT(1) result (number of matching rows)
			}

			// Optional logging of query execution
			loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "Query executed %d |%s", queryIndex, __FUNCTION__);
			pDBHandler->Close(rs,stmt,conn);

		}
		else
		{
			l_ssbarring_c[0]='0';
			int l_ssbarring_c_len=strlen(l_ssbarring_c);
			l_ssbarring_c_len++;
		}
		l_ssbarring_c[sizeof(l_ssbarring_c)-1] = '\0';

		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " --> l_ssbarring_c.arr = %s |%s",l_ssbarring_c,__FUNCTION__);
		//2.0.13.1 Begin
		//obj->setGsmOdbInducedBarring(l_gsmOdbinducedBarring);
		obj->setGprsOdbInducedBarring(l_gprsOdbinducedBarring);

STMT1:
		int l_operatorBarringGsm_c_len;
		/*if ((l_gsmOdbinducedBarring) & (1048576))
		  {

		  memcpy(l_operatorBarringGsm_c, "1",1);
		  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " OperatorBarring Set for GSM...|%s",__FUNCTION__);
		  }else
		  {

		  strncpy(l_operatorBarringGsm_c, "0", sizeof(l_operatorBarringGsm_c) - 1);
		  l_operatorBarringGsm_c[sizeof(l_operatorBarringGsm_c) - 1] = '\0';

		  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "No OperatorBarring Set for GSM... | %s", __FUNCTION__);
		  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, " --> l_operatorBarringGsm_c = %s | %s", l_operatorBarringGsm_c, __FUNCTION__);

		  }*/

		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " --> l_operatorBarringGsm_c.arr = %s |%s",l_operatorBarringGsm_c,__FUNCTION__);

		/*if ((l_gprsOdbinducedBarring) & (1048576))
		  {

		  memcpy(l_operatorBarringGprs_c, "1",1);
		  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " OperatorBarring Set for GPRS...|%s",__FUNCTION__);
		  }else
		  {

		  memcpy(l_operatorBarringGprs_c, "0",1);
		  loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " No OperatorBarring Set for GPRS...| %s",__FUNCTION__);
		  }*/
		l_operatorBarringGprs_c[sizeof(l_operatorBarringGprs_c)-1] = '\0';


		char temp_msisdn[MSISDN_SIZE +1];

		if( !((l_msisdn[0] == 0x30) && (strlen(l_msisdn) == 1)) )
		{
			int temp_msisdn_len = BCDEncoderobj.CharToBCD(temp_msisdn,l_msisdn,strlen(l_msisdn));
			msisdn.setValue(temp_msisdn, temp_msisdn_len);
			obj->setMsisdn(msisdn );
		}

		if( !((l_imsi[0] == 0x30) && (strlen(l_imsi) == 1)) )
		{
			char temp_imsi[IMSI_SIZE +1];
			int temp_imsi_len = BCDEncoderobj.CharToBCD(temp_imsi,l_imsi,strlen(l_imsi));
			imsi.setValue(temp_imsi, temp_imsi_len);
			obj->setImsi( imsi);
		}

		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " msisdn = %s | %s  ",l_msisdn,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " imsi = %s | %s  ",l_imsi,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " lmsi = %s |%s",l_lmsi_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " mnrf = %s |%s",l_mnrf_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " mcef_gsm = %d |%s",l_mcef_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " mcef_gprs = %d |%s",l_mcef_c_gprs,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " mnrr_gsm = %s |%s",l_mnrr_c_gsm,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " mnrr_gprs = %s |%s",l_mnrr_c_gprs,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " mnrg = %s |%s",l_mnrg_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " mscSupportsMtSms = %s |%s",l_mscSupportsMtSms_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " notreachableGsm = %s |%s",l_notreachableGsm_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " sgsnSupportsMtSms = %s |%s",l_sgsnSupportsMtSms_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " networkAccessMode = %s|%s",l_networkAccessMode_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " ssbarring = %s |%s",l_ssbarring_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " operatorBarringForGSM = %s |%s",l_operatorBarringGsm_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " operatorBarringForGPRS = %s |%s",l_operatorBarringGprs_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "MT ODBBarring = %s |%s",l_mtodbBarring_value,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " mtSmsProvisioned = %s |%s",l_mtSmsProvisioned_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " sgsnNumber = %s |%s",l_sgsnNumber_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " sgsnAddress = %s |%s",l_sgsnAddress_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " mscNumber = %s  |%s",l_mscNumber_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Vlr Number is = %s  |%s",l_vlrNumber_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " notreachableGprs = %s |%s",l_notreachableGprs_c,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " l_trace_required = %c |%s",l_trace_required,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " l_gsm_active_roaming_subs_profile_id=%d |%s",l_gsm_active_roaming_subs_profile_id,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " l_gprs_active_roaming_subs_profile_id=%d |%s",l_gprs_active_roaming_subs_profile_id,__FUNCTION__);
		//Begin spell-2100 
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " l_mspurged_nongprs_flag.arr=%s|%s",l_mspurged_nongprs_flag,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " l_mspurged_gprs_flag.arr=%s|%s",l_mspurged_gprs_flag,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "VLR supports SuperCharger l_superCharger_c_gsm.arr=%s|%s",l_superCharger_c_gsm,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "SGSN supports SuperCharger l_superCharger_c_gprs.arr=%s|%s",l_superCharger_c_gprs,__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "GSCF ADRESS %s ipstatus %d|%s",l_gcsf_a,ipstatus,__FUNCTION__);
		//End spell-2100 


		if( !((l_lmsi_c[0] == 0x30) && (strlen(l_lmsi_c) == 1)) )
		{
			::Number lmsi(30);
			lmsi.setValue(l_lmsi_c, strlen(l_lmsi_c));
			obj->setLmsi( lmsi);
		}

		::Number mnrf(30);
		mnrf.setValue(l_mnrf_c, strlen(l_mnrf_c));
		obj->setMnrf(mnrf );

		::Number mcef(30);
		if (l_mcef_c == 1 || l_mcef_c_gprs == 1)
		{
			mcef.setValue((char *)"1", 1);
		}
		else
		{
			mcef.setValue((char *)"0", 1);
		}
		obj->setMcef( mcef );

		::Number mnrrGsm(30);
		mnrrGsm.setValue((char *)l_mnrr_c_gsm, strlen(l_mnrr_c_gsm));
		obj->setMnrrGsm( mnrrGsm );

		if ( strlen(l_mnrr_c_gprs) > 0 )
		{
			::Number mnrrGprs(30);
			mnrrGprs.setValue(l_mnrr_c_gprs, strlen(l_mnrr_c_gprs));
			obj->setMnrrGprs( mnrrGprs );
		}

		if (strlen( l_mnrg_c) > 0 )
		{
			::Number mnrg(30);
			mnrg.setValue(l_mnrg_c, strlen(l_mnrg_c));
			obj->setMnrg( mnrg );
		}


		::Number mscSupportsMtSms(30);
		mscSupportsMtSms.setValue((char *)l_mscSupportsMtSms_c, strlen(l_mscSupportsMtSms_c));
		obj->setMscSupportsMtSms( mscSupportsMtSms );

		::Number msNotReachableFlagForGsm(30);
		msNotReachableFlagForGsm.setValue(l_notreachableGsm_c, strlen(l_notreachableGsm_c));
		obj->setMsNotReachableFlagForGsm( msNotReachableFlagForGsm );


		if(strlen(l_msNotReachableInMscStatus)>0)
		{
			::Number msNotReachableInMscStatus(30);
			msNotReachableInMscStatus.setValue(l_msNotReachableInMscStatus, strlen(l_msNotReachableInMscStatus));
			obj->setMsReachableInMscStatus(msNotReachableInMscStatus);
		}

		if (strlen(l_notreachableGprs_c) > 0)
		{
			::Number msNotReachableFlagForGprs(30);
			msNotReachableFlagForGprs.setValue(l_notreachableGprs_c, strlen(l_notreachableGprs_c));
			obj->setMsNotReachableFlagForGprs( msNotReachableFlagForGprs );
		}

		if (strlen(l_msNotReachableInSgsnStatus) > 0)
		{
			::Number msNotReachableInSgsnStatus(30);
			msNotReachableInSgsnStatus.setValue(l_msNotReachableInSgsnStatus, strlen(l_msNotReachableInSgsnStatus));
			obj->setMsReachableInSgsnStatus( msNotReachableInSgsnStatus );
		}

		::Number RoamingStatus(30);
		RoamingStatus.setValue(l_roaming_status, strlen(l_roaming_status));
		obj->setRoamingStatus( RoamingStatus );

		if (strlen(l_sgsnSupportsMtSms_c) > 0)
		{
			::Number sgsnSupportsMtSms(30);
			sgsnSupportsMtSms.setValue(l_sgsnSupportsMtSms_c, strlen(l_sgsnSupportsMtSms_c));
			obj->setSgsnSuppotsMtSms( sgsnSupportsMtSms );
		}

		::Number networkAccessMode(30);
		networkAccessMode.setValue(l_networkAccessMode_c, strlen(l_networkAccessMode_c));
		obj->setNetworkAccessMode( networkAccessMode );

		::Number ssbarring(30);
		ssbarring.setValue(l_ssbarring_c, strlen(l_ssbarring_c));
		obj->setSsBarring( ssbarring );

		::Number gsm_operatorBarring(30);
		l_operatorBarringGsm_c_len=strlen(l_operatorBarringGsm_c);
		gsm_operatorBarring.setValue(l_operatorBarringGsm_c,l_operatorBarringGsm_c_len);
		obj->setOperatorBarringGSM(gsm_operatorBarring);

		::Number gprs_operatorBarring(30); 
		gprs_operatorBarring.setValue(l_operatorBarringGprs_c, strlen(l_operatorBarringGprs_c));
		obj->setOperatorBarringGPRS(gprs_operatorBarring);


		::Number mtSmsProvisioned(30);
		mtSmsProvisioned.setValue(l_mtSmsProvisioned_c, strlen(l_mtSmsProvisioned_c));
		obj->setMtSmsProvisioned( mtSmsProvisioned );

		::Number traceRequired(30);
		traceRequired.setValue(&l_trace_required[0], 1);
		obj->setTraceRequired( traceRequired );


		::Number vlrSupportsSupercharger(30);
		vlrSupportsSupercharger.setValue(l_superCharger_c_gsm, strlen(l_superCharger_c_gsm));
		obj->setvlrSupportsSuperCharger(vlrSupportsSupercharger);

		::Number sgsnSupportsSupercharger(30);
		sgsnSupportsSupercharger.setValue(l_superCharger_c_gprs, strlen(l_superCharger_c_gprs));
		obj->setsgsnSupportsSuperCharger(sgsnSupportsSupercharger);


		int l_sgsnNumberPresent_value_len;
		if (strlen(l_sgsnNumber_c) > 0) 
		{
			if( !((l_sgsnNumber_c[0] == 0x30) && (strlen(l_sgsnNumber_c) == 1)) )
			{
				char temp_s[SGSNNUM_SIZE +1];
				int sgsnlen = BCDEncoderobj.CharToBCD(temp_s,l_sgsnNumber_c, strlen(l_sgsnNumber_c));
				::Number sgsnNumber(sgsnlen);
				sgsnNumber.setValue(temp_s, sgsnlen);
				obj->setSgsnNumber( sgsnNumber );
			}
			else
			{
				l_sgsnNumberPresent_value_len=1;
				memcpy((char *)l_sgsnNumberPresent_value, "0",l_sgsnNumberPresent_value_len);
				::Number sgsnNumber(30);
				sgsnNumber.setValue(l_sgsnNumberPresent_value, l_sgsnNumberPresent_value_len);
				obj->setSgsnNumber(sgsnNumber);
			}
		}

		if (strlen(l_sgsnAddress_c) > 0)
		{
			if( !((l_sgsnAddress_c[0] == 0x30) && (strlen(l_sgsnAddress_c) == 1)) )
			{	
				char temp_s_addr[SCADDRESS_SIZE +1];
				int sgsn_addr_len = BCDEncoderobj.CharToBCD(temp_s_addr,l_sgsnAddress_c, strlen(l_sgsnAddress_c));
				::Number sgsnAddress(sgsn_addr_len);
				sgsnAddress.setValue(temp_s_addr, sgsn_addr_len);
				obj->setSgsnAddress( sgsnAddress );
			}
		}


		if(ipstatus==1)
		{
			loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " IPSMS STATUS IS ONE setting gscf address|%s",__FUNCTION__);
			loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " gscf  %s|%s",l_gcsf_a,__FUNCTION__);

			char temp_gc[MSCNUM_SIZE +1];
			memset(temp_gc,'\0',sizeof(temp_gc));

			int gsclen = 0;
			gsclen= BCDEncoderobj.CharToBCD(temp_gc,l_gcsf_a,strlen(l_gcsf_a));

			::Number gscfaddr(gsclen);

			loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, "length of gscf %d and gscf %s|%s",gsclen,temp_gc,__FUNCTION__);


			gscfaddr.setValue(temp_gc,gsclen);


			obj->setMscNumber(gscfaddr);


		}
		else
		{
			loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " IPSMS STATUS IS ZERO setting MSC address|%s",__FUNCTION__);

			if( !((l_mscNumber_c[0] == 0x30) && (strlen(l_mscNumber_c)== 1)) )
			{
				loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " INSIDE MSC|%s",__FUNCTION__);
				char temp_m[MSCNUM_SIZE +1];
				int msclen =  0;
				msclen= BCDEncoderobj.CharToBCD(temp_m,l_mscNumber_c, strlen(l_mscNumber_c));
				::Number mscNumber(msclen);
				mscNumber.setValue(temp_m, msclen);
				obj->setMscNumber(mscNumber);
			}     

		}
		int l_vlrNumberPresent_value_len;
		if( !((l_vlrNumber_c[0] == 0x30) && (strlen(l_vlrNumber_c)== 1)) )
		{
			char temp_m[VLR_SIZE +1];
			int vlrlen =  BCDEncoderobj.CharToBCD(temp_m,(char *)l_vlrNumber_c, strlen(l_vlrNumber_c));
			::Number vlrNumber(vlrlen);
			vlrNumber.setValue(temp_m, vlrlen);
			obj->setVlrNumber(vlrNumber);
		}

		else
		{
			l_vlrNumberPresent_value_len=1;
			memcpy((char *)l_vlrNumberPresent_value, "0",l_vlrNumberPresent_value_len);
			::Number vlrNumber(30);
			vlrNumber.setValue(l_vlrNumberPresent_value, l_vlrNumberPresent_value_len);
			obj->setVlrNumber(vlrNumber);
		}
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving getSmsInfo |%s",__FUNCTION__);
		return CODE_SQL_SUCCESS;

	}
	catch (SQLException &ex)
	{   
		pDBHandler->CloseStatement(stmt, conn);		  
		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "Oracle Error code and Message |%d|%s|%s", ex.getErrorCode(), ex.getMessage().c_str(), __FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving getSmsInfo |%s",__FUNCTION__);
		return PROC_CODE_ERROR;
	}
	catch(...)
	{	
		loggerObj.log(LM_ERROR,__FILE__,__LINE__,0,0, "Caught Exception|%s",__FUNCTION__);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving getSmsInfo |%s",__FUNCTION__);
		pDBHandler->Close(rs,stmt,conn);
		return CODE_EXCEPTION;
	}

DATA_NOT_FOUND: 
	{
		loggerObj.log(LM_CRITICAL,__FILE__,__LINE__,0,0, "No Data Found at Query Index [%d]|%s",queryIndex,__FUNCTION__);
		if((isJumpTrue== 0)&& (flag_gsm == 0))
		{
			::Number zero(1);
			zero.setValue("0",1);
			obj->setmtOdbBarring(zero);
		}
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving getSmsInfo |%s",__FUNCTION__);
		return ROWS_NOT_FOUND;
	}

ABSENT_SUBSCRIBER:
	{
		loggerObj.log(LM_CRITICAL,__FILE__,__LINE__,0,0, " Absent Subscriber,Row not found in GSM/GPRS TR table for the Subscriber at Query Index [%d]|%s",queryIndex,__FUNCTION__);
		if ((l_operatorBarring_value) & (1048576))
		{
			memcpy(l_mtodbBarring_value, "1",1);
			loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " BAIC ODB value is present in HLR_MT_ODB_DATA table ...%s|%s",l_mtodbBarring_value,__FUNCTION__);
		}
		else
		{
			loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " BAIC ODB value is not present in HLR_MT_ODB_DATA ...%s|%s",l_mtodbBarring_value,__FUNCTION__);

			memcpy(l_mtodbBarring_value, "0",1);
		}

		::Number mtOdbBarring(30);

		mtOdbBarring.setValue(l_mtodbBarring_value, 1);
		obj->setmtOdbBarring(mtOdbBarring);
		loggerObj.log(LM_DEBUG,__FILE__,__LINE__,0,0, " Leaving getSmsInfo |%s",__FUNCTION__);
		return ABSENT_SUBSCRIBER;
	}
}

void SmsmDbService::getMtSubGsmInfo(char* l_imsi_c,char* l_msisdn_c,unsigned int l_gsm_active_roaming_subs_profile_id)
{
	/* loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"Entered into getMtSubGsmInfo | %s", __FUNCTION__);
	   std::string query =
	   "SELECT COALESCE(IMSI, '0') AS IMSI, "
	   "COALESCE(MSISDN, '0') AS MSISDN, "
	   "COALESCE(SUBSCRIBER_STATE, '0') AS SUBSCRIBER_STATE, "
	   "COALESCE(TRACE_REQUIRED, '0') AS TRACE_REQUIRED, "
	   "COALESCE(NETWORK_ACCESS_MODE, '0') AS NETWORK_ACCESS_MODE, "
	   "COALESCE(NETWORK_ID, '0') AS NETWORK_ID "
	   "FROM HLR_MT_SUB_GSM_INFO "
	   "WHERE (IMSI = '" + std::string(l_imsi_c) + "' OR MSISDN = '" + std::string(l_msisdn_c) + "') "
	   "AND ACTIVE_ROAMING_PROFILE_ID = " + std::to_string(l_gsm_active_roaming_subs_profile_id);*/

}



/*GPRS_QUERY:

  loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,
  "Active_roaming_profile_id for GPRS is = %d | %s",
  l_gprs_active_roaming_subs_profile_id, __FUNCTION__);

// Construct the PostgreSQL query string
std::string query =
"SELECT COALESCE(IMSI, '0') AS IMSI, "
"COALESCE(MSISDN, '0') AS MSISDN, "
"COALESCE(SUBSCRIBER_STATE, '0') AS SUBSCRIBER_STATE, "
"COALESCE(TRACE_REQUIRED, '0') AS TRACE_REQUIRED, "
"COALESCE(NETWORK_ACCESS_MODE, '0') AS NETWORK_ACCESS_MODE, "
"COALESCE(NETWORK_ID, '0') AS NETWORK_ID "
"FROM HLR_MT_SUB_GPRS_INFO "
"WHERE (IMSI = '" + std::string(l_imsi_c) + "' OR MSISDN = '" + std::string(l_msisdn_c) + "') "
"AND ACTIVE_ROAMING_PROFILE_ID = '" + std::to_string(l_gprs_active_roaming_subs_profile_id) + "'";

DbUtil::ResultSet rows = DbUtil::executeQuery(query);

if (rows.empty()) {
loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0,
"Data Not found for Active_roaming_profile_id = %d | %s",
l_gprs_active_roaming_subs_profile_id, __FUNCTION__);
return;
}

const auto& row = rows[0];

char l_imsi[32], l_msisdn[32], l_subscriber_state_gprs_c[16],
l_trace_required_gprs[16], l_networkAccessMode_c[16], l_NetworkId[32];

strncpy(l_imsi, row.at("imsi").c_str(), sizeof(l_imsi) - 1); l_imsi[sizeof(l_imsi) - 1] = '\0';
strncpy(l_msisdn, row.at("msisdn").c_str(), sizeof(l_msisdn) - 1); l_msisdn[sizeof(l_msisdn) - 1] = '\0';
strncpy(l_subscriber_state_gprs_c, row.at("subscriber_state").c_str(), sizeof(l_subscriber_state_gprs_c) - 1); l_subscriber_state_gprs_c[sizeof(l_subscriber_state_gprs_c) - 1] = '\0';
strncpy(l_trace_required_gprs, row.at("trace_required").c_str(), sizeof(l_trace_required_gprs) - 1); l_trace_required_gprs[sizeof(l_trace_required_gprs) - 1] = '\0';
strncpy(l_networkAccessMode_c, row.at("network_access_mode").c_str(), sizeof(l_networkAccessMode_c) - 1); l_networkAccessMode_c[sizeof(l_networkAccessMode_c) - 1] = '\0';
strncpy(l_NetworkId, row.at("network_id").c_str(), sizeof(l_NetworkId) - 1); l_NetworkId[sizeof(l_NetworkId) - 1] = '\0';

loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,
"Retrieved GPRS Info: <IMSI=%s> <MSISDN=%s> <SUB_STATE=%s> <TRACE_REQ=%s> "
"<NET_ACCESS_MODE=%s> <NETWORK_ID=%s> | %s",
l_imsi, l_msisdn, l_subscriber_state_gprs_c, l_trace_required_gprs,
l_networkAccessMode_c, l_NetworkId, __FUNCTION__);*/

/*nNumber SmsmDbService::getNumberType(char data[]) {
  nNumber value(30);
  value.setValue(data, strlen(data));
  return value;
  }*/

#ifdef _TEST_SMSDBSERVICE_
/*::Number bcdEncodedNumber(std::string value) {
  BCDEncoder BCDEncoderobj;
  char bcd_encoded_string[value.length()+1];
  int  bcd_encoded_string_length = BCDEncoderobj.CharToBCD(bcd_encoded_string,value, value.length());
  Number numberBCDEncoded(bcd_encoded_string_length);
  numberBCDEncoded.setValue(bcd_encoded_string,bcd_encoded_string_length);
  return numberBCDEncoded;
  }*/
int main() 
{
  SmsmDbService sds;
  BCDEncoder BCDEncoderobj;
  PgConnectionPool::Config config
  {
    "host=192.168.144.101 port=5432 dbname=postgres user=postgres password=postgres options='-c search_path=hlrdbnew_qc'",
    2, 10, 2,
    std::chrono::seconds(5),
    std::chrono::seconds(30),
    3,
    std::chrono::milliseconds(500)
  };
  try 
  {
    DbUtil::init(config);

  }
  catch (const std::exception& ex) 
  {
    std::cerr << "[FATAL] Failed to initialize DB connection pool: "<< ex.what() << std::endl;
		return EXIT_FAILURE;   // terminate binary
  } catch (...) 
  {
    std::cerr << "[FATAL] Unknown error during DB init" << std::endl;
    return EXIT_FAILURE;   // terminate binary
  }

  if(DbUtil::isAlive())
  {
    std::cout << "DB is Active" << std::endl; 
  }
  else
  {
    std::cout << "DB not active " << std::endl;
    exit(0);
  }
  loggerObj.logInitialize("./Logs/SMSM",4,1,1024*1000*100,"smsm", 1024);
  DataFromDb smsSubscriberProfile;
  char imsi[]="334110000000019";
  char msisdn_c[]="529900000048";
  Subscriber subscriber1(imsi, msisdn_c);
  subscriber1.getMsisdn(true);
  smsSubscriberProfile.setMsisdn(subscriber1.number_msisdn);
  sds.getSmsInfo(&smsSubscriberProfile, 0);

  UpdateDb updateDbObject;
  updateDbObject.setImsi("334110000000019");   
  updateDbObject.setMsisdn("522222798068");  //53232323232373938303  
  updateDbObject.setLmsi("123400000000000");
  updateDbObject.setMnrf("M");
  updateDbObject.setMcef("M");
  updateDbObject.setMnrrGsm("N");
  updateDbObject.setMnrg("1");
  updateDbObject.setMsNotReachableFlagForGsm("0");
  updateDbObject.setMsNotReachableFlagForGprs("0");
  updateDbObject.setSgsnAddress("1234567890");
  updateDbObject.setSgsnNumber("54321");
  //updateDbObject.setMsPurgedNonGprsFlag("0");
  //updateDbObject.setMsPurgedGprsFlag("0");
  
  sds.updateSmsInfo(&updateDbObject);
  cout<<"after  calling Updatesmsmingo"<<endl; 
}
Subscriber::Subscriber() { }
Subscriber::Subscriber(char imsi[], char msisdn[] ) 
{
  this->imsi=string(imsi);
  this->msisdn=string(msisdn);
  l_gsm_active_roaming_subs_profile_id = 0;
  l_gprs_active_roaming_subs_profile_id =0;
}

nNumber Subscriber::getMsisdn(bool encoded){
  bs byteString;
  char msisdn[30];
  char msisdn_encoded[30];
  memset(msisdn_encoded,'\0',30);
  int byteStringLength=0;
  snprintf(msisdn, sizeof(msisdn), "%s", this->msisdn.c_str());
  cout << "Value of msisdn char array: " << msisdn << endl;
  if(encoded) {
    funCharToBcd(msisdn,&byteString);
    byteStringLength = byteString.getValue(msisdn_encoded);
    //TODO: Error Handle 
  }
  number_msisdn.setValue(msisdn_encoded, byteStringLength );
  return number_msisdn;

}
 

int Subscriber::loadObjectFromDB() 
{
  //TO DO: Validate if imsi or msisdn are populated, without them next of the process cannot continue;
  //The following methods must be called in the order as each function gets a parameter to fetch other parameters.
  //try {
  getGsmActiveRoamingSubProfileIdFromDB(); //queryIndex=1;
  getOperatorBarringValueFromDB(); //queryIndex=2;
  getMtSubGsmInfo();
  getMtPlmeGprsInfo();
  getMtPlmeStatInfo();
  getTranctionSubInfo();
  getTranctionGsmBearerTeleData();
  getTranctionGprsBearerTeleData();
  checkGsmSmsBearerServiceProvisioned();
  checkGprsSmsBaearerServiceProvisioned();
  getIpsmgwAndScfInfo();
  isMsisdnFoundInMwd();
  isScAddressFoundInMwd();
  getScAdresses();
  getDistinctMsisdnInMwd();
  getSmsFilterEnabledParam();//routeSmsToHplmnSmsc query 1
  getLiInterceptRequiredParam();//routeSmsToHplmnSmsc query 2
  isLawfulInterceptEnabled();//routeSmsToHplmnSmsc query 3
  // getMwdCount();//updateMwdList  query 1
  //insertMwdInfo();
	//}catch(
  return 0;
}
int UpdateSubscriberDb::loadObjectForUpDateData()
{
  //int  result;
  //cout << "##############l_imsi_c:" << l_imsi_c << endl;
  //cout << "##############l_msisdn_c:" << l_msisdn_c << endl;
 // cout << "############## l_mnrf_c:" << l_mnrf_c << endl;
     
   updateSubscriberGsmInfo();
  cout<<"i am in loadObjectForUpDateData----------------"<<endl;  
}

//TODO: This function is supposed to be private
//queryIndex=1 in the older Code
int Subscriber::getGsmActiveRoamingSubProfileIdFromDB()
{
  const std::string query =
  std::string("SELECT COALESCE (ACTIVE_ROAMING_PROFILE_ID, 0) as ACTIVE_ROAMING_PROFILE_ID ")
  + " FROM HLR_TR_SUB_GSM_INFO " + " WHERE IMSI = '" + std::string(imsi) +"' OR MSISDN = '" + std::string(msisdn) +"';";
  l_gsm_active_roaming_subs_profile_id = DbUtil::getAValue(query);
  return l_gsm_active_roaming_subs_profile_id;
}

//TODO: This function is supposed to be private
//queryIndex=2;
/*Ideally this function must return unsigned int as per existing logic
  However, where there no records in database, flow is different. Some jump should happen
  So, I want to send negative value to indicate "NO ROWS FOUND" Error 
  and then in the called location I use value to typecast to unsigned int if positive
  or otherwise jump if negative
  */
int Subscriber::getOperatorBarringValueFromDB() 
{
  ostringstream query;
  query <<
  "SELECT COALESCE(GENERAL_ODB_DATA, 4177526784) AS GENERAL_ODB_DATA "
  << " FROM HLR_MT_SUB_ODB_DATA "<< " WHERE (IMSI = '" << imsi << "' "
  << " OR MSISDN = '" << msisdn << "') "
  << " AND (ACTIVE_ROAMING_PROFILE_ID = " <<  l_gsm_active_roaming_subs_profile_id 
  << " AND ACTIVE_ROAMING_PROFILE_ID_GPRS = " << l_gprs_active_roaming_subs_profile_id << ")";
  l_operatorBarring_value = DbUtil::getAValue(query.str());
  // if value is zero or some sentinel, handle it
  if (l_operatorBarring_value < 0) 
  {
    //TO DO :  //goto GPRS_QUERY; has to be implemented.
  }
  return l_operatorBarring_value;
}
//queryIndex=2;
//TODO: This function is supposed to be private
int Subscriber::getMtSubGsmInfo() 
{
  ostringstream query;
  query <<
  "SELECT COALESCE(IMSI, '0') AS IMSI, "
  << "COALESCE(MSISDN, '0') AS MSISDN, "
  << "COALESCE(SUBSCRIBER_STATE, '0') AS SUBSCRIBER_STATE, "
  << "COALESCE(TRACE_REQUIRED, '0') AS TRACE_REQUIRED, "
  << "COALESCE(NETWORK_ACCESS_MODE, '0') AS NETWORK_ACCESS_MODE, "
  << "COALESCE(NETWORK_ID, '0') AS NETWORK_ID "
  << "FROM HLR_MT_SUB_GSM_INFO "
  << "WHERE (IMSI = '" << imsi << "' OR MSISDN = '" << msisdn << "') "
  << "AND ACTIVE_ROAMING_PROFILE_ID = " << l_gsm_active_roaming_subs_profile_id;
  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"Executing getMtSubGsmInfo Query: %s | %s",query.str().c_str(),
  __FUNCTION__);
  // Execute query
  DbUtil::ResultSet result = DbUtil::executeQuery(query.str());
  if (result.empty())
  {
    loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "No rows found in getMtSubGsmInfo for IMSI=%s, MSISDN=%s | %s", 
    imsi.c_str(), msisdn.c_str(), __FUNCTION__);
    getMtSubGprsInof();
    //TODO: Throw Error
  }
  if(result.size() > 0 ) 
  {
    const DbUtil::Row& row = result.front();
    l_subscriber_state_gsm_c =row.at("subscriber_state");
    l_trace_required_gsm = row.at("trace_required");
    l_networkAccessMode_c = row.at("network_access_mode");
    l_NetworkId = row.at("network_id");
    loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0,"Subscriber State=%s, Trace_Request=%s, Network_Access_Mode=%s, NetworkId=%s | %s",l_subscriber_state_gsm_c.c_str(),l_trace_required_gsm.c_str(),l_networkAccessMode_c.c_str(),
    l_NetworkId.c_str(),__FUNCTION__);  
   }
   return 0;
}

int Subscriber::getMtPlmeGprsInfo()
{ 
  if (l_NetworkId.empty()) 
  {
    cout << "NETWORK_ID is empty!" << endl;
    loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "NETWORK_ID is empty| %s", __FUNCTION__);
    getMtSubGprsInof();
    //TODO : GPRS_QUERY
  }  
  ostringstream query;
  query<<
  "SELECT COALESCE(HLR_NUMBER, ' ') AS hlr_number, "
  << "COALESCE(SMSC_NUMBER, ' ') AS smsc_number "
  << "FROM HLR_MT_PLMN_DETAILS "
  << "WHERE NETWORK_ID = '"<<l_NetworkId<<"' "
  << "AND (CC::text || NDC::text) = SUBSTRING('"<<msisdn<<"' FROM 1 FOR LENGTH(CC::text || NDC::text))";
  DbUtil::ResultSet result = DbUtil::executeQuery(query.str());
  if (result.empty())
  {
    loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "No rows found in getMtPlmeGprsInfo for  | %s", __FUNCTION__);
    //goto GPRS_QUERY;
    getMtSubGprsInof();
  }
  const DbUtil::Row& row = result.front();
  l_hlr_c = row.at("hlr_number");
  l_smsc_c = row.at("smsc_number"); 
  loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"<HLR=%s> <SMSC=%s> | %s",l_hlr_c.c_str(), l_smsc_c.c_str(), __FUNCTION__);
  return 0;
}
int Subscriber::getMtSubGprsInof()
{
  ostringstream query;
  query <<
  "SELECT COALESCE(IMSI, '0') AS IMSI, "
  << "COALESCE(MSISDN, '0') AS MSISDN, "
  << "COALESCE(SUBSCRIBER_STATE, '0') AS SUBSCRIBER_STATE, "
  << "COALESCE(TRACE_REQUIRED, '0') AS TRACE_REQUIRED, "
  << "COALESCE(NETWORK_ACCESS_MODE, '0') AS NETWORK_ACCESS_MODE, "
  << "COALESCE(CAST(NETWORK_ID AS TEXT), '0') AS NETWORK_ID "
  << "FROM HLR_MT_SUB_GPRS_INFO "
  << "WHERE (IMSI = '" << imsi << "' OR MSISDN = '" << msisdn << "') "
  << "AND ACTIVE_ROAMING_PROFILE_ID = " << l_gprs_active_roaming_subs_profile_id;
  DbUtil::ResultSet result = DbUtil::executeQuery(query.str());
  loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0," %s | %s",query.str().c_str(),  __FUNCTION__);
  if (result.empty())
  {
    loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "No rows found in getMtSubGprsInof for IMSI=%s, MSISDN=%s | %s", 
    imsi.c_str(), msisdn.c_str(), __FUNCTION__);
    //return;
    // TODO::STAT2
  }
  if(result.size() > 0 ) 
  {
    const DbUtil::Row& row = result.front();
    l_subscriber_state_gsm_c =row.at("subscriber_state");
    l_trace_required_gsm = row.at("trace_required");
    l_networkAccessMode_c = row.at("network_access_mode");
    l_NetworkId = row.at("network_id");
    loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0,"Subscriber State=%s, Trace_Request=%s, Network_Access_Mode=%s, NetworkId    =%s | %s",l_subscriber_state_gsm_c.c_str(),l_trace_required_gsm.c_str(),l_networkAccessMode_c.c_str(),
				l_NetworkId.c_str(),__FUNCTION__);  
  }
  return 0;
}
int Subscriber::getMtPlmeStatInfo()
{
  if (l_NetworkId.empty()) 
  {
    loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "getMtPlmeStatInfo in NETWORK_ID is empty| %s", __FUNCTION__);
    // getMtSubGprsInof();
    //TODO : STAT2
  } 
  ostringstream query;
  query<<
  "SELECT COALESCE(HLR_NUMBER, ' ') AS hlr_number, "
  << "COALESCE(SMSC_NUMBER, ' ') AS smsc_number "
  << "FROM HLR_MT_PLMN_DETAILS "
  << "WHERE NETWORK_ID = '"<<l_NetworkId<<"' "
  << "AND (CC::text || NDC::text) = SUBSTRING('"<<msisdn<<"' FROM 1 FOR LENGTH(CC::text || NDC::text))";
  DbUtil::ResultSet result = DbUtil::executeQuery(query.str());
  if (result.empty())
  {
    cout<<"we get query is empty"<<endl;
    loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "No rows found in getMtPlmeStatInfo for  | %s", __FUNCTION__);
    //goto STAT2;
  }
  DbUtil::Row& row = result.front();
  hlr_result = row.at("hlr_number");
  smsc_result = row.at("smsc_number"); 
  loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"<HLR=%s> <SMSC=%s> | %s",hlr_result.c_str(), smsc_result.c_str(),
   __FUNCTION__);
  return 0;
}
int Subscriber::getTranctionSubInfo()
{
  ostringstream query;
  query <<
  "SELECT "
  << "COALESCE(LMSI, '0') AS lmsi, "
  << "COALESCE(MNRF, '0') AS mnrf, "
  << "COALESCE(MCEF, '0') AS mcef, "
  << "COALESCE(MNRR, '0') AS mnrr, "
  << "COALESCE(MSC_NUMBER, '0') AS msc_number, "
  << "COALESCE(NOTREACHABLE_FLAG, '0') AS notreachable_flag, "
  << "COALESCE(MSC_AREA_RESTRIC_FLAG, '0') AS msc_area_restric_flag, "
  << "COALESCE(MSISDN, '0') AS msisdn, "
  << "COALESCE(ROAMING_STATUS, '0') AS roaming_status, "
  << "COALESCE(MSPURGED_NONGPRS_FLAG, '0') AS mspurged_nongprs_flag, "
  << "COALESCE(VLR_SUPPORTS_SUPER_CHARGER, '0') AS vlr_supports_super_charger, "
  << "COALESCE(VLR_NUMBER, '0') AS vlr_number, "
  << "COALESCE(ODB_INDUCED_BARR_DATA, '0') AS odb_induced_barr_data "
  << "FROM HLR_TR_SUB_GSM_INFO "
  << "WHERE (IMSI = '" << imsi << "' OR MSISDN = '" << msisdn << "')";
  DbUtil::ResultSet result = DbUtil::executeQuery(query.str());
  if (result.empty())
  {
    loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "No rows found in getTranctionSubInfo for  | %s", __FUNCTION__);
    //goto QUERY2;
  }
  DbUtil::Row& row = result.front();
  lmsi = row.at("lmsi");
  mnrf = row.at("mnrf");
  mcef = row.at("mcef");
  mnrrGsm = row.at("mnrr");
  l_mscNumber_result    =row.at("msc_number");
  l_notreachable_result =row.at("notreachable_flag");
  l_msStatus_result     =row.at("msc_area_restric_flag");
  l_msisdn_result       =row.at("msisdn");
  l_roaming_result      =row.at("roaming_status");
  l_mspurged_result     =row.at("mspurged_nongprs_flag");
  l_superCharger_result =row.at("vlr_supports_super_charger");
  l_vlrNumber_result    =row.at("vlr_number");
  l_gsmOdbinducedBarring =stoul(row.at("odb_induced_barr_data"));
  return 0;
}

int Subscriber::getTranctionGsmBearerTeleData()
{
  ostringstream query;
  query << "SELECT COUNT(1) AS smsgsm_count "
  << "FROM HLR_TR_GSM_BEARER_TELE_DATA "
  << "WHERE IMSI = '" << imsi << "' AND BEARER_TELE_CODE = '33' "
  << "AND BEARER_TELE_TYPE = '2' AND BEARER_TELE_STATUS = '1' "
  << "AND ACTIVE_ROAMING_PROFILE_ID = " << l_gsm_active_roaming_subs_profile_id;
  DbUtil::ResultSet result = DbUtil::executeQuery(query.str());
  DbUtil::Row& row = result.front();
  string countStr = row.at("smsgsm_count");
  int count = std::stoi(countStr);
  l_mscSupportsMtSms_c = countStr;
  if (count > 0) 
  {
    loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"Subscriber HAS SMS bearer services | Count: %d | %s",count,
     __FUNCTION__);
    return CODE_SQL_SUCCESS;
  } 
  else
  {
    loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0, "No Data Found in getTranctionGsmBearerTeleData | %s", __FUNCTION__);
    loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"Subscriber HAS desont have  SMS bearer services | Count: %d | %s",
    count, __FUNCTION__);
    return ROWS_NOT_FOUND;
  }
}
int Subscriber::getTranctionGprsBearerTeleData()
{
  ostringstream query;
  query << "SELECT COUNT(1) AS smsgprs_count "  
  << "FROM HLR_TR_GPRS_BEARER_TELE_DATA "
  << "WHERE IMSI = '" << imsi << "' AND BEARER_TELE_CODE = '33' "
  << "AND BEARER_TELE_TYPE = '2' AND BEARER_TELE_STATUS = '1' "
  << "AND ACTIVE_ROAMING_PROFILE_ID = " << l_gprs_active_roaming_subs_profile_id;
  DbUtil::ResultSet result = DbUtil::executeQuery(query.str());
  if (result.empty()) 
  {
    loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0,"No Data Found in getTranctionGprsBearerTeleData - empty result | %s",     __FUNCTION__);
    return ROWS_NOT_FOUND;
  }
  DbUtil::Row& row = result.front();
  string countStr = row.at("smsgprs_count");  
  int count = std::stoi(countStr);
  l_sgsnSupportsMtSms_c = countStr;
  if (count > 0) 
  {
    loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"Subscriber HAS SMS bearer services | Count: %d | %s", count, 
    __FUNCTION__);
    return CODE_SQL_SUCCESS; 
  }
  else   
  {
    loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0,"No Data Found in getTranctionGprsBearerTeleData | %s", __FUNCTION__);
    loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"Subscriber doesn't have SMS bearer services | Count: %d | %s", count,
    __FUNCTION__);
    return ROWS_NOT_FOUND;
  }
}

int Subscriber::checkGsmSmsBearerServiceProvisioned()
{ 
	ostringstream query;
	query << "SELECT COUNT(1) AS mt_sms_gsm_provisioned_count  "
		<< "FROM HLR_MT_SUB_GSM_INFO A, HLR_TR_GSM_BEARER_TELE_DATA B "
		<< "WHERE (A.IMSI = '" << imsi << "' OR A.MSISDN = '" << msisdn << "') "
		<< "AND A.ACTIVE_ROAMING_PROFILE_ID = " << l_gsm_active_roaming_subs_profile_id << " "
		<< "AND A.IMSI = B.IMSI "
		<< "AND A.ACTIVE_ROAMING_PROFILE_ID = B.ACTIVE_ROAMING_PROFILE_ID "
		<< "AND B.BEARER_TELE_CODE = '33' "  
		<< "AND B.BEARER_TELE_TYPE = '2'";   

	DbUtil::ResultSet result = DbUtil::executeQuery(query.str());

	DbUtil::Row& row = result.front();
	string countStr = row.at("mt_sms_gsm_provisioned_count");  
	int count = std::stoi(countStr);

	l_mtSmsGsmProvisioned_c = countStr;

	if (count > 0) 
	{
		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0," checkGsmSmsBearerServiceProvisioned Subscriber HAS SMS bearer services | Count: %d | %s",count, __FUNCTION__);
		return CODE_SQL_SUCCESS;
	} 
	else
	{
		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0,"No Data Found in checkGsmSmsBearerServiceProvisioned | %s", __FUNCTION__);
		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"Subscriber doesn't have SMS bearer services | Count: %d | %s", count, __FUNCTION__);
		return ROWS_NOT_FOUND; 
	} 
}
int Subscriber::checkGprsSmsBaearerServiceProvisioned()
{
  ostringstream query;
	query << 
		"SELECT COUNT(1) AS mt_gprs_provisioned_count "
		<< "FROM HLR_MT_SUB_GPRS_INFO A, HLR_TR_GPRS_BEARER_TELE_DATA B "
		<< "WHERE (A.IMSI = '" << imsi << "' OR A.MSISDN = '" << msisdn << "') "
		<< "AND A.ACTIVE_ROAMING_PROFILE_ID = " << l_gprs_active_roaming_subs_profile_id << " "
		<< "AND A.IMSI = B.IMSI "
		<< "AND A.ACTIVE_ROAMING_PROFILE_ID = B.ACTIVE_ROAMING_PROFILE_ID "
		<< "AND B.BEARER_TELE_CODE = '33' "
		<< "AND B.BEARER_TELE_TYPE = '2'";

	DbUtil::ResultSet result = DbUtil::executeQuery(query.str());
	DbUtil::Row& row = result.front();
	string countStr = row.at("mt_gprs_provisioned_count"); 
	int count = std::stoi(countStr);
	l_mtSmsGprsProvisioned_c=countStr;
	if (count > 0) 
	{
		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0," checkGprsSmsBaearerServiceProvisioned Subscriber HAS SMS bearer services | Count: %d | %s",count, __FUNCTION__);
		return CODE_SQL_SUCCESS;
	} 
	else
	{
		loggerObj.log(LM_CRITICAL, __FILE__, __LINE__, 0, 0,"No Data Found in checkGprsSmsBaearerServiceProvisioned | %s", __FUNCTION__);
		loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"checkGprsSmsBaearerServiceProvisioned Subscriber doesn't have SMS bearer services | Count: %d | %s", count, __FUNCTION__);
		return ROWS_NOT_FOUND; 
	}
}

int Subscriber::getIpsmgwAndScfInfo()
{
  
  ostringstream query;
  query <<
  "SELECT COALESCE(\"IPSMGW_STATUS\", 0) AS ipsmgw_status, "
  << "COALESCE(\"GSMSCF_ADDRESS\", '0') AS gsmscf_address "
  << "FROM HLR_TR_SUB_GSM_INFO "
  << "WHERE msisdn = '" << msisdn << "'";
  DbUtil::ResultSet result = DbUtil::executeQuery(query.str());
  if (result.empty())
  {
    loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0, "No rows found in getTranctionSubInfo for  | %s", __FUNCTION__);
    //FROM HLR_MT_SUB_GSM_INFO A, HLR_MT_SUB_CALLBARR_INFO B, HLR_TR_SUB_GSM_INFO C
   fetchCallBarringDetails();
  }
  DbUtil::Row& row = result.front();
  l_ipsmgwStatus  = row.at("ipsmgw_status");
  l_gsmscfAddress = row .at("gsmscf_address");
  
  loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"<l_ipsmgwStatus=%s> <l_gsmscfAddress=%s> | %s",l_ipsmgwStatus.c_str(), 
  l_gsmscfAddress.c_str(), __FUNCTION__);
  return 0;
}

int Subscriber::fetchCallBarringDetails()
{
  
  cout<<" enter into fetchCallBarringDetails"<<endl; 
  ostringstream query;
  query <<
  "SELECT COUNT(1) AS call_barring_count "
  << "FROM HLR_MT_SUB_GSM_INFO A, HLR_MT_SUB_CALLBARR_INFO B, HLR_TR_SUB_GSM_INFO C "
  << "WHERE A.MSISDN = '" << msisdn << "' "
  << "AND A.MSISDN = B.MSISDN "
  << "AND A.MSISDN = C.MSISDN "
  << "AND A.BARRING_SS = '1' "
  << "AND (B.SS_CODE IN (144, 153, 154) OR (B.SS_CODE = 155 AND C.ROAMING_STATUS != '0')) "
  << "AND B.BASICSERVICEGROUP_CODE = 32 "
  << "AND B.BASICSERVICEGROUP_TYPE = '2' "
  << "AND B.SS_STATUS = 7 "
  << "AND B.ACTIVE_ROAMING_PROFILE_ID = " << l_gsm_active_roaming_subs_profile_id << " "
  << "AND A.ACTIVE_ROAMING_PROFILE_ID = " << l_gsm_active_roaming_subs_profile_id;
  
  DbUtil::ResultSet result = DbUtil::executeQuery(query.str());
  DbUtil::Row& row = result.front();
  string countStr = row.at("call_barring_count"); 
  int count = std::stoi(countStr);
  l_ssbarring=countStr;
  if(count > 0) 
  {
    loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0," fetchCallBarringDetails Subscriber HAS SMS bearer services | Count: %d     | %s",count, __FUNCTION__);
   return CODE_SQL_SUCCESS;
  } 
  else
  {  
  loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"<l_ssbarring=%s>  | %s",l_ssbarring.c_str(), __FUNCTION__);
  return 0;
  } 
}
int Subscriber::isMsisdnFoundInMwd()
{
  ostringstream query;
  query <<
	"SELECT COUNT(1) AS msisdn_count FROM HLR_TR_MWD_INFO "
	"WHERE MSISDN = '"<<msisdn << "' AND STATUS = 'N'";
  DbUtil::ResultSet result = DbUtil::executeQuery(query.str());
  DbUtil::Row& row = result.front();
  string countStr = row.at("msisdn_count"); 
  l_count_i=countStr;
  int count = std::stoi(countStr);
  if (row.empty())
  {
    loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"DATA_NOT_FOUND | MSISDN: %s |%s", count, __FUNCTION__);
    return ROWS_NOT_FOUND;
  }

  if(count > 0)
  {
    loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"Given MSISDN is found | Count: %d |%s", count, __FUNCTION__);
    return CODE_SQL_SUCCESS;
  }
  else
  {
    loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"Given MSISDN is not found in Database | Count: %d |%s", count, __FUNCTION__);
    return ROWS_NOT_FOUND;
  }
}
int Subscriber::isScAddressFoundInMwd()
{
  ostringstream query;
  query <<
  "SELECT COUNT(1) AS mwd_count "
  << "FROM HLR_TR_MWD_INFO "
  << "WHERE MSISDN = '" << msisdn << "' "
  << "AND SVC_CENTRE_ADDRESS = '" << l_scAddress_c << "' "
  << "AND STATUS = 'N'"; 
  DbUtil::ResultSet result = DbUtil::executeQuery(query.str());
  DbUtil::Row& row = result.front();
  string countStr = row.at("mwd_count");
  l_count_i =countStr; 
  int count = std::stoi(countStr);
  if (row.empty())
  {
    loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"count %s |%s",count,__FUNCTION__);
    return ROWS_NOT_FOUND;
  }
  if(count > 0)
  {
   loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"isScAddressFoundInMwd Count: %d |%s",count, __FUNCTION__);   
   return CODE_SQL_SUCCESS;
  }
  else
  {
   loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"isScAddressFoundInMwd  Count: %d |%s",count, __FUNCTION__);
   //while executin entering 
   return ROWS_NOT_FOUND;
  }
}
int Subscriber::getScAdresses()
{
  ostringstream query;
  query <<
  "SELECT COALESCE(SVC_CENTRE_ADDRESS, '0') AS sc_address "
  << "FROM HLR_TR_MWD_INFO "
  << "WHERE MSISDN::text = '" << msisdn << "' "
  << "AND STATUS = 'N'";
  DbUtil::ResultSet result = DbUtil::executeQuery(query.str());
  if (result.empty())
  {
    loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"No rows found in getScAdresses for MSISDN=%s | %s",msisdn,__FUNCTION__);
    return ROWS_NOT_FOUND;
  }
  const DbUtil::Row& row = result.front();
  string l_scAddress_c = row.at("sc_address");
  loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"<SVC_CENTRE_ADDRESS=%s> | %s",l_scAddress_c.c_str(), __FUNCTION__);
 //while executin entering  hear
  return CODE_SQL_SUCCESS;
}
int Subscriber::getDistinctMsisdnInMwd()
{
  ostringstream query;
  query << "SELECT DISTINCT msisdn "
  << "FROM HLR_TR_MWD_INFO "
  << "WHERE status = 'N'";
  // Execute query
  DbUtil::ResultSet result = DbUtil::executeQuery(query.str());
  if (result.empty())
  {
    cout << "Entered empty block in getDistinctMsisdnInMwd" << endl;
    loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"No Data found | %s", __FUNCTION__);
    return ROWS_NOT_FOUND;
  }
  int count = 0;
  for (const auto &row : result)
  {
    msisdn = row.at("msisdn");
    if (!msisdn.empty() && msisdn != "0")
    {
     // loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"List of MSISDN found:  %s | %s",msisdn, __FUNCTION__);
      count++;
    }
  }
  if (count > 0)
  {
    loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"List of MSISDN found | Count: %d | %s",count, __FUNCTION__);
    return CODE_SQL_SUCCESS;
  }
  else
  {
    cout << "Entered else condition — No valid MSISDN found" << endl;
    loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0,"No valid MSISDN found | %s",__FUNCTION__);
    return ROWS_NOT_FOUND;
  }
}
int Subscriber::getSmsFilterEnabledParam()
{
  ostringstream query;
  query <<
  "SELECT COALESCE(VALUE, ' ') AS value "
  << "FROM HLR_MT_SYS_PARAMS "
  << "WHERE PARAMETER = 'SMS_FILTER_ENABLED_IN_SMSC'";
  DbUtil::ResultSet result = DbUtil::executeQuery(query.str());
  if(result.empty())
  {
  loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"No Data found | %s", __FUNCTION__);
  return ROWS_NOT_FOUND;
  }
  const DbUtil::Row& row= result.front();
  l_sms_filter_enabled= row.at("value");
  loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"<l_sms_filter_enabled=%s> | %s",l_sms_filter_enabled.c_str(), __FUNCTION__);
  return 0;
}
int Subscriber::getLiInterceptRequiredParam()
{
  ostringstream query;
  query<<   
  "SELECT COALESCE(VALUE,' ') AS value "
  <<"FROM HLR_MT_SYS_PARAMS " 
  << "WHERE PARAMETER = 'NON_HPLMN_MT_SMS_LI_REQUIRED'";
  DbUtil::ResultSet result=DbUtil::executeQuery(query.str());
  if(result.empty())
  {
    cout<<"getLiInterceptRequiredParam empty"<<endl;
    loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"No Data found | %s", __FUNCTION__);
    return ROWS_NOT_FOUND;
  }
  const DbUtil::Row& row = result.front();
  l_required_value = row.at("value");
  loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"<l_required_value=%s> | %s",l_sms_filter_enabled.c_str(), __FUNCTION__);
  //while exectin entering hear
  return 0;
}

int Subscriber::isLawfulInterceptEnabled()
{
  ostringstream query;
  query <<
  "SELECT COALESCE(COUNT(case_id), 0) AS li_enabled_count "
  << "FROM hlr_tt_lawfull_intercpt_info "
  << "WHERE msisdn = '" << msisdn << "' "
  << "OR (prefix = '1' AND msisdn = SUBSTR('" << msisdn << "', 1, LENGTH(msisdn)))";
  DbUtil::ResultSet result = DbUtil::executeQuery(query.str());
  if(result.empty())
  {
    loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"No Data found | %s", __FUNCTION__);
    return ROWS_NOT_FOUND;
  }
  DbUtil::Row &row = result.front();
  l_li_enabled_count =row.at("li_enabled_count");
  loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"<l_li_enabled_count=%s> | %s",l_sms_filter_enabled.c_str(), __FUNCTION__);
  return 0;
}  


int Subscriber::funCharToBcd(char *charBuff,bs* byteStrObj)
{

	char    tempBuff[100] = {'\0'};
	int     bcdlength=0;
	int     tempBuff_len=0;
	BCDEncoder  BCDEncoderobj;

	if( (charBuff == NULL) || (byteStrObj == NULL)){
		return 1;
	}

	tempBuff_len=strlen(charBuff);
	if ( tempBuff_len <= 0 ){
		loggerObj.log(LM_INFO,__FILE__,__LINE__,0,0," CharBuff Value NULL | %s ",__FUNCTION__);
		return 1;
	}

	bcdlength=BCDEncoderobj.CharToBCD(tempBuff,charBuff,tempBuff_len);
	byteStrObj->setValue(tempBuff,bcdlength);

	return 0;
}

int Subscriber::funBcdToChar(char *charBuff,bs* byteStrObj)
{

	char    tempBuff[100] = {'\0'};
	int     bcdlength=0;
	int     tempBuff_len=0;
	BCDEncoder  BCDEncoderobj;

	if( (charBuff == NULL) || (byteStrObj == NULL)){
		return 1;
	}

	tempBuff_len= byteStrObj->getValue(tempBuff);
	if ( tempBuff_len <= 0 ){
		loggerObj.log(LM_INFO,__FILE__,__LINE__,0,0," ByteString Return Value is Null| %s ",__FUNCTION__);
		return 1;
	}
	bcdlength=BCDEncoderobj.BCDToChar(charBuff,tempBuff,tempBuff_len);
	charBuff[bcdlength] ='\0';

	return 0;
}

int UpdateSubscriberDb::updateSubscriberGsmInfo()
{
    ostringstream updateQuery;
    updateQuery << "UPDATE HLR_TR_SUB_GSM_INFO "
    << "SET MSPURGED_NONGPRS_FLAG = '" << l_mspurgedNonGprs_c << "', "
    << "NOTREACHABLE_FLAG = '" << l_notreachableGsm_c << "', "
    << "MNRF = '" << l_mnrf_c << "', "
    << "MCEF = '" << l_mcef_c << "', "
    << "MNRR = '" << l_mnrr_c_gsm << "' "
    << "WHERE (IMSI::text = '" << l_imsi_c << "' OR MSISDN::text = '" << l_msisdn_c << "')";
   
    loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Executing update query  l_msisdn_c -------%s| %s",l_msisdn_c,__FUNCTION__);
    
    loggerObj.log(LM_DEBUG, __FILE__, __LINE__, 0, 0, "Executing update query  l_imsi_c -------%s| %s",l_msisdn_c,__FUNCTION__);
    
    bool update_status = DbUtil::executeNonQuery(updateQuery.str());

    if (!update_status)
    {
      cout<<" if -------------------"<<endl;
      loggerObj.log(LM_ERROR, __FILE__, __LINE__, 0, 0,"No rows updated at| IMSI=%s | MSISDN=%s | %s",l_imsi_c, l_msisdn_c, __FUNCTION__);
      return ROWS_NOT_FOUND;
    }
    else
    {
      cout<<"else -------------------"<<endl;
      loggerObj.log(LM_INFO, __FILE__, __LINE__, 0, 0,"Rows updated successfully at | IMSI=%s | MSISDN=%s | %s",l_imsi_c, l_msisdn_c, __FUNCTION__);
      return CODE_SQL_SUCCESS;
    }
}
 
string Subscriber::toString() 
{
  ostringstream oss;
  oss << "l_gsm_active_roaming_subs_profile_id[" << l_gsm_active_roaming_subs_profile_id << "], " << endl
  << "l_operatorBarring_value["<< l_operatorBarring_value << "]" << endl;
  return oss.str();
}

#endif

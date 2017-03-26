/* --------------------------------------------------------------------------------
 #
 #	4DPlugin.cpp
 #	source generated by 4D Plugin Wizard
 #	Project : BASE64
 #	author : miyako
 #	2017/03/27
 #
 # --------------------------------------------------------------------------------*/


#include "4DPluginAPI.h"
#include "4DPlugin.h"

void PluginMain(PA_long32 selector, PA_PluginParameters params)
{
	try
	{
		PA_long32 pProcNum = selector;
		sLONG_PTR *pResult = (sLONG_PTR *)params->fResult;
		PackagePtr pParams = (PackagePtr)params->fParameters;

		CommandDispatcher(pProcNum, pResult, pParams); 
	}
	catch(...)
	{

	}
}

void CommandDispatcher (PA_long32 pProcNum, sLONG_PTR *pResult, PackagePtr pParams)
{
	switch(pProcNum)
	{
// --- BASE64

		case 1 :
			BLOB_to_base64(pResult, pParams);
			break;

		case 2 :
			BLOB_from_base64(pResult, pParams);
			break;

		case 3 :
			BLOB_to_hex(pResult, pParams);
			break;

		case 4 :
			BLOB_from_hex(pResult, pParams);
			break;

	}
}

// ------------------------------------ BASE64 ------------------------------------

void BLOB_to_base64(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_BLOB Param1;
	C_TEXT returnValue;

	Param1.fromParamAtIndex(pParams, 1);
	Param1.toB64Text(&returnValue);

	returnValue.setReturn(pResult);
}

void BLOB_from_base64(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_BLOB returnValue;

	Param1.fromParamAtIndex(pParams, 1);
	returnValue.fromB64Text(&Param1);

	returnValue.setReturn(pResult);
}

void BLOB_to_hex(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_BLOB Param1;
	C_TEXT returnValue;

	Param1.fromParamAtIndex(pParams, 1);
	Param1.toHexText(&returnValue);

	returnValue.setReturn(pResult);
}

void BLOB_from_hex(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_BLOB returnValue;

	Param1.fromParamAtIndex(pParams, 1);
	returnValue.fromHexText(&Param1);

	returnValue.setReturn(pResult);
}

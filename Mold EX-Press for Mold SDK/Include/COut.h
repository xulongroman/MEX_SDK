/************************************************************************/
/*                            Mold EX-Press for Mold API                         */
/************************************************************************/
#pragma once

/////////////////////////////////////////////////////////////////////////////
// COut

class COut : public COleDispatchDriver
{
public:
	COut() {}		// Calls COleDispatchDriver default constructor
	COut(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	COut(const COut& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:

	/**
	Call the first class interface of Mold EX-Press for Mold.
	Input Parameters:
	   i: value range is integer from 0 to 5; 
	      Mold EX-Press for Mold call the corresponding first class interface according to parameters.
	Return Values:
	   short: meaningless; Mold EX-Press for Mold use it internally.
	**/
	short oShowInterfaceDlg1(short i)
	{
		short result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I2, (void*)&result, parms, i);
		return result;
	}

	/**
	Call the second class interface of Mold EX-Press for Mold for each category.
	Input Parameters:
	   index: value range is integer from 0 to 34; 
	          Mold EX-Press for Mold call the second class interface for the corresponding category according to parameters.
	   C_Value: FALSE; fixed value.
	   Setting: 0; fixed value.
	Return Values:
	   short: meaningless; Mold EX-Press for Mold use it internally.
	**/
	short oShowInterfaceDlg2(short index, BOOL C_Value, short Setting)
	{
		short result;
		static BYTE parms[] = VTS_I2 VTS_BOOL VTS_I2 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I2, (void*)&result, parms, index, C_Value, Setting);
		return result;
	}

	/**
	Call the third class interface of Mold EX-Press for Mold for specific model; used for modifying.
	Input Parameters:
	   strPara: the modify string for specific model
	Return Values:
	   short: meaningless; Mold EX-Press for Mold use it internally.
	**/
	short oShowInterfaceDlg3(LPCTSTR strPara)
	{
		short result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I2, (void*)&result, parms, strPara);
		return result;
	}

	/**
	Get the number of properties in model.
	Return Values:
	   short: the number of properties in model
	**/
	short oGetParaCount()
	{
		short result;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}

	/**
	Get the property name at index position in model.
	Input Parameters:
	   index: value range is integer from 0 to the number of properties; 
	          there is an one-one correlation between the property name and value at the same index position.
	Return Values:
	   CString: the property name at index position in model
	**/
	CString oGetParaName(short index)
	{
		CString result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, index);
		return result;
	}

	/**
	Get the property value at index position in model.
	Input Parameters:
	   index: value range is integer from 0 to the number of properties; 
	          there is an one-one correlation between the property name and value at the same index position
	Return Values:
	   CString: the property value at index position in model
	**/
	CString oGetParaVlaue(short index)
	{
		CString result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, index);
		return result;
	}

    /**
	Get the Type value of model.
	Return Values:
	   CString: the Type value of model
	   Note: the return Type value can be used to check if end-user import model successfully;
	         if the Type is null, that means end-user close Mold EX-Press for Mold before importing. 
    **/
	CString oGetTypeName()
	{
		CString result;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	/**
	Get the Order value of model.
	Return Values:
	   CString: the Order value of model
	**/
	CString oGetOrder()
	{
		CString result;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	/**
	Clear all existing contents in BOM.
	**/
	void oClearBom()
	{
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	/**
	Add the specific model into BOM.
	Input Parameters:
	   cPara: the BOM string for the specific model
	**/
	void oAddToBom(LPCTSTR cPara)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, cPara);
	}

	/**
	Call BOM interface of Mold EX-Press for Mold.
	**/
	void oShowBom()
	{
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	/**
	Initialize Mold EX-Press for Mold API; Can only be call once in one process.
	Input Parameters:
	   i: value range is 1,4,5,6; 
	      different parameters can decide what formats of model files Mold EX-Press for Mold API will generate.
	**/
	void oInit(short i)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, i);
	}

	/**
	Terminate Mold EX-Press for Mold API; Can only be call once in one process.
	**/
	void oUninit()
	{
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	/**
	Call setting interface of Mold EX-Press for Mold.
	Input Parameters:
	   VALUE: value range is integer from 0 to 7, 30; 
	          if 0 <= value <= 7, Mold EX-Press for Mold call the corresponding setting interface according to parameters;
	          if value = 30, Mold EX-Press for Mold call the "Custom Properties" interface
	**/
	void oShowDlg_Menu_Mold_Jap(short VALUE)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms, VALUE);
	}

	/**
	Call "Add Customized BOM info" interface of Mold EX-Press for Mold.
	**/
	void oShowProductAddDlg()
	{
		InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}


};

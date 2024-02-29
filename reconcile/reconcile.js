
var trx_index   = 0;
var trx_amount  = 1;
var trx_include = 2;

function SetStartDate( Obj, DateObj, BalanceObj )
{
	for ( var xa = 0; xa < acctcount; xa++ )
	{
		if ( Obj.value == acctdetail[xa][0] )
		{
			DateObj.value = acctdetail[xa][1];
			BalanceObj.value = acctdetail[xa][2] / 100.0;
			return;
		}
	}
}

function RoundTwoDec ( dblNumber )
{
	var intNumber = 0;
	var intCents = 0;
	var intDollars = 0;
	var strAnswer = '0.0';
	var NegNumber = 0;

	if ( dblNumber < 0.0 )
	{
		NegNumber = 1;
		dblNumber = 0.0 - dblNumber;
	}

	intDollars = Math.floor ( dblNumber );

	intCents   = Math.floor ( dblNumber * 100 );
	intCents   = intCents % 100;

	if ( intCents == 0 )
	{
		strAnswer = intDollars + '.00';
	}
	else if ( intCents < 10 )
	{
		strAnswer = intDollars + '.0' + intCents;
	}
	else if ( (intCents % 10) == 0 )
	{
//		strAnswer = intDollars + '.' + (intCents / 10) + '0';
		strAnswer = intDollars + '.' + intCents;
	}
	else
	{
		strAnswer = intDollars + '.' + intCents;
	}
	
	if ( NegNumber == 1 )
	{
		strAnswer = '-' + strAnswer;
	}

	return ( strAnswer );
}

function CalcBalances ()
{
	var pgMinus = 0.0;
	var pgPlus  = 0.0;
	var pgTotal = StartBalance;

	for ( var xt = 0; xt < TrxCount; xt++ )
	{
		if ( TrxArray[xt][trx_include] >= 1 )
		{
			if ( TrxArray[xt][trx_amount] > 0.0 )
			{
				pgPlus += ( 1.0 * TrxArray[xt][trx_amount] );
			}

			if ( TrxArray[xt][trx_amount] < 0.0 )
			{
				pgMinus += ( 1.0 * TrxArray[xt][trx_amount] );
			}

			pgTotal += ( 1.0 * TrxArray[xt][trx_amount] );
		}
	}
	pgTotal -= ( 1.0 * EndBalance );

	TopElement = window.document.getElementById ( 'PlusBalance' );
	MyNode = TopElement.childNodes[0];
	MyNode.nodeValue = RoundTwoDec ( pgPlus );

	TopElement = window.document.getElementById ( 'MinusBalance' );
	MyNode = TopElement.childNodes[0];
	MyNode.nodeValue = RoundTwoDec ( pgMinus );

	TopElement = window.document.getElementById ( 'TotalBalance' );
	MyNode = TopElement.childNodes[0];
	MyNode.nodeValue = RoundTwoDec ( pgTotal );
}

function SetMarks ( Prefix )
{
	var FieldName = 'abc';

//	alert ( Prefix );
	for ( var Index = 0; Index < document.reconcile.elements.length; Index++ )
	{
		FieldName = document.reconcile.elements[Index].name;
		if ( FieldName.substr(0,5) == Prefix )
		{
			document.reconcile.elements[Index].checked = true;
		}
	}

	if ( Prefix == 'rec_m' )
	{
		for ( var xt = 0; xt < TrxCount; xt++ )
		{
			if ( TrxArray[xt][trx_amount] < 0.0 && TrxArray[xt][trx_include] == 0 )
			{
				TrxArray[xt][trx_include] = 1;
			}
		}
	}
	else
	{
		for ( var xt = 0; xt < TrxCount; xt++ )
		{
			if ( TrxArray[xt][trx_amount] > 0.0 && TrxArray[xt][trx_include] == 0 )
			{
				TrxArray[xt][trx_include] = 1;
			}
		}
	}

	CalcBalances ();
}

function ClearMarks ( Prefix )
{
	var FieldName = 'abc';

//	alert ( Prefix );
	for ( var Index = 0; Index < document.reconcile.elements.length; Index++ )
	{
		FieldName = document.reconcile.elements[Index].name;
		if ( FieldName.substr(0,5) == Prefix )
		{
			document.reconcile.elements[Index].checked = false;
		}
	}

	if ( Prefix == 'rec_m' )
	{
		for ( var xt = 0; xt < TrxCount; xt++ )
		{
			if ( TrxArray[xt][trx_amount] < 0.0 && TrxArray[xt][trx_include] == 1 )
			{
				TrxArray[xt][trx_include] = 0;
			}
		}
	}
	else
	{
		for ( var xt = 0; xt < TrxCount; xt++ )
		{
			if ( TrxArray[xt][trx_amount] > 0.0 && TrxArray[xt][trx_include] == 1 )
			{
				TrxArray[xt][trx_include] = 0;
			}
		}
	}

	CalcBalances ();
}

function Adjust ( Index, cbField )
{
	if ( cbField.checked )
	{
		TrxArray[Index][trx_include] = 1;
	}
	else
	{
		TrxArray[Index][trx_include] = 0;
	}

	CalcBalances ();
}



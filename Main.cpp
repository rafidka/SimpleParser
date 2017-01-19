/*
	Filename: Main.cpp
	Description: This file is the main file in the application.

	Created by: Rafid K. Abdullah

	Created on: Monday, March 01, 2004
	Last modified on: Sunday, April 04, 2004
----------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

//****************************************************************************************
// Functions Declarations
//****************************************************************************************

bool VerifyExpression(const char * strExpression);
bool EvaluateExpression(const char * strExpression, double & fResult);
bool EvaluateTerm(const char * strExpression, int nTermLength, double & fResult);
bool StrToFloat(const char * str, int nLength, double & f);



//****************************************************************************************
// Functions Definitions
//****************************************************************************************

//----------------------------------------------------------------------------------------
// Function: main
// Remarks: It is the entry point of any Command Prompt application.
//
// Created on: Monday, March 01, 2004
// Last modified on: Monday, April 5, 2004
//----------------------------------------------------------------------------------------
int main()
{
	char strExpression[100];

	// Asks the user to enter the mathematical expression to be evaluated.
	printf("Enter the expression:\n"); scanf("%s", strExpression);

	// Evaluates the expression.
	double fResult;
	if (!EvaluateExpression(strExpression, fResult))
		printf("Invalid expression\n");
	else
		printf("%f\n", fResult);

	return 0;
}

//----------------------------------------------------------------------------------------
// Function: VerifyExpression
// Remarks: Verifies that the mathematical expression given is a valid one.
//
// Parameters: strExpression = a pointer to a string that contains the mathematical
//                expression.
//
// Return Value: Returns 'true' if the expression is valid; otherwise return 'false'.
//
// Created on: Monday, March 01, 2004
// Last modified on: Sunday, April 04, 2004
//----------------------------------------------------------------------------------------
bool VerifyExpression(const char * strExpression)
{
	int nLength = strlen(strExpression);
	if (nLength == 0)
		return false;

	// The expression should only contains +, -, *, /, and numbers (including the decimal point).
	for (int i = 0; i < nLength; i++)
	{
		if (strExpression[i] >= '0' && strExpression[i] <= '9')
			continue;
		else
			if (strExpression[i] == '+' || strExpression[i] == '-' ||
				strExpression[i] == '*' || strExpression[i] == '/' ||
				strExpression[i] == '.')
				continue;
			else
				// An invalid character!!!
				return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------
// Function: EvaluateExpression
// Remarks: Evaluates the mathematical expression specified by (strExpression) and returns
//    its value.
//
// Parameters: strExpression = a pointer to a string containing the mathematical expression
//                to be evaluated.
//             fResult = a reference to a double that will contain the result of the expression.
//
// Return Value: 'true' if the function succeeded; otherwise 'false'.
//
// Created on: Monday, March 01, 2004
// Last modified on: Sunday, April 04, 2004
//----------------------------------------------------------------------------------------
bool EvaluateExpression(const char * strExpression, double & fResult)
{
	// Verifies the expression before evaluating.
	if (!VerifyExpression(strExpression))
		// Invalid expression; returns 'false'.
		return false;

	// Evaluates the value of each term.
	int nLength = strlen(strExpression);
	int nEnd = nLength - 1;
	double fTerm;
	fResult = 0.0;
	// Searches the expression from right to left for a '+' or '-', i.e. partition the expression
	//  into terms.
  int i;
	for (i = nLength - 1; i >= 0; i--)
		if (strExpression[i] == '+')
		{
			if (!EvaluateTerm(strExpression + i + 1, nEnd - i, fTerm))
				// Invalid term!!!
				return false;
			fResult += fTerm;
			nEnd = i-1;
		}
		else
			if (strExpression[i] == '-')
			{
				if (!EvaluateTerm(strExpression + i + 1, nEnd - i, fTerm))
					// Invalid term!!!
					return false;
				fResult -= fTerm;
				nEnd = i-1;
			}

	// The first term is often not preceded by a sign!!!
	if (strExpression[0] != '+' && strExpression[0] != '-')
	{
		if (!EvaluateTerm(strExpression, nEnd + 1, fTerm))
			// Invalid term!!!
			return false;
		fResult += fTerm;
		nEnd = i-1;
	}

	return true;
}

//----------------------------------------------------------------------------------------
// Function: EvaluateTerm
// Remarks: Evaluates the mathematical term specified by (strTerm) and returns its value.
//
// Parameters: strTerm = a pointer to a string containing the mathematical term to be
//                evaluated.
//             nTermLength = the length of the term to be evaluated.
//             fResult = a reference to a double that will contain the result of the term.
//
// Return Value: 'true' if the function succeeded; otherwise 'false'.
//
// Created on: Monday, March 01, 2004
// Last modified on: Sunday, March 01, 2004
//----------------------------------------------------------------------------------------
bool EvaluateTerm(const char * strTerm, int nTermLength, double & fResult)
{
	// Evaluates the value of each term.
	int nEnd = nTermLength - 1;
	fResult = 1.0;
	double fNumber;
	// Searches the term from right to left for a '*' or '/', i.e. partition the terms
	//  into numbers.
	for (int i = nEnd; i >= 0; i--)
		if (strTerm[i] == '*')
		{
			if (!StrToFloat(strTerm + i + 1, nEnd - i, fNumber))
				// Invalid number!!!
				return false;
			fResult *= fNumber;
			nEnd = i-1;
		}
		else
			if (strTerm[i] == '/')
			{
				if (!StrToFloat(strTerm + i + 1, nEnd - i, fNumber))
					// Invalid number!!!
					return false;
				fResult /= fNumber;
				nEnd = i-1;
			}

	// The first term is often not preceded by a sign!!!
	if (strTerm[0] != '*' && strTerm[0] != '/')
	{
		if (!StrToFloat(strTerm, nEnd + 1, fNumber))
			// Invalid number!!!
			return false;
		fResult *= fNumber;
	}

	return true;
}

//----------------------------------------------------------------------------------------
// Function: StrToFloat
// Remarks: Converts a string to the corresponding deciaml number.
//
// Parameters: str = a pointer to a string containing the deciaml number to be obtained.
//             nLength = the length of the string.
//             f = a reference to a double which will receive the decimal number represented
//                by the string.
//
// Return Value: 'true' if the function succeeded; otherwise 'false'.
//
// Created on: Monday, March 01, 2004
//----------------------------------------------------------------------------------------
bool StrToFloat(const char * str, int nLength, double & f)
{
	if (!str)
		// Invalid poitner.
		return false;

	double factor = 0.1;
	// Scans for the decimal point, if there is.
  int i;
	for (i = 0; i < nLength; i++)
	{
		if (str[i] == '.')
		{
			// Verifies that the remaining of the string is a valid decimal number.
			for (int j = i + 1; j < nLength; j++)
				if (str[j] < '0' || str[j] > '9')
					return false;
			break;
		}
		else
			// Verifies that there is no invalid character in the string.
			if (str[i] < '0' || str[i] > '9')
				return false;
		factor *= 10;
	}

	f = 0.0;
	for (i = 0; i < nLength; i++)
		if (str[i] == '.')
			continue;
		else
		{
			f += (double)(str[i] - '0')*factor;
			factor *= 0.1;
		}

	return true;
}

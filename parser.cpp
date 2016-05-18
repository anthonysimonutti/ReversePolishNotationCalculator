
#include <stdio.h>
#include <iostream>
#include <string>
#include <algorithm>

enum type {notSet = 0, Plus = 1, Minus = 2, Multiply = 3, Divide = 4, Number = 5, LParen = 6, RParen = 7, End = 8};


class Token
{
private:
	type kind;
	double value;

public:
	Token()
	{
		kind = notSet;
		value = (double) NULL;
	}

	void setType(type typeToSet)
	{
		kind = typeToSet;
	}

	type getType()
	{
		return kind;
	}

	void setValue(double valueToSet)
	{
		value = valueToSet;
	}

	double getValue()
	{
		return value;
	}

	bool unknown()
	{
		if(kind == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

class Lexer
{
private:
	std::string input;
	bool returnPreviousToken;
	Token previousToken;

public:
	void initialize(std::string tempInput)
	{
		input = tempInput;
		returnPreviousToken = false;
		previousToken.setType(notSet);
	}

	Token getNextToken()
	{
		if(returnPreviousToken)
		{
			returnPreviousToken = false;
			return previousToken;
		}

		Token toke;
		if(input.size() == 0)
		{
			toke.setType(End);
		}
		else
		{
			char firstInput = input[0];
			int numCharToDelete = 1;
			if(firstInput == '+')
			{
				toke.setType(Plus);
			}
			else if(firstInput == '-')
			{
				toke.setType(Minus);
			}
			else if(firstInput == '*')
			{
				toke.setType(Multiply);
			}
			else if(firstInput == '/')
			{
				toke.setType(Divide);
			}
			else if(firstInput == '(')
			{
				toke.setType(LParen);
			}
			else if(firstInput == ')')
			{
				toke.setType(RParen);
			}
			else if(isdigit(firstInput))
			{
				std::string temp = "";
				temp.push_back(input[0]);
				bool isLastDigit = true;
				int i = 1;
				while(isLastDigit)
				{
					if(i < input.size())
					{
						if((isdigit(input[i]) || input[i] == '.'))
						{
							temp.push_back(input[i]);
							i++;
						}
						else
						{
							isLastDigit = false;
						}
					}
					else
					{
						isLastDigit = false;
					}
				}
				numCharToDelete = i;
				toke.setType(Number);
				toke.setValue(strtod(temp.c_str(), NULL));
			}
			else
			{
				toke.setType(notSet);
			}

			input.erase(input.begin(), input.begin() + numCharToDelete);
		}
		previousToken = toke;
		return toke;

	}

	void revert()
	{
		returnPreviousToken = true;
	}
};

class Parser
{
private:
	Lexer lex;

public:
	double parse(std::string input)
	{
		lex.initialize(input);
		double expressionValue = this->expression();
		Token toke = lex.getNextToken();
		if(toke.getType() == End)
		{
			return expressionValue;
		}
		else
		{
			// ERROR: End expected!
			throw 2;
		}
	}

protected:
	double expression()
	{
		double component1 = this->factor();
		Token currentToken = lex.getNextToken();
		while(currentToken.getType() == Plus || currentToken.getType() == Minus)
		{
			double component2 = this->factor();
			if(currentToken.getType() == Plus)
			{
				component1 += component2;
			}
			else if(currentToken.getType() == Minus)
			{
				component1 -= component2;
			}

			currentToken = lex.getNextToken();
		}
		lex.revert();

		return component1;
	}

	double factor()
	{
		double factor1 = this->number();
		Token currentToken = lex.getNextToken();
		while(currentToken.getType() == Multiply || currentToken.getType() == Divide)
		{
			double factor2 = this->number();

			if(currentToken.getType() == Multiply)
			{
				factor1 *= factor2;
			}
			else if(currentToken.getType() == Divide)
			{
				factor1 /= factor2;
			}

			currentToken = lex.getNextToken();
		}
		lex.revert();

		return factor1;
	}

	double number()
	{
		double value;
		Token currentToken = lex.getNextToken();

		if(currentToken.getType() == LParen)
		{
			value = this->expression();

			Token nextToken = lex.getNextToken();
			if(nextToken.getType() != RParen)
			{
				// ERROR: Unbalanced parentheses!
				throw 1;
			}
		}
		else if(currentToken.getType() == Number)
		{
			value = currentToken.getValue();
		}
		else
		{
			// ERROR: Invalid input!
			throw 0;
		}

		return value;
	}
};

int parse()
{
	std::string errors[4] = {"ERROR: Not valid input!\n", "ERROR: Unbalanced parentheses!\n", "ERROR: End expected!\n", "ERROR: Unknown error.\n"};
	Parser par;

	std::cout << "Type q to quit\n";
	while(1)
	{
		std::string input;
		std::cout << ">>";
		std::getline(std::cin, input);
		if(input == "q")
		{
			break;
		}
		input.erase(remove_if(input.begin(), input.end(), isspace), input.end());
		try
		{
			double result = par.parse(input);
			std::cout << "Result: " << result << "\n";
		}
		catch(int exception)
		{
			try
			{
				std::cout << errors[exception];
			}
			catch(int ex)
			{
				std::cout << "ERROR: Unknown error!\n";
			}
		}
	}
	return 0;
}

int main()
{
	parse();
	return 0;
}

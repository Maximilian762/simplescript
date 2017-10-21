#pragma once
#include <memory>
#include <string>
//#include "Table.h"
#include <vector>

class Table;

enum class VarType
{
	Unassigned,
	Int,
	Float,
	String,
	Table
};

class Variable
{
public:
	Variable();
	Variable(int _c);
	Variable(float _c);
	Variable(std::string _c);
	Variable(std::shared_ptr<Table> _c);
	Variable(Table *_c);
	Variable(const Variable& other, bool independent);
	~Variable();

	VarType Type();
	bool IntOrFloat();
	float GetNumber();
	int& GetIntRef();
	float& GetFloatRef();
	std::string& GetStringRef();
	std::shared_ptr<Table> GetTablePtr();

	bool SameType(Variable &other);
	std::string GetTypeName();

	std::string ToString();
	std::string GetStringRepresentation();

	void SetType(std::string type_name);

	void BecomeTable(std::shared_ptr<Table> _c);
private:

	VarType type = VarType::Unassigned;
	std::string type_name;

	int intcontent = 0;
	float floatcontent = 0;
	std::string stringcontent;
	std::shared_ptr<Table> tablecontent;
};

typedef std::vector<std::string> VarList;
typedef std::shared_ptr<Variable> VarPtr;
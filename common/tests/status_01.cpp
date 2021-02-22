#include <common/Status.h>
#include <iostream>

int main(int argc, char **argv)
{
	Status st;

	std::cout << "Checking SUCCES:"<< std::endl;
	std::cout << "String: "<< st << std::endl;
	if (!(st == Status::SUCCESS)) {
		std::cout << "Operator equal type failed" << std::endl;
		return -1;
	}
	if (st != Status::SUCCESS) {
		std::cout << "Operator not equal type failed" << std::endl;
		return -1;
	}
	if (!(st == string("Success"))) {
		std::cout << "Operator string equal type failed" << std::endl;
		return -1;
	}
	if (st != 0) {
		std::cout << "Operator equal type failed" << std::endl;
		return -1;
	}	std::cout << "OK" << std::endl;
	if (bool(st)) {
		std::cout << "Operator bool type failed" << std::endl;
		return -1;
	}	
	std::cout << "OK" << std::endl;
	
	st = Status(string("Error"), Status::ERROR);
	std::cout << "Checking ERROR:"<< std::endl;
	std::cout << "String: "<< st << std::endl;
	if (!(st == Status::ERROR)) {
		std::cout << "Operator equal type failed" << std::endl;
		return -1;
	}
	if (st != Status::ERROR) {
		std::cout << "Operator not equal type failed" << std::endl;
		return -1;
	}
	if (!(st == string("Error"))) {
		std::cout << "Operator string equal type failed" << std::endl;
		return -1;
	}
	if (!bool(st)) {
		std::cout << "Operator bool type failed" << std::endl;
		return -1;
	}	std::cout << "OK" << std::endl;
	std::cout << "OK" << std::endl;

	st = Status::ERROR_XML_CANNOT_OPEN;
	std::cout << "Checking status code assigment operator:"<< std::endl;
	std::cout << "String: "<< st << std::endl;
	if (!(st == Status::ERROR_XML_CANNOT_OPEN)) {
		std::cout << "Operator equal type failed" << std::endl;
		return -1;
	}
	if (!(st == string("Error"))) {
		std::cout << "Operator string equal type failed" << std::endl;
		return -1;
	}
	std::cout << "OK" << std::endl;

	st.setErrorMsg( string("test_str"));
	std::cout << "Checking status code/string setters:"<< std::endl;
	std::cout << "String: "<< st << std::endl;

	if (!(st == string("test_str"))) {
		std::cout << "Operator string equal type failed" << std::endl;
		return -1;
	}
	if (!(st == Status::ERROR_XML_CANNOT_OPEN)) {
		std::cout << "Operator equal type failed" << std::endl;
		return -1;
	}

	st.setErrorCode( Status::ERROR);
	std::cout << "String: "<< st << std::endl;

	if (!(st == string("test_str"))) {
		std::cout << "Operator string equal type failed" << std::endl;
		return -1;
	}
	if (!(st == Status::ERROR)) {
		std::cout << "Operator equal type failed" << std::endl;
		return -1;
	}
	std::cout << "OK" << std::endl;

	std::cout << "Checking return Status function:"<< std::endl;
	if (bool(Status::returnStatusOkay())) {
		std::cout << "Operator bool type failed" << std::endl;
		return -1;
	}	
	if (!bool(Status::returnStatusError())) {
		std::cout << "Operator bool type failed" << std::endl;
		return -1;
	}	

	std::cout << "OK" << std::endl;

	return 0;
}


